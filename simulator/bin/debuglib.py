#!/usr/bin/python
# Copyright (c) 2003-2005, Cavium Inc.. All rights reserved.
#
# This Software is the property of Cavium Inc..  The Software and all
# accompanying documentation are copyrighted.  The Software made available
# here constitutes the proprietary information of Cavium Inc..  You
# agree to take reasonable steps to prevent the disclosure, unauthorized use
# or unauthorized distribution of the Software.  You shall use this Software
# solely with Cavium hardware.
#
# Except as expressly permitted in a separate Software License Agreement
# between You and Cavium Inc., you shall not modify, decompile,
# disassemble, extract, or otherwise reverse engineer this Software.  You
# shall not make any copy of the Software or its accompanying documentation,
# except for copying incident to the ordinary and intended use of the
# Software and the Underlying Program and except for the making of a single
# archival copy.
#
# This Software, including technical data, may be subject to U.S.  export
# control laws, including the U.S.  Export Administration Act and its
# associated regulations, and may be subject to export or import regulations
# in other countries.  You warrant that You will comply strictly in all
# respects with all such regulations and acknowledge that you have the
# responsibility to obtain licenses to export, re-export or import the
# Software.
#
# TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
# AND WITH ALL FAULTS AND CAVIUM MAKES NO PROMISES, REPRESENTATIONS OR
# WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH RESPECT
# TO THE SOFTWARE, INCLUDING ITS CONDITION, ITS CONFORMITY TO ANY
# REPRESENTATION OR DESCRIPTION, OR THE EXISTENCE OF ANY LATENT OR PATENT
# DEFECTS, AND CAVIUM SPECIFICALLY DISCLAIMS ALL IMPLIED (IF ANY) WARRANTIES
# OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR
# PURPOSE, LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET
# POSSESSION OR CORRESPONDENCE TO DESCRIPTION.  THE ENTIRE RISK ARISING OUT
# OF USE OR PERFORMANCE OF THE SOFTWARE LIES WITH YOU.
#
# File version info: $Id: debuglib.py 165374 2017-08-28 19:15:44Z cchavva $

import sys
import os
import bisect
import re
import array
import stat
from gdbstub import fromHex, toHex
from gdbstub import REGISTER_NAMES

REGISTER_REF = ["DW_OP_reg%d" % r for r in xrange(32)]

#
# Convert a CVS tag in a name keyword into a Cavium version number
#
def getSdkVersion():
    try:
        f = open(os.getenv("OCTEON_ROOT", "") + "/host/bin/oct-version", "r")
        version = f.readline().strip()
        f.close();
        if version.startswith("echo"):
            return version[4:].strip()
        else:
            return version
    except:
        return "Unable to read oct-version"

#
# Represents the debugging information for an address
#
class DebugLineInfo:
    def __init__(self, address, sourcefile, row):
        self.address = address
        self.sourcefile = sourcefile
        self.row = row
    def __str__(self):
        return "%s => %s:%d" % (toHex(self.address, 16), self.sourcefile, self.row)

class DebugLineContainer:
    def __init__(self):
        self.list = []
        self.need_sort = 0

    def add(self, entry):
        self.list.append(entry)
        self.need_sort = 1;

    def getCount(self):
        return len(self.list)

    def lookup(self, find_address):
        length = len(self.list)
        if not length:
            return None
        if self.need_sort:
            self.need_sort = 0;
            self.list.sort()
        loc = bisect.bisect_left(self.list, (find_address, None))
        if loc == length:
            loc -= 1
        (address, source, row) = self.list[loc]
        if (address <= find_address) and (address > find_address - 256):
            return DebugLineInfo(address, source, row)
        elif loc > 0:
            (address, source, row) = self.list[loc-1]
            if (address <= find_address) and (address > find_address - 256):
                return DebugLineInfo(address, source, row)
            else:
                return None
        else:
            return None

    def print_info(self):
        length = len(self.list)
        for i in xrange(length):
            (address, source, row) = self.list[i]
            print "%x %s %d " % (address, source, row)

#
# Represents the Object tree. Objects in the tree represent types, variables,
# functions, etc ...
#
class DebugObjectInfo:
    def __init__(self, object_id, privateDwarfTree):
        self.object_id = object_id
        self.pt = privateDwarfTree
        self.object_type = self.pt.index_to_string(self.pt.node_type[object_id])

    def children(self):
        child = self.pt.node_child_first[self.object_id]
        while child != -1:
            yield DebugObjectInfo(child, self.pt)
            child = self.pt.node_next_sibling[child]

    def attributes(self):
        base = self.pt.node_attribute_first[self.object_id]
        num = self.pt.node_attribute_num[self.object_id]
        for i in xrange(num):
            a = base + i*2
            name = self.pt.index_to_string(self.pt.attributes[a])
            value = self.pt.index_to_string(self.pt.attributes[a+1])
            yield name, value

    def get(self, name, default=None, depth=0):
        base = self.pt.node_attribute_first[self.object_id]
        num = self.pt.node_attribute_num[self.object_id]
        match = self.pt.string_to_index(name)
        for i in xrange(num):
            a = base + i*2
            if match == self.pt.attributes[a]:
                return self.pt.index_to_string(self.pt.attributes[a+1])
        if (name != "DW_AT_abstract_origin") and (depth < 10):
            origin = self.get("DW_AT_abstract_origin", "", depth+1)
            if origin:
                offset = int(fromHex(origin[1:-1]))
                compile_unit = self.pt.object_id_to_compile_unit[self.object_id]
                object_id = self.pt.compile_unit_offset_to_object_id[(compile_unit,offset)]
                return DebugObjectInfo(object_id, self.pt).get(name, default, depth+1)
        if default == None:
            raise AttributeError()
        else:
            return default

    # Find the node with the associated name
    def findNode(self, search_name):
        if name == self.get("DW_AT_name", ""):
            return self
        else:
            for c in self.children():
                result = c.findNode(search_name)
                if result:
                    return result
        return None

    def containsPC(self, address):
        low = self.get("DW_AT_low_pc", "")
        if low:
            high = self.get("DW_AT_high_pc", low)
            low = fromHex(low)
            high = fromHex(high)
            return (address >= low) and (address < high)
        else:
            return 0

    # Format a block of binary data for display using the current type node
    def formatType(self, binary_data, offset, prefix):
        if self.object_type == "DW_TAG_typedef":
            # Ignore typedefs and traverse to their actual type
            return self.get("DW_AT_type","").formatType(binary_data, offset, prefix)

        elif self.object_type == "DW_TAG_union_type":
            # A union type is unique in that each of its members gets the
            # same data. Data isn't consumed until the entire union is parsed
            size = int(self.get("DW_AT_byte_size", ""))
            prefix += ".union"
            result = []
            for c in self.children():
                result.extend(c.formatType(binary_data, offset, prefix))
            return result

        elif self.object_type == "DW_TAG_structure_type":
            # Structures are just the addition of all members. We
            # copy the data since I don't trust the members to consume
            # the proper amount of data
            size = int(self.get("DW_AT_byte_size", ""))
            result = []
            for c in self.children():
                loc = int(c.get("DW_AT_data_member_location", "").split()[1])
                result.extend(c.formatType(binary_data, offset + loc, prefix))
            return result

        elif self.object_type == "DW_TAG_member":
            # Member of a structure or union
            name = self.get("DW_AT_name", "")
            if self.get("DW_AT_bit_size", ""):
                bits = int(self.get("DW_AT_bit_size", ""))
                size = int(self.get("DW_AT_byte_size", ""))
                shift = size*8 - bits - int(self.get("DW_AT_bit_offset", "0"))
                child_data = 0
                for i in xrange(size):
                    child_data = child_data * 256 + binary_data[i + offset]
                child_data = (child_data >> shift) & ((1<<bits) - 1)
                child_list = range(size)
                for i in xrange(size):
                    child_list[i] = (child_data >> 8*(size-i-1)) & 0xff
                return self.get("DW_AT_type", "").formatType(child_list, 0, prefix + "." + name)
            else:
                return self.get("DW_AT_type", "").formatType(binary_data, offset, prefix + "." + name)

        elif self.object_type == "DW_TAG_base_type":
            # Finally we've gotten down to the base types
            typename = self.get("DW_AT_name", "")
            size = int(self.get("DW_AT_byte_size", ""))
            encoding = self.get("DW_AT_encoding", "")
            if encoding == "DW_ATE_unsigned" or encoding == "DW_ATE_unsigned_char":
                value = 0
                for i in xrange(size):
                    value = value * 256 + binary_data[i + offset]
                value = str(value) + "(" + hex(value) + ")"
            else:
                raise Exception("Illegal encoding: " + encoding)
            return [(prefix, value)]

        elif self.object_type == "DW_TAG_enumeration_type":
            # Display the value as well as the name for enumerations
            value = 0
            size = int(self.get("DW_AT_byte_size", ""))
            for i in xrange(size):
                value = value * 256 + binary_data[i + offset]
            text_value = ""
            for c in self.children():
                if value == int(c.get("DW_AT_const_value", "")):
                    text_value = c.get("DW_AT_name", "")
                    break
            return [(prefix, str(value) + "(" + text_value + ")")]

        elif self.object_type == "DW_TAG_pointer_type":
            # Simple pointer somewhere. We don't follow it
            value = 0
            size = int(self.get("DW_AT_byte_size", ""))
            for i in xrange(size):
                value = value * 256 + binary_data[i + offset]
            return [(prefix, hex(value) + "(pointer)")]

        elif self.object_type == "DW_TAG_array_type":
            # Fixed size Array of objects
            upper = int(self.children().get("DW_AT_upper_bound", ""))
            typenode = self.get("DW_AT_type", "")
            while not typenode.get("DW_AT_byte_size", ""):
                typenode = typenode.get("DW_AT_type", "")
            size = int(typenode.get("DW_AT_byte_size", ""))
            result = []
            for i in xrange(upper+1):
                result.extend(typenode.formatType(binary_data, offset + i * size, prefix + "[" + str(i) + "]"))
            return result

        else:
            # Something not supported yet
            raise Exception("Unsupported object type: " + self.object_type)

    # Display myself and my children.
    def display(self, indent=""):
        print indent, self.object_id, self.object_type
        for a,v in self.attributes():
            print indent, a, v
        for c in self.children():
            c.display(indent + "    ")

class PrivateDwarfTree:
    def __init__(self):
        self.num_string_mapping = 0
        self.string_mapping = {}
        self.reverse_string_mapping = {}
        self.object_id_to_compile_unit = {}       # The key is object_id, the valye - the compile_unit in which it resides
        self.compile_unit_offset_to_object_id = {}    # The key is a tuple (compile_unit,offset) and it points to object_id which resides in that compile unit at the specified offset
        self.attributes = array.array('i')
        self.node_type = array.array('i')
        self.node_attribute_first = array.array('i')
        self.node_attribute_num = array.array('i')
        self.node_child_first = array.array('i')
        self.node_next_sibling = array.array('i')
        self.progress = ""
    def string_to_index(self, str):
        str = str.strip()
        try:
            return self.string_mapping[str]
        except:
            self.string_mapping[str] = self.num_string_mapping
            self.num_string_mapping += 1
            return self.num_string_mapping-1
    def index_to_string(self, index):
        try:
            return self.reverse_string_mapping[index]
        except:
            for k,i in self.string_mapping.iteritems():
                self.reverse_string_mapping[i] = k
            return self.reverse_string_mapping[index]

    def fix_node_numbers(self, node_id_mapping):
        for node_id,node_num in node_id_mapping.iteritems():
            old_key = "<%d>" % node_id
            if old_key in self.string_mapping:
                new_key = "<<%d>>" % node_num
                id = self.string_mapping[old_key]
                del self.string_mapping[old_key]
                self.string_mapping[new_key] = id

    def showProgress(self, file_handle, message):
        loc = file_handle.tell()
        total_filesize = os.fstat(file_handle.fileno())[stat.ST_SIZE]
        self.progress += message
        sys.stdout.write("\r\x1b[K"+self.progress.__str__() + '\t\t%d%% Complete' % (100*loc/total_filesize))
        sys.stdout.flush()

    #
    # Read the object (types, variables, etc...) from an ELF file
    # This can take a while...
    #
    def readDwarfTree(self, filename):
        TMP_FILE = "top_debuginfo_%s.tmp" % (filename.split('/')[-1])
        self.attributes = array.array('i')
        self.node_type = array.array('i')
        self.node_attribute_first = array.array('i')
        self.node_attribute_num = array.array('i')
        self.node_child_first = array.array('i')
        self.node_next_sibling = array.array('i')

        node_id_mapping = {}
        # This is the root node of the tree
        self.node_type.append(self.string_to_index("root"))
        self.node_attribute_first.append(0)
        self.node_attribute_num.append(0)
        self.node_child_first.append(-1)
        self.node_next_sibling.append(-1)
        node_id_mapping[0] = 0
        last_children = [-1]
        parent = [0]
        scope = 1
        compile_unit = -1
        # Regular expression used to extract object node information
        object_re = re.compile("^< *([0-9]+)><0x([0-9a-f]*)>  ([a-zA-Z0-9_\- ]+)$")
        object_const = re.compile("([a-zA-Z0-9_:%,\- ]+)")

        print "Creating dwarfdump for %s" % filename
        if not os.path.isfile(TMP_FILE):
            os.system("dwarfdump -i \"" + filename + "\" > " + TMP_FILE)
        print "Reading Dwarftree"
        lineno = 0
        infile = open(TMP_FILE, "r") 
        for line in infile:
            lineno += 1
            if (lineno & 0xfffff) == 0:
                self.showProgress(infile, ".")
            # Strip the newline
            line = line[0:-1]
            constMatch = object_const.match(line)
            if line.strip() == "":
                continue
            # Found an attribute for an object
            elif line[0:20] == "                    ":
                if line.strip().count(" ") < 1:
                    (attrib, value) = (line.strip(), "")
                else:
                    (attrib, value) = line.strip().split(" ", 1) 
                if "DW_OP_reg" in value:
                    for i in xrange(31, -1, -1):
                        value = value.replace(REGISTER_REF[i], REGISTER_NAMES[i])
                self.attributes.append(self.string_to_index(attrib))
                self.attributes.append(self.string_to_index(value))
                self.node_attribute_num[-1] += 1
            elif line[0:2] == "\t\t":
                continue
            # Begin a new object
            elif line[0] == '<':
                assert(len(parent) == scope)
                assert(len(last_children) == scope)
                match = object_re.match(line)
                obj_scope = int(match.group(1))+1
                self.node_type.append(self.string_to_index(match.group(3)))
                self.node_attribute_first.append(len(self.attributes))
                self.node_attribute_num.append(0)
                self.node_child_first.append(-1)
                self.node_next_sibling.append(-1)
                current_object = len(self.node_type)-1
                node_id_mapping[int(fromHex(match.group(2)))] = current_object
                self.object_id_to_compile_unit[current_object] = compile_unit
                self.compile_unit_offset_to_object_id[(compile_unit,int(fromHex(match.group(2))))] = current_object
                # Figure out what level of the tree we need to be in
                if obj_scope == scope:
                    assert(scope > 0)
                    p = parent[scope-1]
                    # This object is a sibling to the current object
                    if last_children[scope-1] == -1:
                        assert(self.node_next_sibling[p] == -1)
                        assert(self.node_child_first[p] == -1)
                        self.node_child_first[p] = current_object
                    else:
                        assert(self.node_child_first[p] != -1)
                        assert(self.node_next_sibling[last_children[scope-1]] == -1)
                        self.node_next_sibling[last_children[scope-1]] = current_object
                    last_children[scope-1] = current_object
                elif obj_scope == scope + 1:
                    # This object is a child of the current object
                    parent.append(current_object-1)
                    assert(self.node_child_first[parent[scope]] == -1)
                    assert(self.node_next_sibling[parent[scope]] == -1)
                    self.node_child_first[parent[scope]] = current_object
                    last_children.append(current_object)
                    scope = obj_scope
                elif obj_scope < scope:
                    # This child is from a different root
                    scope = obj_scope
                    parent = parent[0:scope]
                    last_children = last_children[0:scope]
                    p = parent[scope-1]
                    if last_children[scope-1] == -1:
                        assert(self.node_next_sibling[p] == -1)
                        assert(self.node_child_first[p] == -1)
                        self.node_child_first[p] = current_object
                    else:
                        assert(self.node_child_first[p] != -1)
                        assert(self.node_next_sibling[last_children[scope-1]] == -1)
                        self.node_next_sibling[last_children[scope-1]] = current_object
                    last_children[scope-1] = current_object
                else:
                    # This should be impossible
                    raise Exception("Scoping error")
            # Begin a new compile unit. All tables reset
            elif line[0:12] == "COMPILE_UNIT":
                self.fix_node_numbers(node_id_mapping)
                node_id_mapping = {}
                scope = 1
                parent = parent[0:scope]
                last_children = last_children[0:scope]
                compile_unit += 1
            # Skip some useless lines
            elif line in [".debug_info", "LOCAL_SYMBOLS:"]:
                pass
            elif constMatch:
                parts = constMatch.groups()
                #print parts[0]
                continue
            else:
                raise Exception("Unexpected input: " + line)
        infile.close()
        self.fix_node_numbers(node_id_mapping)

#
# Container for all debugging information
#
class DebugInfo:
    def __init__(self):
        self.addressInfo = DebugLineContainer()
        self.objectInfo = None
        self.privateDwarfTree = PrivateDwarfTree()

    #
    # Read debug line information from an ELF file
    #
    def readLineInfo(self, filename):
        infile = os.popen("dwarfdump -l \"" + filename + "\"", "r")
        for line in infile:
            if line == "\n" or line.startswith(".debug_line: line number info") or line.startswith("Source lines") or line.strip().startswith("IS=val") or line.startswith("<pc>") or line.strip().startswith("NS") or line.strip().startswith("PE") or line.startswith(" Line table is") :
                continue
            elif line.startswith("No DWARF information"):
                print line.replace("\n", "")
                break

            line = line[0:-1]
            parts = line.split()

            try:
                parts.remove('[') 
            except:
                pass

            if parts[0].startswith("0x"):
                address = fromHex(parts[0][2:])
            else:
                address = int(parts[0])
            if address == 0:
                break

            if parts[1][0:].startswith("["):
                location = parts[1][1:].split(",")
            else:
                location = parts[1][0:].split(",")

            try:
                absFilepath = parts[5]
                if absFilepath == "":
                    absFilepath = source
                else:
                    if absFilepath == "uri:":
                        absFilepath = parts[6]
                        source = absFilepath
                    else:
                        source = absFilepath
            except:
                source = absFilepath
            
            #print "%s %s %s %d" % (parts[0][0:], address, source, int(location[0])) 
            entry = address, source.replace('\"',''), int(location[0])

            # Stop loading line info if we get a zero address. The ELF hasn't been linked
            if address == 0:
                break
            self.addressInfo.add(entry)
        infile.close()
        #self.addressInfo.print_info();

    #
    # Read the object (types, variables, etc...) from an ELF file
    # This can take a while...
    #
    def readObjectInfo(self, filename):
        self.privateDwarfTree.readDwarfTree(filename)
        self.objectInfo = DebugObjectInfo(0, self.privateDwarfTree)
        #self.objectInfo.display()

    #
    # Read debug information from an ELF file
    #
    def read(self, filename):
        self.readLineInfo(filename)
        #self.readObjectInfo(filename)

    #
    # Get debug info for a specified address
    #
    def getDebugLineInfo(self, find_address):
        return self.addressInfo.lookup(find_address)

    #
    # Find the address of a variable given its name
    #
    def getVariableAddress(self, name):
        if self.objectInfo:
            node = self.objectInfo.findNode(name)
            if node:
                if node.get("DW_AT_location", "").startswith("addr"):
                    return fromHex(node.get("DW_AT_location", "").split()[1][2:])
        return 0

    #
    # Display the data using the supplied typedef
    #
    def formatType(self, typename, hex_data):
        if self.objectInfo:
            node = self.objectInfo.findNode(typename)
            if node:
                data = []
                while hex_data:
                    data.append(int(hex_data[0:2], 16))
                    hex_data = hex_data[2:]
                return node.formatType(data, 0, typename)
        return None
#
# Only execute if run directly
#
if __name__ == "__main__":
    print
    print "Use oct-debuginfo to get line information."
    print

