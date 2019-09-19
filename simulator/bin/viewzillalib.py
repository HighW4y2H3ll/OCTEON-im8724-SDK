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
# File version info: $Id: viewzillalib.py 69916 2012-02-14 14:47:03Z bprakash $
import re

#
# Describes all the data about a hardware unit (PP0_ISS0, PP0_ISS1, ...)
#
class HardwareInfo:
    def __init__(self, name, view_cycles):
        self.name = name.replace("_", " ")
        self.color = ["" for x in xrange(view_cycles)]
        self.label = ["" for x in xrange(view_cycles)]
        self.references = [[] for x in xrange(view_cycles)]

#
# Class for random access to Viewzilla files
#
class Viewzilla:
    def __init__(self, filename="octeon.vz"):
        self.filename = filename    # Name of the file to work on
        self.current_cycle = 0      # Starting cycle location for the current window
        self.view_cycles = 50       # Number of cycles to load
        self.SLOP_FACTOR = 100      # Slop factor to extend on each side in cycles
        self.hw_labels = self.getHwLabels()  # Read the hardware labels that are active
        self.hardware = {}          # Dictionary of all hardware units with data

    #
    # Read the active hw_labels
    #
    def getHwLabels(self):
        hw_labels = []
        file = open(self.filename, "r")
        for line in file:
            parts=line.split(" ", 3)
            if parts[0] == "cell" or parts[0] == "cellt":
                break;
            elif (parts[0] == "set") and (len(parts) == 3) and (re.match("^[A-Z0-9_]+$", parts[1])):
                hw_labels.append(parts[1])
        file.close()
        return hw_labels

    #
    # Get the lowest cycle in the file
    #
    def getFirstCycle(self):
        # The simulator now leaves off the @pipe when in noperf mode
        re_real_cycle = re.compile("[:][ ]?([0-9]+)[@ ]")
        cycle = -1
        file = open(self.filename, "r")
        for line in file:
            parts=line.split(" ", 8)
            if (len(parts) >= 7) and (parts[0] == "cell" or parts[0] == "cellt"):
                real_cycle = re_real_cycle.search(line)
                if real_cycle:
                    partial_cycle = long(parts[5])
                    cycle = long(real_cycle.group(1))
                    self.cycle_offset = cycle - partial_cycle
                    break;
        file.close()
        return cycle

    #
    # Get the highest cycle in the file
    #
    def getLastCycle(self):
        cycle = -1
        file = open(self.filename, "r")
        file.seek(-2048, 2)
        for line in file:
            parts=line.split(" ", 7)
            if (len(parts) >= 6) and (parts[0] == "cell" or parts[0] == "cellt"):
                cycle = long(parts[5])
        file.close()
        return cycle + self.cycle_offset

    #
    # Return a file descriptor to begin loading from. It will start at
    # roughly cycle + SLOP_FACTOR
    #
    def jumpToCycle(self, cycle):
        self.current_cycle = cycle
        file = open(self.filename, "r")
        min = 0
        file.seek(0, 2)
        max = file.tell()
        while max - min > 100:
            loc = (min + max) / 2
            file.seek(loc)
            file.readline()
            cycle = -1
            while cycle == -1:
                parts = file.readline()
                if parts == "":
                    cycle = 0
                parts = parts.split(" ", 7)
                if parts[0] == "cell" or parts[0] == "cellt":
                    cycle = long(parts[5]) + self.cycle_offset
            if cycle == self.current_cycle - self.SLOP_FACTOR:
                break
            elif cycle > self.current_cycle - self.SLOP_FACTOR:
                max=loc
            else:
                min=file.tell()
        file.seek(loc)
        file.readline()
        return file

    #
    # Store the cycle information for a hardware unit
    #
    def addCycleInfo(self, hardware, color, start_cycle, stop_cycle, label, references):
        start_cycle -= self.current_cycle
        stop_cycle -= self.current_cycle

        if stop_cycle < 0:
            return
        if start_cycle >= self.view_cycles:
            return

        if label[-1] == "\n":
            label = label[0:-1]
        if label[-1] == "}":
            label = label[0:-1]
        label = label.strip()
        ref = []
        for r in references:
            if not (r[0:2] in ["pp", "pi", "re", "me", "df"]):
                ref.append(r)

        if start_cycle < 0:
            start_cycle = 0
        if stop_cycle >= self.view_cycles:
            stop_cycle = self.view_cycles - 1

        hw = self.hardware[hardware]
        if start_cycle == stop_cycle:
            hw.color[start_cycle] = color
            hw.label[start_cycle] = label
            hw.references[start_cycle] = ref
        else:
            for cycle in xrange(start_cycle, stop_cycle + 1):
                hw.color[cycle] = color
                hw.label[cycle] = label
                hw.references[cycle] = ref

    #
    # Read the data from the file, starting at the supplied cycle
    #
    def read(self, current_cycle):
        self.hardware = {}
        for hw_name in self.hw_labels:
            self.hardware[hw_name] = HardwareInfo(hw_name, self.view_cycles)

        file = self.jumpToCycle(current_cycle)
        max_cycle = self.current_cycle + self.view_cycles
        for line in file:
            try:
                line = line.replace("\r", "")
                if line == "\n" or line[0] == "#":
                    continue
                parts=line.split(" ", 8)
                if parts[0] == "cell":
                    cycle = long(parts[5]) + self.cycle_offset
                    if cycle >= self.current_cycle and cycle <= max_cycle:
                        hardware = parts[3][1:]
                        color = parts[4]
                        parts = parts[8].split("{", 1)
                        label = parts[1]
                        references = parts[0].split()[1:-1]
                        self.addCycleInfo(hardware, color, cycle, cycle, label, references)
                    elif cycle > max_cycle + self.SLOP_FACTOR:
                        break
                elif parts[0] == "}\n":
                    pass
                elif parts[0] == "cellt":
                    start_cycle = long(parts[4]) + self.cycle_offset
                    stop_cycle = long(parts[5]) + self.cycle_offset
                    if stop_cycle >= self.current_cycle and start_cycle <= max_cycle:
                        parts=line.split(" ", 8)
                        hardware = parts[2][1:]
                        color = parts[3]
                        parts = parts[8].split("{", 1)
                        label = parts[1]
                        references = parts[0].split()[1:-1]
                        self.addCycleInfo(hardware, color, start_cycle, stop_cycle, label, references)
                    elif start_cycle > max_cycle + self.SLOP_FACTOR:
                        break
                elif parts[0] == "set":
                    pass
                else:
                    raise "Invalid line: " + line
            except:
                pass
        file.close()

#
# If run directly just show all the hardware units found
#
if __name__ == "__main__":
    vz = Viewzilla()
    vz.view_cycles = vz.getLastCycle()  # Load everything
    vz.read(0)
    print vz.hardware.keys()
