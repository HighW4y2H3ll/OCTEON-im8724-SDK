#!/usr/bin/python
# Copyright (c) 2005, Cavium Inc.. All rights reserved.
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
# File version info: $Id: gdbstub.py 69916 2012-02-14 14:47:03Z bprakash $

#
# Octeon GDB stub communications wrapper
#
import sys
import os
import socket
import select

REGISTER_NAMES = [
    "zero",
    "at",
    "v0","v1",
    "a0","a1","a2","a3",
    "t0","t1","t2","t3","t4","t5","t6","t7",
    "s0","s1","s2","s3","s4","s5","s6","s7",
    "t8","t9",
    "k0","k1",
    "gp",
    "sp",
    "s8",
    "ra",
    "COP0_STATUS_REG",
    "mflo",
    "mfhi",
    "COP0_BADVADDR_REG",
    "COP0_CAUSE_REG",
    "pc",
    "COP0_DEBUG_REG",
    "COP0_MDEBUG_REG"]

#
# Convert a number to a hex string with at least "digits" in length. The
# builtin hex() in python does weird things
#
def toHex(v, digits):
    hex_digit = "0123456789abcdef"
    result = ""
    for digit in xrange(digits):
        bit_shift = (digits - digit - 1) * 4
        result += hex_digit[(v>>bit_shift) & 0xf]
    return result

#
# Convert a hex string into a number. The builtin long(x,16) in python does
# weird things
#
def fromHex(hex_str):
    if hex_str.startswith("0x"):
        hex_str = hex_str[2:]
    result = long(hex_str, 16)
    l = len(hex_str)
    if l == 16:
        if result >= 0x8000000000000000L:
            result -= 0x10000000000000000L
    elif l == 8:
        if result >= 0x80000000L:
            result -= 0x100000000L
    return result

#
# Class for controlling the Octeon through the debug stub
#
class GenericDebugConnection:
    #
    def __init__(self, gdb_logging = 0):
        self.gdb_logging = gdb_logging
        self.memcache = {}
        for retry in xrange(5):
            try:
                self.writeCommand("?")
                reply = self.readCommand()
                break
            except:
                reply = ""
        if reply != "S09":
            raise Exception("Illegal protocol version: " + reply + " (expected S09)")

    # Write a raw string to the debugger
    def writeString(self, data):
        raise Exception("Override with correct implementation")

    # Read a single character from the debugger
    def readChar(self):
        raise Exception("Override with correct implementation")

    # Calculate the checksum for a string to be sent to the debugger
    def calcChecksum(self, str):
        checksum = 0
        for c in str:
            checksum += ord(c)
        checksum &= 0xff
        return toHex(checksum, 2)

    # Write a complete command to the debugger. The leading $ and the
    # final checksum will be added
    def writeCommand(self, command):
        str = "$" + command + "#" + self.calcChecksum(command)
        reply = ""
        while reply != "+":
            if self.gdb_logging:
                print "Write: " + str
            self.writeString(str)
            reply = self.readString()

    # Read a complete string from the debugger. The result will either
    # be "+", "-", or "$xxxx#xx".
    def readString(self):
        while 1:
            c = self.readChar()
            if c in ["+", "-"]:
                return c

            while c != "$":
                c = self.readChar()

            str = c
            while c != "#":
                c = self.readChar()
                str += c
            crc = self.calcChecksum(str[1:-1])
            str += self.readChar()
            str += self.readChar()

            if self.gdb_logging:
                print "Read: " + str

            if str[-2:] != crc:
                self.writeString("-")
                raise Exception("Invalid CRC: " + str + " (correct " + crc + ")")
            self.writeString("+")
            if str.startswith("$!"):
                print "Message: " + str
            else:
                break
        return str

    # Read a command from the debugger and strip off the header and footer
    def readCommand(self):
        reply = self.readString()
        end = reply.find("#")
        return reply[1:end]

    # Read a command and validate it is the one we expected
    def expectCommand(self, cmd):
        reply = self.readCommand()
        if reply[0] == cmd:
            return reply
        else:
            raise Exception("Illegal reply: " + reply + " (expected " + cmd + ")")

    # Read a command, validate it, and convert its payload to a number
    def expectHex(self, cmd):
        return fromHex(self.expectCommand(cmd)[1:])

    # Interrupt a running program (Control-C)
    def interrupt(self):
        self.writeString("\003")

    # Read the value of a register
    def readRegister(self, register):
        if register in REGISTER_NAMES:
            register = REGISTER_NAMES.index(register)
        self.writeCommand("g" + toHex(register, 2))
        return fromHex(self.readCommand())

    # Read the value of all registers and return them in a dictionary
    def readRegisters(self):
        registers = {}
        self.writeCommand("q00")
        reply = self.readCommand()
        for r in REGISTER_NAMES:
            registers[r] = fromHex(reply[0:16])
            reply = reply[16:]
        self.memcache[registers["pc"]] = reply
        return registers

    # Write the contents of a register
    def writeRegister(self, register, value):
        if register in REGISTER_NAMES:
            register = REGISTER_NAMES.index(register)
        self.writeCommand("G" + toHex(register, 2) + "," + toHex(value, 16))

    # Read the value of memory and return it as hex
    def readMemory(self, address, size):
        if (address in self.memcache) and (len(self.memcache[address]) >= size * 2):
            return self.memcache[address][0:size*2]
        reply = ""
        # The stub can't handle large requests. We may need to break up the
        # request into a series
        addr = address
        while size > 0:
            # Limit request to at most 1000 bytes per
            if size > 1000:
                next = 1000
            else:
                next = size
            self.writeCommand("m" + toHex(addr, 16) + "," + toHex(next, 4))
            replycmd = self.readCommand()
            if len(replycmd) != next*2:
                print "Illegal response to memory read:", replycmd
                replycmd = ""
                for i in xrange((next+3)/4):
                    replycmd += "bad0deed"
                replycmd = replycmd[0:next*2]
            reply += replycmd
            size -= next
            addr += next
        self.memcache[address] = reply
        return reply

    # Write a hex block into memory
    def writeMemory(self, address, data):
        self.writeCommand("M" + toHex(address, 16) + "," + toHex(len(data)/2, 4) + ":" + data)
        self.memcache = {}

    # Get the current focus core
    def getFocus(self):
        self.writeCommand("f")
        return self.expectHex("F")

    # Set the current focus core
    def setFocus(self, core):
        self.writeCommand("F" + toHex(core, 2))
        self.memcache = {}

    # Get the list of active cores
    def getActiveCores(self):
        self.writeCommand("i")
        mask = self.expectHex("I")
        cores = []
        for i in xrange(16):
            if mask & (1 << i):
                cores.append(i)
        return cores

    # Set the list of active cores. An empty list sets it to all cores
    def setActiveCores(self, core_list = []):
        mask = 0
        for core in core_list:
            mask |= 1<<core
        self.writeCommand("I" + toHex(mask, 4))

    # Get the current step-all state
    def getStepAll(self):
        self.writeCommand("a")
        return self.expectHex("A") != 0

    # Set the step-all state
    def setStepAll(self, stepAll):
        if stepAll:
            self.writeCommand("A1")
        else:
            self.writeCommand("A0")

    # Set a hardware code breakpoint
    def setHardwareBreak(self, number, address):
        self.writeCommand("Z" + toHex(number, 2) + "," + toHex(address, 16))

    # Clear a hardware code breakpoint
    def clearHardwareBreak(self, number):
        self.writeCommand("z" + toHex(number, 2))

    # Single step one instruction
    def step(self):
        self.memcache = {}
        self.writeCommand("s")
        return self.readCommand()

    # Start running
    def run(self):
        self.memcache = {}
        self.writeCommand("c")

    # Return true if the debugger is stopped
    def isStopped(self):
        raise Exception("Override with correct implementation")

#
# Connect to the Debugger using a TCP connection to the simulator
#
class TCPDebugConnection(GenericDebugConnection):
    #
    def __init__(self, gdb_logging, host, port):
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.connect((host, port))
        self.sock.settimeout(5)
        GenericDebugConnection.__init__(self, gdb_logging)

    #
    def writeString(self, data):
        self.sock.send(data)

    #
    def readChar(self):
        return self.sock.recv(1)

    #
    def isStopped(self):
        result = select.select([self.sock.fileno()], [], [], 0)
        if result[0]:
            return 1
        else:
            return 0

#
# Connect to the Debugger using a Serial port
#
class SerialDebugConnection(GenericDebugConnection):
    #
    def __init__(self, gdb_logging, device, baud):
        print "SerialDebugConnection Warning: Setting the baud rate isn't supported yet"
        self.fileno = os.open(device, os.O_RDWR | os.O_SYNC | os.O_NDELAY)
        self.poll = select.poll()
        self.poll.register(self.fileno, select.POLLIN)
        self.timeout = 5
        GenericDebugConnection.__init__(self, gdb_logging)

    #
    def writeString(self, data):
        os.write(self.fileno, data)

    #
    def readChar(self):
        ready = self.poll.poll(self.timeout * 1000)
        if ready:
            result = os.read(self.fileno, 1)
        else:
            raise Exception("Timeout")
        return result

    #
    def isStopped(self):
        ready = self.poll.poll(0)
        if ready:
            return 1
        else:
            return 0

#
# get a connection to the debugger
#
def getDebugConnection(connect, gdb_logging = 0):
    parts = connect.split(":")
    if parts[0] in ["tcp", "TCP"]:
        if parts[1]:
            host = parts[1]
        else:
            host = "localhost"
        return TCPDebugConnection(gdb_logging, host, int(parts[2]))
    else:
        if len(parts) > 1:
            baud = int(parts[1])
        else:
            baud = 115200
        return SerialDebugConnection(gdb_logging, parts[0], baud)

#
# Trivial test
#
def test(connect):
    debug = getDebugConnection(connect, 1)
    debug.interrupt()
    print "Current focus:", debug.getFocus()
    print "Changing focus to 1"
    debug.setFocus(1)
    print "Current focus:", debug.getFocus()
    print "Active cores:", debug.getActiveCores()
    print "Changing active cores to [1]"
    debug.setActiveCores([1])
    print "Active cores:", debug.getActiveCores()
    print "Step all: ", debug.getStepAll()
    print "Enabling step all"
    debug.setStepAll(1)
    print "Step all: ", debug.getStepAll()
    print "Memory at 0x8000000000000000:", debug.readMemory(0x8000000000000000L, 16)
    print "Registers:", debug.readRegisters()
    print "Setting v0=", 0x1234
    #debug.writeRegister("v0",0x1234)
    print "Registers:", debug.readRegisters()
    debug.step()
    debug.run()

#
#
#
if __name__ == "__main__":
    test(sys.argv[1])
