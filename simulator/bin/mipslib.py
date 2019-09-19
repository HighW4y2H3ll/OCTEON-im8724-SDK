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
# File version info: $Id: mipslib.py 69916 2012-02-14 14:47:03Z bprakash $

#
# Mips utility library
#
import sys
from gdbstub import REGISTER_NAMES
from gdbstub import toHex
from gdbstub import fromHex

OPCODE_LIST = []

INSN_ISA1 = 0x00000001
INSN_ISA2 = 0x00000002
INSN_ISA3 = 0x00000004
INSN_ISA4 = 0x00000008
INSN_ISA5 = 0x00000010
INSN_ISA32 = 0x00000020
INSN_ISA64 = 0x00000040
INSN_ISA32R2 = 0x00000080
INSN_ISA64R2 = 0x00000100

# Masks used for MIPS-defined ASEs.
INSN_ASE_MASK = 0x0000f000

# MIPS 16 ASE
INSN_MIPS16 = 0x00002000
# MIPS-3D ASE
INSN_MIPS3D = 0x00004000
# MDMX ASE
INSN_MDMX = 0x00008000

# Chip specific instructions.  These are bitmasks.

# MIPS R4650 instruction.
INSN_4650 = 0x00010000
# LSI R4010 instruction.
INSN_4010 = 0x00020000
# NEC VR4100 instruction.
INSN_4100 = 0x00040000
# Toshiba R3900 instruction.
INSN_3900 = 0x00080000
# MIPS R10000 instruction.
INSN_10000 = 0x00100000
# Broadcom SB-1 instruction.
INSN_SB1 = 0x00200000
# NEC VR4111/VR4181 instruction.
INSN_4111 = 0x00400000
# NEC VR4120 instruction.
INSN_4120 = 0x00800000
# NEC VR5400 instruction.
INSN_5400 = 0x01000000
# NEC VR5500 instruction.
INSN_5500 = 0x02000000
# CAVIUM NETWORKS OCTEON instruction.
INSN_OCTEON = 0x04000000


I1 = INSN_ISA1
I2 = INSN_ISA2
I3 = INSN_ISA3
I4 = INSN_ISA4
I5 = INSN_ISA5
I32 = INSN_ISA32
I64 = INSN_ISA64
I33 = INSN_OCTEON|INSN_ISA32R2
I65 = INSN_OCTEON|INSN_ISA64R2

IOCTEON = INSN_OCTEON

# MIPS64 MIPS-3D ASE support.
I16 = INSN_MIPS16

# MIPS64 MIPS-3D ASE support.
M3D = INSN_MIPS3D

# MIPS64 MDMX ASE support.
MX = INSN_MDMX

P3 = INSN_4650
L1 = INSN_4010
V1 = INSN_4100 | INSN_4111 | INSN_4120
T3 = INSN_3900
M1 = INSN_10000
SB1 = INSN_SB1
N411 = INSN_4111
N412 = INSN_4120
N5 = INSN_5400 | INSN_5500
N54 = INSN_5400
N55 = INSN_5500

G1 = T3
G2 = T3
G3 = I4


OP_MASK_OP = 0x3f
OP_SH_OP = 26
OP_MASK_RS = 0x1f
OP_SH_RS = 21
OP_MASK_FR = 0x1f
OP_SH_FR = 21
OP_MASK_FMT = 0x1f
OP_SH_FMT = 21
OP_MASK_BCC = 0x7
OP_SH_BCC = 18
OP_MASK_CODE = 0x3ff
OP_SH_CODE = 16
OP_MASK_CODE2 = 0x3ff
OP_SH_CODE2 = 6
OP_MASK_RT = 0x1f
OP_SH_RT = 16
OP_MASK_FT = 0x1f
OP_SH_FT = 16
OP_MASK_CACHE = 0x1f
OP_SH_CACHE = 16
OP_MASK_RD = 0x1f
OP_SH_RD = 11
OP_MASK_FS = 0x1f
OP_SH_FS = 11
OP_MASK_PREFX = 0x1f
OP_SH_PREFX = 11
OP_MASK_CCC = 0x7
OP_SH_CCC = 8
OP_MASK_CODE20 = 0xfffff
OP_SH_CODE20 = 6
OP_MASK_SHAMT = 0x1f
OP_SH_SHAMT = 6
OP_MASK_BITIND = OP_MASK_RT
OP_SH_BITIND = OP_SH_RT
OP_MASK_FD = 0x1f
OP_SH_FD = 6
OP_MASK_TARGET = 0x3ffffff
OP_SH_TARGET = 0
OP_MASK_COPZ = 0x1ffffff
OP_SH_COPZ = 0
OP_MASK_IMMEDIATE = 0xffff
OP_SH_IMMEDIATE = 0
OP_MASK_DELTA = 0xffff
OP_SH_DELTA = 0
OP_MASK_FUNCT = 0x3f
OP_SH_FUNCT = 0
OP_MASK_SPEC = 0x3f
OP_SH_SPEC = 0
OP_SH_LOCC = 8
OP_SH_HICC = 18
OP_MASK_CC = 0x7
OP_SH_COP1NORM = 25
OP_MASK_COP1NORM = 0x1
OP_SH_COP1SPEC = 21
OP_MASK_COP1SPEC = 0xf
OP_MASK_COP1SCLR = 0x4
OP_MASK_COP1CMP = 0x3
OP_SH_COP1CMP = 4
OP_SH_FORMAT = 21
OP_MASK_FORMAT = 0x7
OP_SH_TRUE = 16
OP_MASK_TRUE = 0x1
OP_SH_GE = 17
OP_MASK_GE = 0x01
OP_SH_UNSIGNED = 16
OP_MASK_UNSIGNED = 0x1
OP_SH_HINT = 16
OP_MASK_HINT = 0x1f
OP_SH_MMI = 0
OP_MASK_MMI = 0x3f
OP_SH_MMISUB = 6
OP_MASK_MMISUB = 0x1f
OP_MASK_PERFREG = 0x1f
OP_SH_PERFREG = 1
OP_SH_SEL = 0
OP_MASK_SEL = 0x7
OP_SH_CODE19 = 6
OP_MASK_CODE19 = 0x7ffff
OP_SH_ALN = 21
OP_MASK_ALN = 0x7
OP_SH_VSEL = 21
OP_MASK_VSEL = 0x1f
OP_MASK_VECBYTE = 0x7
OP_SH_VECBYTE = 22
OP_MASK_VECALIGN = 0x7
OP_SH_VECALIGN = 21
OP_MASK_INSMSB = 0x1f
OP_SH_INSMSB = 11
OP_MASK_EXTMSBD = 0x1f
OP_SH_EXTMSBD = 11
OP_OP_COP0 = 0x10
OP_OP_COP1 = 0x11
OP_OP_COP2 = 0x12
OP_OP_COP3 = 0x13
OP_OP_LWC1 = 0x31
OP_OP_LWC2 = 0x32
OP_OP_LWC3 = 0x33
OP_OP_LDC1 = 0x35
OP_OP_LDC2 = 0x36
OP_OP_LDC3 = 0x37
OP_OP_SWC1 = 0x39
OP_OP_SWC2 = 0x3a
OP_OP_SWC3 = 0x3b
OP_OP_SDC1 = 0x3d
OP_OP_SDC2 = 0x3e
OP_OP_SDC3 = 0x3f
MDMX_FMTSEL_IMM_QH = 0x1d
MDMX_FMTSEL_IMM_OB = 0x1e
MDMX_FMTSEL_VEC_QH = 0x15
MDMX_FMTSEL_VEC_OB = 0x16























class Opcode:
    def __init__(self, name, args, opcode, mask, archs):
        self.name = name
        self.args = args
        self.opcode = opcode
        self.mask = mask
        self.archs = archs

def getBits(num, high, low):
    return (num >> low) & ((1<<(high-low+1)) - 1)

def addOpcode(name, args, opcode, mask, archs):
    OPCODE_LIST.append(Opcode(name, args, opcode, mask, archs))

def buildOpcodeTable():
    # The following table was copied from GDB
    addOpcode("pref",    "k,o(b)",   0xcc000000, 0xfc000000, I4|I32|G3)
    addOpcode("prefx",   "h,t(b)",  0x4c00000f, 0xfc0007ff, I4)
    addOpcode("nop",     "",         0x00000000, 0xffffffff, I1) # sll
    addOpcode("ssnop",   "",         0x00000040, 0xffffffff, I32|N55) # sll
    addOpcode("ehb",     "",         0x000000c0, 0xffffffff, I33) # sll
    addOpcode("li",      "t,j",      0x24000000, 0xffe00000, I1) # addiu
    addOpcode("li",     "t,i",  0x34000000, 0xffe00000, I1) # ori
    addOpcode("move",    "d,s", 0x0000002d, 0xfc1f07ff, I3)# daddu
    addOpcode("move",    "d,s", 0x00000021, 0xfc1f07ff, I1)# addu
    addOpcode("move",    "d,s", 0x00000025, 0xfc1f07ff, I1)# or
    addOpcode("b",       "p",   0x10000000, 0xffff0000, I1)# beq 0,0
    addOpcode("b",       "p",   0x04010000, 0xffff0000, I1)# bgez 0
    addOpcode("bal",     "p",   0x04110000, 0xffff0000, I1)# bgezal 0

    addOpcode("abs.s",   "D,V", 0x46000005, 0xffff003f, I1)
    addOpcode("abs.d",   "D,V", 0x46200005, 0xffff003f, I1)
    addOpcode("abs.ps",  "D,V", 0x46c00005, 0xffff003f, I5)
    addOpcode("add",     "d,v,t",   0x00000020, 0xfc0007ff, I1)
    addOpcode("add.s",   "D,V,T",   0x46000000, 0xffe0003f, I1)
    addOpcode("add.d",   "D,V,T",   0x46200000, 0xffe0003f, I1)
    addOpcode("add.ob",  "X,Y,Q",   0x7800000b, 0xfc20003f, MX|SB1)
    addOpcode("add.ob",  "D,S,T",   0x4ac0000b, 0xffe0003f, N54)
    addOpcode("add.ob",  "D,S,T[e]",    0x4800000b, 0xfe20003f, N54)
    addOpcode("add.ob",  "D,S,k",   0x4bc0000b, 0xffe0003f, N54)
    addOpcode("add.ps",  "D,V,T",   0x46c00000, 0xffe0003f, I5)
    addOpcode("add.qh",  "X,Y,Q",   0x7820000b, 0xfc20003f, MX)
    addOpcode("adda.ob", "Y,Q", 0x78000037, 0xfc2007ff, MX|SB1)
    addOpcode("adda.qh", "Y,Q", 0x78200037, 0xfc2007ff, MX)
    addOpcode("addi",    "t,r,j",   0x20000000, 0xfc000000, I1)
    addOpcode("addiu",   "t,r,j",   0x24000000, 0xfc000000, I1)
    addOpcode("addl.ob", "Y,Q", 0x78000437, 0xfc2007ff, MX|SB1)
    addOpcode("addl.qh", "Y,Q", 0x78200437, 0xfc2007ff, MX)
    addOpcode("addr.ps", "D,S,T",   0x46c00018, 0xffe0003f, M3D)
    addOpcode("addu",    "d,v,t",   0x00000021, 0xfc0007ff, I1)
    addOpcode("alni.ob", "X,Y,Z,O", 0x78000018, 0xff00003f, MX|SB1)
    addOpcode("alni.ob", "D,S,T,%", 0x48000018, 0xff00003f, N54)
    addOpcode("alni.qh", "X,Y,Z,O", 0x7800001a, 0xff00003f, MX)
    addOpcode("alnv.ps", "D,V,T,s", 0x4c00001e, 0xfc00003f, I5)
    addOpcode("alnv.ob", "X,Y,Z,s", 0x78000019, 0xfc00003f, MX|SB1)
    addOpcode("alnv.qh", "X,Y,Z,s", 0x7800001b, 0xfc00003f, MX)
    addOpcode("and",     "d,v,t",   0x00000024, 0xfc0007ff, I1)
    addOpcode("and.ob",  "X,Y,Q",   0x7800000c, 0xfc20003f, MX|SB1)
    addOpcode("and.ob",  "D,S,T",   0x4ac0000c, 0xffe0003f, N54)
    addOpcode("and.ob",  "D,S,T[e]",    0x4800000c, 0xfe20003f, N54)
    addOpcode("and.ob",  "D,S,k",   0x4bc0000c, 0xffe0003f, N54)
    addOpcode("and.qh",  "X,Y,Q",   0x7820000c, 0xfc20003f, MX)
    addOpcode("andi",    "t,r,i",   0x30000000, 0xfc000000, I1)
    # b is at the top of the table.
    # bal is at the top of the table.
    addOpcode("baddu",   "d,v,t",    0x70000028, 0xfc0007ff, IOCTEON)
    addOpcode("bbit032", "s,^,p",    0xd8000000, 0xfc000000, IOCTEON)
    addOpcode("bbit0",   "s,&,p",    0xd8000000, 0xfc000000, IOCTEON) # bbit032
    addOpcode("bbit0",   "s,^,p",    0xc8000000, 0xfc000000, IOCTEON)
    addOpcode("bbit132", "s,^,p",    0xf8000000, 0xfc000000, IOCTEON)
    addOpcode("bbit1",   "s,&,p",    0xf8000000, 0xfc000000, IOCTEON) # bbit132
    addOpcode("bbit1",   "s,^,p",    0xe8000000, 0xfc000000, IOCTEON)
    addOpcode("bc0f",    "p",   0x41000000, 0xffff0000, I1)
    addOpcode("bc0fl",   "p",   0x41020000, 0xffff0000, I2|T3)
    addOpcode("bc0t",    "p",   0x41010000, 0xffff0000, I1)
    addOpcode("bc0tl",   "p",   0x41030000, 0xffff0000, I2|T3)
    addOpcode("bc1any2f", "N,p",    0x45200000, 0xffe30000, M3D)
    addOpcode("bc1any2t", "N,p",    0x45210000, 0xffe30000, M3D)
    addOpcode("bc1any4f", "N,p",    0x45400000, 0xffe30000, M3D)
    addOpcode("bc1any4t", "N,p",    0x45410000, 0xffe30000, M3D)
    addOpcode("bc1f",    "p",   0x45000000, 0xffff0000, I1)
    addOpcode("bc1f",    "N,p",      0x45000000, 0xffe30000, I4|I32)
    addOpcode("bc1fl",   "p",   0x45020000, 0xffff0000, I2|T3)
    addOpcode("bc1fl",   "N,p",      0x45020000, 0xffe30000, I4|I32)
    addOpcode("bc1t",    "p",   0x45010000, 0xffff0000, I1)
    addOpcode("bc1t",    "N,p",      0x45010000, 0xffe30000, I4|I32)
    addOpcode("bc1tl",   "p",   0x45030000, 0xffff0000, I2|T3)
    addOpcode("bc1tl",   "N,p",      0x45030000, 0xffe30000, I4|I32)
    addOpcode("bc2f",    "p",   0x49000000, 0xffff0000, I1)
    addOpcode("bc2fl",   "p",   0x49020000, 0xffff0000, I2|T3)
    addOpcode("bc2t",    "p",   0x49010000, 0xffff0000, I1)
    addOpcode("bc2tl",   "p",   0x49030000, 0xffff0000, I2|T3)
    addOpcode("bc3f",    "p",   0x4d000000, 0xffff0000, I1)
    addOpcode("bc3fl",   "p",   0x4d020000, 0xffff0000, I2|T3)
    addOpcode("bc3t",    "p",   0x4d010000, 0xffff0000, I1)
    addOpcode("bc3tl",   "p",   0x4d030000, 0xffff0000, I2|T3)
    addOpcode("beqz",    "s,p", 0x10000000, 0xfc1f0000, I1)
    addOpcode("beqzl",   "s,p", 0x50000000, 0xfc1f0000, I2|T3)
    addOpcode("beq",     "s,t,p",   0x10000000, 0xfc000000, I1)
    addOpcode("beql",    "s,t,p",   0x50000000, 0xfc000000, I2|T3)
    addOpcode("bgez",    "s,p", 0x04010000, 0xfc1f0000, I1)
    addOpcode("bgezl",   "s,p", 0x04030000, 0xfc1f0000, I2|T3)
    addOpcode("bgezal",  "s,p", 0x04110000, 0xfc1f0000, I1)
    addOpcode("bgezall", "s,p", 0x04130000, 0xfc1f0000, I2|T3)
    addOpcode("bgtz",    "s,p", 0x1c000000, 0xfc1f0000, I1)
    addOpcode("bgtzl",   "s,p", 0x5c000000, 0xfc1f0000, I2|T3)
    addOpcode("blez",    "s,p", 0x18000000, 0xfc1f0000, I1)
    addOpcode("blezl",   "s,p", 0x58000000, 0xfc1f0000, I2|T3)
    addOpcode("bltz",    "s,p", 0x04000000, 0xfc1f0000, I1)
    addOpcode("bltzl",   "s,p", 0x04020000, 0xfc1f0000, I2|T3)
    addOpcode("bltzal",  "s,p", 0x04100000, 0xfc1f0000, I1)
    addOpcode("bltzall", "s,p", 0x04120000, 0xfc1f0000, I2|T3)
    addOpcode("bnez",    "s,p", 0x14000000, 0xfc1f0000, I1)
    addOpcode("bnezl",   "s,p", 0x54000000, 0xfc1f0000, I2|T3)
    addOpcode("bne",     "s,t,p",   0x14000000, 0xfc000000, I1)
    addOpcode("bnel",    "s,t,p",   0x54000000, 0xfc000000, I2|T3)
    addOpcode("break",   "",        0x0000000d, 0xffffffff, I1)
    addOpcode("break",   "B",        0x0000000d, 0xfc00003f, I32)
    addOpcode("break",   "c",   0x0000000d, 0xfc00ffff, I1)
    addOpcode("break",   "c,q", 0x0000000d, 0xfc00003f, I1)
    addOpcode("c.f.d",   "S,T", 0x46200030, 0xffe007ff, I1)
    addOpcode("c.f.d",   "M,S,T",    0x46200030, 0xffe000ff, I4|I32)
    addOpcode("c.f.s",   "S,T",      0x46000030, 0xffe007ff, I1)
    addOpcode("c.f.s",   "M,S,T",    0x46000030, 0xffe000ff, I4|I32)
    addOpcode("c.f.ps",  "S,T", 0x46c00030, 0xffe007ff, I5)
    addOpcode("c.f.ps",  "M,S,T",   0x46c00030, 0xffe000ff, I5)
    addOpcode("c.un.d",  "S,T", 0x46200031, 0xffe007ff, I1)
    addOpcode("c.un.d",  "M,S,T",    0x46200031, 0xffe000ff, I4|I32)
    addOpcode("c.un.s",  "S,T",      0x46000031, 0xffe007ff, I1)
    addOpcode("c.un.s",  "M,S,T",    0x46000031, 0xffe000ff, I4|I32)
    addOpcode("c.un.ps", "S,T", 0x46c00031, 0xffe007ff, I5)
    addOpcode("c.un.ps", "M,S,T",   0x46c00031, 0xffe000ff, I5)
    addOpcode("c.eq.d",  "S,T", 0x46200032, 0xffe007ff, I1)
    addOpcode("c.eq.d",  "M,S,T",    0x46200032, 0xffe000ff, I4|I32)
    addOpcode("c.eq.s",  "S,T",      0x46000032, 0xffe007ff, I1)
    addOpcode("c.eq.s",  "M,S,T",    0x46000032, 0xffe000ff, I4|I32)
    addOpcode("c.eq.ob", "Y,Q", 0x78000001, 0xfc2007ff, MX|SB1)
    addOpcode("c.eq.ob", "S,T", 0x4ac00001, 0xffe007ff, N54)
    addOpcode("c.eq.ob", "S,T[e]",  0x48000001, 0xfe2007ff, N54)
    addOpcode("c.eq.ob", "S,k", 0x4bc00001, 0xffe007ff, N54)
    addOpcode("c.eq.ps", "S,T", 0x46c00032, 0xffe007ff, I5)
    addOpcode("c.eq.ps", "M,S,T",   0x46c00032, 0xffe000ff, I5)
    addOpcode("c.eq.qh", "Y,Q", 0x78200001, 0xfc2007ff, MX)
    addOpcode("c.ueq.d", "S,T", 0x46200033, 0xffe007ff, I1)
    addOpcode("c.ueq.d", "M,S,T",    0x46200033, 0xffe000ff, I4|I32)
    addOpcode("c.ueq.s", "S,T",      0x46000033, 0xffe007ff, I1)
    addOpcode("c.ueq.s", "M,S,T",    0x46000033, 0xffe000ff, I4|I32)
    addOpcode("c.ueq.ps","S,T", 0x46c00033, 0xffe007ff, I5)
    addOpcode("c.ueq.ps","M,S,T",   0x46c00033, 0xffe000ff, I5)
    addOpcode("c.olt.d", "S,T",      0x46200034, 0xffe007ff, I1)
    addOpcode("c.olt.d", "M,S,T",    0x46200034, 0xffe000ff, I4|I32)
    addOpcode("c.olt.s", "S,T", 0x46000034, 0xffe007ff, I1)
    addOpcode("c.olt.s", "M,S,T",    0x46000034, 0xffe000ff, I4|I32)
    addOpcode("c.olt.ps","S,T", 0x46c00034, 0xffe007ff, I5)
    addOpcode("c.olt.ps","M,S,T",   0x46c00034, 0xffe000ff, I5)
    addOpcode("c.ult.d", "S,T", 0x46200035, 0xffe007ff, I1)
    addOpcode("c.ult.d", "M,S,T",    0x46200035, 0xffe000ff, I4|I32)
    addOpcode("c.ult.s", "S,T",      0x46000035, 0xffe007ff, I1)
    addOpcode("c.ult.s", "M,S,T",    0x46000035, 0xffe000ff, I4|I32)
    addOpcode("c.ult.ps","S,T", 0x46c00035, 0xffe007ff, I5)
    addOpcode("c.ult.ps","M,S,T",   0x46c00035, 0xffe000ff, I5)
    addOpcode("c.ole.d", "S,T",      0x46200036, 0xffe007ff, I1)
    addOpcode("c.ole.d", "M,S,T",    0x46200036, 0xffe000ff, I4|I32)
    addOpcode("c.ole.s", "S,T",      0x46000036, 0xffe007ff, I1)
    addOpcode("c.ole.s", "M,S,T",    0x46000036, 0xffe000ff, I4|I32)
    addOpcode("c.ole.ps","S,T", 0x46c00036, 0xffe007ff, I5)
    addOpcode("c.ole.ps","M,S,T",   0x46c00036, 0xffe000ff, I5)
    addOpcode("c.ule.d", "S,T", 0x46200037, 0xffe007ff, I1)
    addOpcode("c.ule.d", "M,S,T",    0x46200037, 0xffe000ff, I4|I32)
    addOpcode("c.ule.s", "S,T",      0x46000037, 0xffe007ff, I1)
    addOpcode("c.ule.s", "M,S,T",    0x46000037, 0xffe000ff, I4|I32)
    addOpcode("c.ule.ps","S,T", 0x46c00037, 0xffe007ff, I5)
    addOpcode("c.ule.ps","M,S,T",   0x46c00037, 0xffe000ff, I5)
    addOpcode("c.sf.d",  "S,T", 0x46200038, 0xffe007ff, I1)
    addOpcode("c.sf.d",  "M,S,T",    0x46200038, 0xffe000ff, I4|I32)
    addOpcode("c.sf.s",  "S,T",      0x46000038, 0xffe007ff, I1)
    addOpcode("c.sf.s",  "M,S,T",    0x46000038, 0xffe000ff, I4|I32)
    addOpcode("c.sf.ps", "S,T", 0x46c00038, 0xffe007ff, I5)
    addOpcode("c.sf.ps", "M,S,T",   0x46c00038, 0xffe000ff, I5)
    addOpcode("c.ngle.d","S,T", 0x46200039, 0xffe007ff, I1)
    addOpcode("c.ngle.d","M,S,T",    0x46200039, 0xffe000ff, I4|I32)
    addOpcode("c.ngle.s","S,T",      0x46000039, 0xffe007ff, I1)
    addOpcode("c.ngle.s","M,S,T",    0x46000039, 0xffe000ff, I4|I32)
    addOpcode("c.ngle.ps","S,T",    0x46c00039, 0xffe007ff, I5)
    addOpcode("c.ngle.ps","M,S,T",  0x46c00039, 0xffe000ff, I5)
    addOpcode("c.seq.d", "S,T", 0x4620003a, 0xffe007ff, I1)
    addOpcode("c.seq.d", "M,S,T",    0x4620003a, 0xffe000ff, I4|I32)
    addOpcode("c.seq.s", "S,T",      0x4600003a, 0xffe007ff, I1)
    addOpcode("c.seq.s", "M,S,T",    0x4600003a, 0xffe000ff, I4|I32)
    addOpcode("c.seq.ps","S,T", 0x46c0003a, 0xffe007ff, I5)
    addOpcode("c.seq.ps","M,S,T",   0x46c0003a, 0xffe000ff, I5)
    addOpcode("c.ngl.d", "S,T", 0x4620003b, 0xffe007ff, I1)
    addOpcode("c.ngl.d", "M,S,T",    0x4620003b, 0xffe000ff, I4|I32)
    addOpcode("c.ngl.s", "S,T",      0x4600003b, 0xffe007ff, I1)
    addOpcode("c.ngl.s", "M,S,T",    0x4600003b, 0xffe000ff, I4|I32)
    addOpcode("c.ngl.ps","S,T", 0x46c0003b, 0xffe007ff, I5)
    addOpcode("c.ngl.ps","M,S,T",   0x46c0003b, 0xffe000ff, I5)
    addOpcode("c.lt.d",  "S,T", 0x4620003c, 0xffe007ff, I1)
    addOpcode("c.lt.d",  "M,S,T",    0x4620003c, 0xffe000ff, I4|I32)
    addOpcode("c.lt.s",  "S,T", 0x4600003c, 0xffe007ff, I1)
    addOpcode("c.lt.s",  "M,S,T",    0x4600003c, 0xffe000ff, I4|I32)
    addOpcode("c.lt.ob", "Y,Q", 0x78000004, 0xfc2007ff, MX|SB1)
    addOpcode("c.lt.ob", "S,T", 0x4ac00004, 0xffe007ff, N54)
    addOpcode("c.lt.ob", "S,T[e]",  0x48000004, 0xfe2007ff, N54)
    addOpcode("c.lt.ob", "S,k", 0x4bc00004, 0xffe007ff, N54)
    addOpcode("c.lt.ps", "S,T", 0x46c0003c, 0xffe007ff, I5)
    addOpcode("c.lt.ps", "M,S,T",   0x46c0003c, 0xffe000ff, I5)
    addOpcode("c.lt.qh", "Y,Q", 0x78200004, 0xfc2007ff, MX)
    addOpcode("c.nge.d", "S,T", 0x4620003d, 0xffe007ff, I1)
    addOpcode("c.nge.d", "M,S,T",    0x4620003d, 0xffe000ff, I4|I32)
    addOpcode("c.nge.s", "S,T",      0x4600003d, 0xffe007ff, I1)
    addOpcode("c.nge.s", "M,S,T",    0x4600003d, 0xffe000ff, I4|I32)
    addOpcode("c.nge.ps","S,T", 0x46c0003d, 0xffe007ff, I5)
    addOpcode("c.nge.ps","M,S,T",   0x46c0003d, 0xffe000ff, I5)
    addOpcode("c.le.d",  "S,T", 0x4620003e, 0xffe007ff, I1)
    addOpcode("c.le.d",  "M,S,T",    0x4620003e, 0xffe000ff, I4|I32)
    addOpcode("c.le.s",  "S,T", 0x4600003e, 0xffe007ff, I1)
    addOpcode("c.le.s",  "M,S,T",    0x4600003e, 0xffe000ff, I4|I32)
    addOpcode("c.le.ob", "Y,Q", 0x78000005, 0xfc2007ff, MX|SB1)
    addOpcode("c.le.ob", "S,T", 0x4ac00005, 0xffe007ff, N54)
    addOpcode("c.le.ob", "S,T[e]",  0x48000005, 0xfe2007ff, N54)
    addOpcode("c.le.ob", "S,k", 0x4bc00005, 0xffe007ff, N54)
    addOpcode("c.le.ps", "S,T", 0x46c0003e, 0xffe007ff, I5)
    addOpcode("c.le.ps", "M,S,T",   0x46c0003e, 0xffe000ff, I5)
    addOpcode("c.le.qh", "Y,Q", 0x78200005, 0xfc2007ff, MX)
    addOpcode("c.ngt.d", "S,T", 0x4620003f, 0xffe007ff, I1)
    addOpcode("c.ngt.d", "M,S,T",    0x4620003f, 0xffe000ff, I4|I32)
    addOpcode("c.ngt.s", "S,T",      0x4600003f, 0xffe007ff, I1)
    addOpcode("c.ngt.s", "M,S,T",    0x4600003f, 0xffe000ff, I4|I32)
    addOpcode("c.ngt.ps","S,T", 0x46c0003f, 0xffe007ff, I5)
    addOpcode("c.ngt.ps","M,S,T",   0x46c0003f, 0xffe000ff, I5)
    addOpcode("cabs.eq.d",  "M,S,T",    0x46200072, 0xffe000ff, M3D)
    addOpcode("cabs.eq.ps", "M,S,T",    0x46c00072, 0xffe000ff, M3D)
    addOpcode("cabs.eq.s",  "M,S,T",    0x46000072, 0xffe000ff, M3D)
    addOpcode("cabs.f.d",   "M,S,T",    0x46200070, 0xffe000ff, M3D)
    addOpcode("cabs.f.ps",  "M,S,T",    0x46c00070, 0xffe000ff, M3D)
    addOpcode("cabs.f.s",   "M,S,T",    0x46000070, 0xffe000ff, M3D)
    addOpcode("cabs.le.d",  "M,S,T",    0x4620007e, 0xffe000ff, M3D)
    addOpcode("cabs.le.ps", "M,S,T",    0x46c0007e, 0xffe000ff, M3D)
    addOpcode("cabs.le.s",  "M,S,T",    0x4600007e, 0xffe000ff, M3D)
    addOpcode("cabs.lt.d",  "M,S,T",    0x4620007c, 0xffe000ff, M3D)
    addOpcode("cabs.lt.ps", "M,S,T",    0x46c0007c, 0xffe000ff, M3D)
    addOpcode("cabs.lt.s",  "M,S,T",    0x4600007c, 0xffe000ff, M3D)
    addOpcode("cabs.nge.d", "M,S,T",    0x4620007d, 0xffe000ff, M3D)
    addOpcode("cabs.nge.ps","M,S,T",    0x46c0007d, 0xffe000ff, M3D)
    addOpcode("cabs.nge.s", "M,S,T",    0x4600007d, 0xffe000ff, M3D)
    addOpcode("cabs.ngl.d", "M,S,T",    0x4620007b, 0xffe000ff, M3D)
    addOpcode("cabs.ngl.ps","M,S,T",    0x46c0007b, 0xffe000ff, M3D)
    addOpcode("cabs.ngl.s", "M,S,T",    0x4600007b, 0xffe000ff, M3D)
    addOpcode("cabs.ngle.d","M,S,T",    0x46200079, 0xffe000ff, M3D)
    addOpcode("cabs.ngle.ps","M,S,T",0x46c00079, 0xffe000ff,    M3D)
    addOpcode("cabs.ngle.s","M,S,T",    0x46000079, 0xffe000ff, M3D)
    addOpcode("cabs.ngt.d", "M,S,T",    0x4620007f, 0xffe000ff, M3D)
    addOpcode("cabs.ngt.ps","M,S,T",    0x46c0007f, 0xffe000ff, M3D)
    addOpcode("cabs.ngt.s", "M,S,T",    0x4600007f, 0xffe000ff, M3D)
    addOpcode("cabs.ole.d", "M,S,T",    0x46200076, 0xffe000ff, M3D)
    addOpcode("cabs.ole.ps","M,S,T",    0x46c00076, 0xffe000ff, M3D)
    addOpcode("cabs.ole.s", "M,S,T",    0x46000076, 0xffe000ff, M3D)
    addOpcode("cabs.olt.d", "M,S,T",    0x46200074, 0xffe000ff, M3D)
    addOpcode("cabs.olt.ps","M,S,T",    0x46c00074, 0xffe000ff, M3D)
    addOpcode("cabs.olt.s", "M,S,T",    0x46000074, 0xffe000ff, M3D)
    addOpcode("cabs.seq.d", "M,S,T",    0x4620007a, 0xffe000ff, M3D)
    addOpcode("cabs.seq.ps","M,S,T",    0x46c0007a, 0xffe000ff, M3D)
    addOpcode("cabs.seq.s", "M,S,T",    0x4600007a, 0xffe000ff, M3D)
    addOpcode("cabs.sf.d",  "M,S,T",    0x46200078, 0xffe000ff, M3D)
    addOpcode("cabs.sf.ps", "M,S,T",    0x46c00078, 0xffe000ff, M3D)
    addOpcode("cabs.sf.s",  "M,S,T",    0x46000078, 0xffe000ff, M3D)
    addOpcode("cabs.ueq.d", "M,S,T",    0x46200073, 0xffe000ff, M3D)
    addOpcode("cabs.ueq.ps","M,S,T",    0x46c00073, 0xffe000ff, M3D)
    addOpcode("cabs.ueq.s", "M,S,T",    0x46000073, 0xffe000ff, M3D)
    addOpcode("cabs.ule.d", "M,S,T",    0x46200077, 0xffe000ff, M3D)
    addOpcode("cabs.ule.ps","M,S,T",    0x46c00077, 0xffe000ff, M3D)
    addOpcode("cabs.ule.s", "M,S,T",    0x46000077, 0xffe000ff, M3D)
    addOpcode("cabs.ult.d", "M,S,T",    0x46200075, 0xffe000ff, M3D)
    addOpcode("cabs.ult.ps","M,S,T",    0x46c00075, 0xffe000ff, M3D)
    addOpcode("cabs.ult.s", "M,S,T",    0x46000075, 0xffe000ff, M3D)
    addOpcode("cabs.un.d",  "M,S,T",    0x46200071, 0xffe000ff, M3D)
    addOpcode("cabs.un.ps", "M,S,T",    0x46c00071, 0xffe000ff, M3D)
    addOpcode("cabs.un.s",  "M,S,T",    0x46000071, 0xffe000ff, M3D)
    addOpcode("cache",   "k,o(b)",   0xbc000000, 0xfc000000, I3|I32|T3)
    addOpcode("ceil.l.d", "D,S",    0x4620000a, 0xffff003f, I3)
    addOpcode("ceil.l.s", "D,S",    0x4600000a, 0xffff003f, I3)
    addOpcode("ceil.w.d", "D,S",    0x4620000e, 0xffff003f, I2)
    addOpcode("ceil.w.s", "D,S",    0x4600000e, 0xffff003f, I2)
    addOpcode("cfc0",    "t,G", 0x40400000, 0xffe007ff, I1)
    addOpcode("cfc1",    "t,G", 0x44400000, 0xffe007ff, I1)
    addOpcode("cfc1",    "t,S", 0x44400000, 0xffe007ff, I1)
    addOpcode("cfc2",    "t,G", 0x48400000, 0xffe007ff, I1)
    addOpcode("cfc3",    "t,G", 0x4c400000, 0xffe007ff, I1)
    addOpcode("cins32",  "t,r,<,h",  0x70000033, 0xfc00003f, IOCTEON)
    addOpcode("cins",    "t,r,>,h",  0x70000033, 0xfc00003f, IOCTEON) # cins32
    addOpcode("cins",    "t,r,<,h",  0x70000032, 0xfc00003f, IOCTEON)
    addOpcode("clo",     "U,s",      0x70000021, 0xfc0007ff, I32|N55)
    addOpcode("clz",     "U,s",      0x70000020, 0xfc0007ff, I32|N55)
    addOpcode("ctc0",    "t,G", 0x40c00000, 0xffe007ff, I1)
    addOpcode("ctc1",    "t,G", 0x44c00000, 0xffe007ff, I1)
    addOpcode("ctc1",    "t,S", 0x44c00000, 0xffe007ff, I1)
    addOpcode("ctc2",    "t,G", 0x48c00000, 0xffe007ff, I1)
    addOpcode("ctc3",    "t,G", 0x4cc00000, 0xffe007ff, I1)
    addOpcode("cvt.d.l", "D,S", 0x46a00021, 0xffff003f, I3)
    addOpcode("cvt.d.s", "D,S", 0x46000021, 0xffff003f, I1)
    addOpcode("cvt.d.w", "D,S", 0x46800021, 0xffff003f, I1)
    addOpcode("cvt.l.d", "D,S", 0x46200025, 0xffff003f, I3)
    addOpcode("cvt.l.s", "D,S", 0x46000025, 0xffff003f, I3)
    addOpcode("cvt.s.l", "D,S", 0x46a00020, 0xffff003f, I3)
    addOpcode("cvt.s.d", "D,S", 0x46200020, 0xffff003f, I1)
    addOpcode("cvt.s.w", "D,S", 0x46800020, 0xffff003f, I1)
    addOpcode("cvt.s.pl","D,S", 0x46c00028, 0xffff003f, I5)
    addOpcode("cvt.s.pu","D,S", 0x46c00020, 0xffff003f, I5)
    addOpcode("cvt.w.d", "D,S", 0x46200024, 0xffff003f, I1)
    addOpcode("cvt.w.s", "D,S", 0x46000024, 0xffff003f, I1)
    addOpcode("cvt.ps.pw", "D,S",   0x46800026, 0xffff003f, M3D)
    addOpcode("cvt.ps.s","D,V,T",   0x46000026, 0xffe0003f, I5)
    addOpcode("cvt.pw.ps", "D,S",   0x46c00024, 0xffff003f, M3D)
    addOpcode("dadd",    "d,v,t",   0x0000002c, 0xfc0007ff, I3)
    addOpcode("daddi",   "t,r,j",   0x60000000, 0xfc000000, I3)
    addOpcode("daddiu",  "t,r,j",   0x64000000, 0xfc000000, I3)
    addOpcode("daddu",   "d,v,t",   0x0000002d, 0xfc0007ff, I3)
    addOpcode("dbreak",  "",        0x7000003f, 0xffffffff, N5)
    addOpcode("dclo",    "U,s",      0x70000025, 0xfc0007ff, I64|N55)
    addOpcode("dclz",    "U,s",      0x70000024, 0xfc0007ff, I64|N55)
    # dctr and dctw are used on the r5000.
    addOpcode("dctr",    "o(b)",    0xbc050000, 0xfc1f0000, I3)
    addOpcode("dctw",    "o(b)",    0xbc090000, 0xfc1f0000, I3)
    addOpcode("deret",   "",         0x4200001f, 0xffffffff, I32|G2)
    addOpcode("dext",    "t,r,+A,+C", 0x7c000003, 0xfc00003f, I65)
    addOpcode("dextm",   "t,r,+A,+G", 0x7c000001, 0xfc00003f, I65)
    addOpcode("dextu",   "t,r,+E,+H", 0x7c000002, 0xfc00003f, I65)
    # For ddiv, see the comments about div.
    addOpcode("ddiv",    "z,s,t",    0x0000001e, 0xfc00ffff, I3)
    # For ddivu, see the comments about div.
    addOpcode("ddivu",   "z,s,t",    0x0000001f, 0xfc00ffff, I3)
    addOpcode("di",      "",        0x41606000, 0xffffffff, I33)
    addOpcode("di",      "t",   0x41606000, 0xffe0ffff, I33)
    addOpcode("dins",    "t,r,+A,+B", 0x7c000007, 0xfc00003f, I65)
    addOpcode("dinsm",   "t,r,+A,+F", 0x7c000005, 0xfc00003f, I65)
    addOpcode("dinsu",   "t,r,+E,+F", 0x7c000006, 0xfc00003f, I65)
    # The MIPS assembler treats the div opcode with two operands as
    # though the first operand appeared twice (the first operand is both
    # a source and a destination).  To get the div machine instruction,
    # you must use an explicit destination of $0.
    addOpcode("div",     "z,s,t",    0x0000001a, 0xfc00ffff, I1)
    addOpcode("div",     "z,t",      0x0000001a, 0xffe0ffff, I1)
    addOpcode("div.d",   "D,V,T",   0x46200003, 0xffe0003f, I1)
    addOpcode("div.s",   "D,V,T",   0x46000003, 0xffe0003f, I1)
    addOpcode("div.ps",  "D,V,T",   0x46c00003, 0xffe0003f, SB1)
    # For divu, see the comments about div.
    addOpcode("divu",    "z,s,t",    0x0000001b, 0xfc00ffff, I1)
    addOpcode("divu",    "z,t",      0x0000001b, 0xffe0ffff, I1)
    addOpcode("dli",     "t,j",      0x24000000, 0xffe00000, I3) # addiu
    addOpcode("dli",        "t,i",  0x34000000, 0xffe00000, I3) # ori
    addOpcode("dmacc",   "d,s,t",   0x00000029, 0xfc0007ff, N412)
    addOpcode("dmacchi", "d,s,t",   0x00000229, 0xfc0007ff, N412)
    addOpcode("dmacchis", "d,s,t",  0x00000629, 0xfc0007ff, N412)
    addOpcode("dmacchiu", "d,s,t",  0x00000269, 0xfc0007ff, N412)
    addOpcode("dmacchius", "d,s,t", 0x00000669, 0xfc0007ff, N412)
    addOpcode("dmaccs",  "d,s,t",   0x00000429, 0xfc0007ff, N412)
    addOpcode("dmaccu",  "d,s,t",   0x00000069, 0xfc0007ff, N412)
    addOpcode("dmaccus", "d,s,t",   0x00000469, 0xfc0007ff, N412)
    addOpcode("dmadd16", "s,t",      0x00000029, 0xfc00ffff, N411)
    addOpcode("dmfc0",   "t,G", 0x40200000, 0xffe007ff, I3)
    addOpcode("dmfc0",   "t,+D",     0x40200000, 0xffe007f8, I64)
    addOpcode("dmfc0",   "t,G,H",    0x40200000, 0xffe007f8, I64)
    addOpcode("dmtc0",   "t,G", 0x40a00000, 0xffe007ff, I3)
    addOpcode("dmtc0",   "t,+D",     0x40a00000, 0xffe007f8, I64)
    addOpcode("dmtc0",   "t,G,H",    0x40a00000, 0xffe007f8, I64)
    addOpcode("dmfc1",   "t,S", 0x44200000, 0xffe007ff, I3)
    addOpcode("dmfc1",   "t,G",      0x44200000, 0xffe007ff, I3)
    addOpcode("dmtc1",   "t,S", 0x44a00000, 0xffe007ff, I3)
    addOpcode("dmtc1",   "t,G",      0x44a00000, 0xffe007ff, I3)
    addOpcode("dmfc2",   "t,i",      0x48200000, 0xffe00000, IOCTEON)
    addOpcode("dmfc2",   "t,G",      0x48200000, 0xffe007ff, I3)
    addOpcode("dmfc2",   "t,G,H",    0x48200000, 0xffe007f8, I64)
    addOpcode("dmtc2",   "t,i",      0x48a00000, 0xffe00000, IOCTEON)
    addOpcode("dmtc2",   "t,G",      0x48a00000, 0xffe007ff, I3)
    addOpcode("dmtc2",   "t,G,H",    0x48a00000, 0xffe007f8, I64)
    addOpcode("dmfc3",   "t,G",      0x4c200000, 0xffe007ff, I3)
    addOpcode("dmfc3",   "t,G,H",    0x4c200000, 0xffe007f8, I64)
    addOpcode("dmtc3",   "t,G",      0x4ca00000, 0xffe007ff, I3)
    addOpcode("dmtc3",   "t,G,H",    0x4ca00000, 0xffe007f8, I64)
    addOpcode("dmul",    "d,v,t",    0x70000003, 0xfc0007ff, IOCTEON)
    addOpcode("dmult",   "s,t",      0x0000001c, 0xfc00ffff, I3)
    addOpcode("dmultu",  "s,t",      0x0000001d, 0xfc00ffff, I3)
    addOpcode("dneg",    "d,w", 0x0000002e, 0xffe007ff, I3) # dsub 0
    addOpcode("dnegu",   "d,w", 0x0000002f, 0xffe007ff, I3) # dsubu 0
    addOpcode("dpop",    "d,s",      0x7000002d, 0xfc1f07ff, IOCTEON)
    addOpcode("drem",    "z,s,t",    0x0000001e, 0xfc00ffff, I3)
    addOpcode("dremu",   "z,s,t",    0x0000001f, 0xfc00ffff, I3)
    addOpcode("dret",    "",        0x7000003e, 0xffffffff, N5)
    addOpcode("dror",    "d,w,<",   0x0020003a, 0xffe0003f, N5|I65)
    addOpcode("drorv",   "d,t,s",   0x00000056, 0xfc0007ff, N5|I65)
    addOpcode("dror32",  "d,w,<",   0x0020003e, 0xffe0003f, N5|I65)
    addOpcode("drotrv",  "d,t,s",   0x00000056, 0xfc0007ff, I65)
    addOpcode("drotr32", "d,w,<",   0x0020003e, 0xffe0003f, I65)
    addOpcode("dsbh",    "d,w", 0x7c0000a4, 0xffe007ff, I65)
    addOpcode("dshd",    "d,w", 0x7c000164, 0xffe007ff, I65)
    addOpcode("dsllv",   "d,t,s",   0x00000014, 0xfc0007ff, I3)
    addOpcode("dsll32",  "d,w,<",   0x0000003c, 0xffe0003f, I3)
    addOpcode("dsll",    "d,w,s",   0x00000014, 0xfc0007ff, I3) # dsllv
    addOpcode("dsll",    "d,w,>",   0x0000003c, 0xffe0003f, I3) # dsll32
    addOpcode("dsll",    "d,w,<",   0x00000038, 0xffe0003f, I3)
    addOpcode("dsrav",   "d,t,s",   0x00000017, 0xfc0007ff, I3)
    addOpcode("dsra32",  "d,w,<",   0x0000003f, 0xffe0003f, I3)
    addOpcode("dsra",    "d,w,s",   0x00000017, 0xfc0007ff, I3) # dsrav
    addOpcode("dsra",    "d,w,>",   0x0000003f, 0xffe0003f, I3) # dsra32
    addOpcode("dsra",    "d,w,<",   0x0000003b, 0xffe0003f, I3)
    addOpcode("dsrlv",   "d,t,s",   0x00000016, 0xfc0007ff, I3)
    addOpcode("dsrl32",  "d,w,<",   0x0000003e, 0xffe0003f, I3)
    addOpcode("dsrl",    "d,w,s",   0x00000016, 0xfc0007ff, I3) # dsrlv
    addOpcode("dsrl",    "d,w,>",   0x0000003e, 0xffe0003f, I3) # dsrl32
    addOpcode("dsrl",    "d,w,<",   0x0000003a, 0xffe0003f, I3)
    addOpcode("dsub",    "d,v,t",   0x0000002e, 0xfc0007ff, I3)
    addOpcode("dsubu",   "d,v,t",   0x0000002f, 0xfc0007ff, I3)
    addOpcode("ei",      "",        0x41606020, 0xffffffff, I33)
    addOpcode("ei",      "t",   0x41606020, 0xffe0ffff, I33)
    addOpcode("eret",    "",         0x42000018, 0xffffffff, I3|I32)
    addOpcode("ext",     "t,r,+A,+C", 0x7c000000, 0xfc00003f, I33)
    addOpcode("exts32",  "t,r,<,h",  0x7000003b, 0xfc00003f, IOCTEON)
    addOpcode("exts",    "t,r,>,h",  0x7000003b, 0xfc00003f, IOCTEON) # exts32
    addOpcode("exts",    "t,r,<,h",  0x7000003a, 0xfc00003f, IOCTEON)
    addOpcode("floor.l.d", "D,S",   0x4620000b, 0xffff003f, I3)
    addOpcode("floor.l.s", "D,S",   0x4600000b, 0xffff003f, I3)
    addOpcode("floor.w.d", "D,S",   0x4620000f, 0xffff003f, I2)
    addOpcode("floor.w.s", "D,S",   0x4600000f, 0xffff003f, I2)
    addOpcode("flushi",  "",        0xbc010000, 0xffffffff, L1)
    addOpcode("flushd",  "",        0xbc020000, 0xffffffff, L1)
    addOpcode("flushid", "",        0xbc030000, 0xffffffff, L1)
    addOpcode("hibernate","",        0x42000023, 0xffffffff,    V1)
    addOpcode("ins",     "t,r,+A,+B", 0x7c000004, 0xfc00003f, I33)
    addOpcode("jr",      "s",   0x00000008, 0xfc1fffff, I1)
    addOpcode("jr.hb",   "s",   0x00000408, 0xfc1fffff, I33)
    addOpcode("j",       "s",   0x00000008, 0xfc1fffff, I1) # jr
    addOpcode("j",       "a",   0x08000000, 0xfc000000, I1)
    addOpcode("jalr",    "s",   0x0000f809, 0xfc1fffff, I1)
    addOpcode("jalr",    "d,s", 0x00000009, 0xfc1f07ff, I1)
    addOpcode("jalr.hb", "s",   0x0000fc09, 0xfc1fffff, I33)
    addOpcode("jalr.hb", "d,s", 0x00000409, 0xfc1f07ff, I33)
    addOpcode("jal",     "a",   0x0c000000, 0xfc000000, I1)
    addOpcode("jalx",    "a",   0x74000000, 0xfc000000, I16)
    addOpcode("lb",      "t,o(b)",  0x80000000, 0xfc000000, I1)
    addOpcode("lbu",     "t,o(b)",  0x90000000, 0xfc000000, I1)
    addOpcode("ld",     "t,o(b)",   0xdc000000, 0xfc000000, I3)
    addOpcode("ldc1",    "T,o(b)",  0xd4000000, 0xfc000000, I2)
    addOpcode("ldc1",    "E,o(b)",  0xd4000000, 0xfc000000, I2)
    addOpcode("l.d",     "T,o(b)",  0xd4000000, 0xfc000000, I2) # ldc1
    addOpcode("ldc2",    "E,o(b)",  0xd8000000, 0xfc000000, I2)
    addOpcode("ldc3",    "E,o(b)",  0xdc000000, 0xfc000000, I2)
    addOpcode("ldl",        "t,o(b)",   0x68000000, 0xfc000000, I3)
    addOpcode("ldr",        "t,o(b)",   0x6c000000, 0xfc000000, I3)
    addOpcode("ldxc1",   "D,t(b)",  0x4c000001, 0xfc00f83f, I4)
    addOpcode("lh",      "t,o(b)",  0x84000000, 0xfc000000, I1)
    addOpcode("lhu",     "t,o(b)",  0x94000000, 0xfc000000, I1)
    # li is at the start of the table.
    addOpcode("ll",     "t,o(b)",   0xc0000000, 0xfc000000, I2)
    addOpcode("lld",        "t,o(b)",   0xd0000000, 0xfc000000, I3)
    addOpcode("lui",     "t,u", 0x3c000000, 0xffe00000, I1)
    addOpcode("luxc1",   "D,t(b)",  0x4c000005, 0xfc00f83f, I5|N55)
    addOpcode("lw",      "t,o(b)",  0x8c000000, 0xfc000000, I1)
    addOpcode("lwc0",    "E,o(b)",  0xc0000000, 0xfc000000, I1)
    addOpcode("lwc1",    "T,o(b)",  0xc4000000, 0xfc000000, I1)
    addOpcode("lwc1",    "E,o(b)",  0xc4000000, 0xfc000000, I1)
    addOpcode("l.s",     "T,o(b)",  0xc4000000, 0xfc000000, I1) # lwc1
    addOpcode("lwc2",    "E,o(b)",  0xc8000000, 0xfc000000, I1)
    addOpcode("lwc3",    "E,o(b)",  0xcc000000, 0xfc000000, I1)
    addOpcode("lwl",     "t,o(b)",  0x88000000, 0xfc000000, I1)
    addOpcode("lcache",  "t,o(b)",  0x88000000, 0xfc000000, I2) # same
    addOpcode("lwr",     "t,o(b)",  0x98000000, 0xfc000000, I1)
    addOpcode("flush",   "t,o(b)",  0x98000000, 0xfc000000, I2) # same
    addOpcode("lwu",     "t,o(b)",  0x9c000000, 0xfc000000, I3)
    addOpcode("lwxc1",   "D,t(b)",  0x4c000000, 0xfc00f83f, I4)
    addOpcode("macc",    "d,s,t",   0x00000028, 0xfc0007ff, N412)
    addOpcode("macc",    "d,s,t",   0x00000158, 0xfc0007ff, N5)
    addOpcode("maccs",   "d,s,t",   0x00000428, 0xfc0007ff, N412)
    addOpcode("macchi",  "d,s,t",   0x00000228, 0xfc0007ff, N412)
    addOpcode("macchi",  "d,s,t",   0x00000358, 0xfc0007ff, N5)
    addOpcode("macchis", "d,s,t",   0x00000628, 0xfc0007ff, N412)
    addOpcode("macchiu", "d,s,t",   0x00000268, 0xfc0007ff, N412)
    addOpcode("macchiu", "d,s,t",   0x00000359, 0xfc0007ff, N5)
    addOpcode("macchius","d,s,t",   0x00000668, 0xfc0007ff, N412)
    addOpcode("maccu",   "d,s,t",   0x00000068, 0xfc0007ff, N412)
    addOpcode("maccu",   "d,s,t",   0x00000159, 0xfc0007ff, N5)
    addOpcode("maccus",  "d,s,t",   0x00000468, 0xfc0007ff, N412)
    addOpcode("mad",     "s,t",      0x70000000, 0xfc00ffff, P3)
    addOpcode("madu",    "s,t",      0x70000001, 0xfc00ffff, P3)
    addOpcode("madd.d",  "D,R,S,T", 0x4c000021, 0xfc00003f, I4)
    addOpcode("madd.s",  "D,R,S,T", 0x4c000020, 0xfc00003f, I4)
    addOpcode("madd.ps", "D,R,S,T", 0x4c000026, 0xfc00003f, I5)
    addOpcode("madd",    "s,t",      0x0000001c, 0xfc00ffff, L1)
    addOpcode("madd",    "s,t",      0x70000000, 0xfc00ffff, I32|N55)
    addOpcode("madd",    "s,t",      0x70000000, 0xfc00ffff, G1)
    addOpcode("madd",    "d,s,t",    0x70000000, 0xfc0007ff, G1)
    addOpcode("maddu",   "s,t",      0x0000001d, 0xfc00ffff, L1)
    addOpcode("maddu",   "s,t",      0x70000001, 0xfc00ffff, I32|N55)
    addOpcode("maddu",   "s,t",      0x70000001, 0xfc00ffff, G1)
    addOpcode("maddu",   "d,s,t",    0x70000001, 0xfc0007ff, G1)
    addOpcode("madd16",  "s,t",      0x00000028, 0xfc00ffff, N411)
    addOpcode("max.ob",  "X,Y,Q",   0x78000007, 0xfc20003f, MX|SB1)
    addOpcode("max.ob",  "D,S,T",   0x4ac00007, 0xffe0003f, N54)
    addOpcode("max.ob",  "D,S,T[e]",    0x48000007, 0xfe20003f, N54)
    addOpcode("max.ob",  "D,S,k",   0x4bc00007, 0xffe0003f, N54)
    addOpcode("max.qh",  "X,Y,Q",   0x78200007, 0xfc20003f, MX)
    addOpcode("mfpc",    "t,P", 0x4000c801, 0xffe0ffc1, M1|N5)
    addOpcode("mfps",    "t,P", 0x4000c800, 0xffe0ffc1, M1|N5)
    addOpcode("mfc0",    "t,G", 0x40000000, 0xffe007ff, I1)
    addOpcode("mfc0",    "t,+D",     0x40000000, 0xffe007f8, I32)
    addOpcode("mfc0",    "t,G,H",    0x40000000, 0xffe007f8, I32)
    addOpcode("mfc1",    "t,S", 0x44000000, 0xffe007ff, I1)
    addOpcode("mfc1",    "t,G", 0x44000000, 0xffe007ff, I1)
    addOpcode("mfhc1",   "t,S", 0x44600000, 0xffe007ff, I33)
    addOpcode("mfhc1",   "t,G", 0x44600000, 0xffe007ff, I33)
    addOpcode("mfc2",    "t,G", 0x48000000, 0xffe007ff, I1)
    addOpcode("mfc2",    "t,G,H",    0x48000000, 0xffe007f8, I32)
    addOpcode("mfhc2",   "t,i", 0x48600000, 0xffe00000, I33)
    addOpcode("mfc3",    "t,G", 0x4c000000, 0xffe007ff, I1)
    addOpcode("mfc3",    "t,G,H",    0x4c000000, 0xffe007f8, I32)
    addOpcode("mfdr",    "t,G", 0x7000003d, 0xffe007ff, N5)
    addOpcode("mfhi",    "d",   0x00000010, 0xffff07ff, I1)
    addOpcode("mflo",    "d",   0x00000012, 0xffff07ff, I1)
    addOpcode("min.ob",  "X,Y,Q",   0x78000006, 0xfc20003f, MX|SB1)
    addOpcode("min.ob",  "D,S,T",   0x4ac00006, 0xffe0003f, N54)
    addOpcode("min.ob",  "D,S,T[e]",    0x48000006, 0xfe20003f, N54)
    addOpcode("min.ob",  "D,S,k",   0x4bc00006, 0xffe0003f, N54)
    addOpcode("min.qh",  "X,Y,Q",   0x78200006, 0xfc20003f, MX)
    addOpcode("mov.d",   "D,S", 0x46200006, 0xffff003f, I1)
    addOpcode("mov.s",   "D,S", 0x46000006, 0xffff003f, I1)
    addOpcode("mov.ps",  "D,S", 0x46c00006, 0xffff003f, I5)
    addOpcode("movf",    "d,s,N",    0x00000001, 0xfc0307ff, I4|I32)
    addOpcode("movf.d",  "D,S,N",    0x46200011, 0xffe3003f, I4|I32)
    addOpcode("movf.l",  "D,S,N",   0x46a00011, 0xffe3003f, MX|SB1)
    addOpcode("movf.l",  "X,Y,N",   0x46a00011, 0xffe3003f, MX|SB1)
    addOpcode("movf.s",  "D,S,N",    0x46000011, 0xffe3003f, I4|I32)
    addOpcode("movf.ps", "D,S,N",   0x46c00011, 0xffe3003f, I5)
    addOpcode("movn",    "d,v,t",    0x0000000b, 0xfc0007ff, I4|I32)
    addOpcode("ffc",     "d,v", 0x0000000b, 0xfc1f07ff, L1)
    addOpcode("movn.d",  "D,S,t",    0x46200013, 0xffe0003f, I4|I32)
    addOpcode("movn.l",  "D,S,t",    0x46a00013, 0xffe0003f, MX|SB1)
    addOpcode("movn.l",  "X,Y,t",    0x46a00013, 0xffe0003f, MX|SB1)
    addOpcode("movn.s",  "D,S,t",    0x46000013, 0xffe0003f, I4|I32)
    addOpcode("movn.ps", "D,S,t",    0x46c00013, 0xffe0003f, I5)
    addOpcode("movt",    "d,s,N",    0x00010001, 0xfc0307ff, I4|I32)
    addOpcode("movt.d",  "D,S,N",    0x46210011, 0xffe3003f, I4|I32)
    addOpcode("movt.l",  "D,S,N",    0x46a10011, 0xffe3003f, MX|SB1)
    addOpcode("movt.l",  "X,Y,N",    0x46a10011, 0xffe3003f, MX|SB1)
    addOpcode("movt.s",  "D,S,N",    0x46010011, 0xffe3003f, I4|I32)
    addOpcode("movt.ps", "D,S,N",   0x46c10011, 0xffe3003f, I5)
    addOpcode("movz",    "d,v,t",    0x0000000a, 0xfc0007ff, I4|I32)
    addOpcode("ffs",     "d,v", 0x0000000a, 0xfc1f07ff, L1)
    addOpcode("movz.d",  "D,S,t",    0x46200012, 0xffe0003f, I4|I32)
    addOpcode("movz.l",  "D,S,t",    0x46a00012, 0xffe0003f, MX|SB1)
    addOpcode("movz.l",  "X,Y,t",    0x46a00012, 0xffe0003f, MX|SB1)
    addOpcode("movz.s",  "D,S,t",    0x46000012, 0xffe0003f, I4|I32)
    addOpcode("movz.ps", "D,S,t",    0x46c00012, 0xffe0003f, I5)
    addOpcode("msac",    "d,s,t",   0x000001d8, 0xfc0007ff, N5)
    addOpcode("msacu",   "d,s,t",   0x000001d9, 0xfc0007ff, N5)
    addOpcode("msachi",  "d,s,t",   0x000003d8, 0xfc0007ff, N5)
    addOpcode("msachiu", "d,s,t",   0x000003d9, 0xfc0007ff, N5)
    # move is at the top of the table.
    addOpcode("msgn.qh", "X,Y,Q",   0x78200000, 0xfc20003f, MX)
    addOpcode("msub.d",  "D,R,S,T", 0x4c000029, 0xfc00003f, I4)
    addOpcode("msub.s",  "D,R,S,T", 0x4c000028, 0xfc00003f, I4)
    addOpcode("msub.ps", "D,R,S,T", 0x4c00002e, 0xfc00003f, I5)
    addOpcode("msub",    "s,t",      0x0000001e, 0xfc00ffff, L1)
    addOpcode("msub",    "s,t",      0x70000004, 0xfc00ffff, I32|N55)
    addOpcode("msubu",   "s,t",      0x0000001f, 0xfc00ffff, L1)
    addOpcode("msubu",   "s,t",      0x70000005, 0xfc00ffff, I32|N55)
    addOpcode("mtpc",    "t,P", 0x4080c801, 0xffe0ffc1, M1|N5)
    addOpcode("mtps",    "t,P", 0x4080c800, 0xffe0ffc1, M1|N5)
    addOpcode("mtc0",    "t,G", 0x40800000, 0xffe007ff, I1)
    addOpcode("mtc0",    "t,+D",     0x40800000, 0xffe007f8, I32)
    addOpcode("mtc0",    "t,G,H",    0x40800000, 0xffe007f8, I32)
    addOpcode("mtc1",    "t,S", 0x44800000, 0xffe007ff, I1)
    addOpcode("mtc1",    "t,G", 0x44800000, 0xffe007ff, I1)
    addOpcode("mthc1",   "t,S", 0x44e00000, 0xffe007ff, I33)
    addOpcode("mthc1",   "t,G", 0x44e00000, 0xffe007ff, I33)
    addOpcode("mtc2",    "t,G", 0x48800000, 0xffe007ff, I1)
    addOpcode("mtc2",    "t,G,H",    0x48800000, 0xffe007f8, I32)
    addOpcode("mthc2",   "t,i", 0x48e00000, 0xffe00000, I33)
    addOpcode("mtc3",    "t,G", 0x4c800000, 0xffe007ff, I1)
    addOpcode("mtc3",    "t,G,H",    0x4c800000, 0xffe007f8, I32)
    addOpcode("mtdr",    "t,G", 0x7080003d, 0xffe007ff, N5)
    addOpcode("mthi",    "s",   0x00000011, 0xfc1fffff, I1)
    addOpcode("mtlo",    "s",   0x00000013, 0xfc1fffff, I1)
    addOpcode("mtm0",    "s",        0x70000008, 0xfc1fffff, IOCTEON)
    addOpcode("mtm1",    "s",        0x7000000c, 0xfc1fffff, IOCTEON)
    addOpcode("mtm2",    "s",        0x7000000d, 0xfc1fffff, IOCTEON)
    addOpcode("mtp0",    "s",        0x70000009, 0xfc1fffff, IOCTEON)
    addOpcode("mtp1",    "s",        0x7000000a, 0xfc1fffff, IOCTEON)
    addOpcode("mtp2",    "s",        0x7000000b, 0xfc1fffff, IOCTEON)
    addOpcode("mul.d",   "D,V,T",   0x46200002, 0xffe0003f, I1)
    addOpcode("mul.s",   "D,V,T",   0x46000002, 0xffe0003f, I1)
    addOpcode("mul.ob",  "X,Y,Q",   0x78000030, 0xfc20003f, MX|SB1)
    addOpcode("mul.ob",  "D,S,T",   0x4ac00030, 0xffe0003f, N54)
    addOpcode("mul.ob",  "D,S,T[e]",    0x48000030, 0xfe20003f, N54)
    addOpcode("mul.ob",  "D,S,k",   0x4bc00030, 0xffe0003f, N54)
    addOpcode("mul.ps",  "D,V,T",   0x46c00002, 0xffe0003f, I5)
    addOpcode("mul.qh",  "X,Y,Q",   0x78200030, 0xfc20003f, MX)
    addOpcode("mul",     "d,v,t",    0x70000002, 0xfc0007ff, I32|P3|N55)
    addOpcode("mul",     "d,s,t",   0x00000058, 0xfc0007ff, N54)
    addOpcode("mula.ob", "Y,Q", 0x78000033, 0xfc2007ff, MX|SB1)
    addOpcode("mula.ob", "S,T", 0x4ac00033, 0xffe007ff, N54)
    addOpcode("mula.ob", "S,T[e]",  0x48000033, 0xfe2007ff, N54)
    addOpcode("mula.ob", "S,k", 0x4bc00033, 0xffe007ff, N54)
    addOpcode("mula.qh", "Y,Q", 0x78200033, 0xfc2007ff, MX)
    addOpcode("mulhi",   "d,s,t",   0x00000258, 0xfc0007ff, N5)
    addOpcode("mulhiu",  "d,s,t",   0x00000259, 0xfc0007ff, N5)
    addOpcode("mull.ob", "Y,Q", 0x78000433, 0xfc2007ff, MX|SB1)
    addOpcode("mull.ob", "S,T", 0x4ac00433, 0xffe007ff, N54)
    addOpcode("mull.ob", "S,T[e]",  0x48000433, 0xfe2007ff, N54)
    addOpcode("mull.ob", "S,k", 0x4bc00433, 0xffe007ff, N54)
    addOpcode("mull.qh", "Y,Q", 0x78200433, 0xfc2007ff, MX)
    addOpcode("mulr.ps", "D,S,T",   0x46c0001a, 0xffe0003f, M3D)
    addOpcode("muls",    "d,s,t",   0x000000d8, 0xfc0007ff, N5)
    addOpcode("mulsu",   "d,s,t",   0x000000d9, 0xfc0007ff, N5)
    addOpcode("mulshi",  "d,s,t",   0x000002d8, 0xfc0007ff, N5)
    addOpcode("mulshiu", "d,s,t",   0x000002d9, 0xfc0007ff, N5)
    addOpcode("muls.ob", "Y,Q", 0x78000032, 0xfc2007ff, MX|SB1)
    addOpcode("muls.ob", "S,T", 0x4ac00032, 0xffe007ff, N54)
    addOpcode("muls.ob", "S,T[e]",  0x48000032, 0xfe2007ff, N54)
    addOpcode("muls.ob", "S,k", 0x4bc00032, 0xffe007ff, N54)
    addOpcode("muls.qh", "Y,Q", 0x78200032, 0xfc2007ff, MX)
    addOpcode("mulsl.ob", "Y,Q",    0x78000432, 0xfc2007ff, MX|SB1)
    addOpcode("mulsl.ob", "S,T",    0x4ac00432, 0xffe007ff, N54)
    addOpcode("mulsl.ob", "S,T[e]", 0x48000432, 0xfe2007ff, N54)
    addOpcode("mulsl.ob", "S,k",    0x4bc00432, 0xffe007ff, N54)
    addOpcode("mulsl.qh", "Y,Q",    0x78200432, 0xfc2007ff, MX)
    addOpcode("mult",    "s,t",      0x00000018, 0xfc00ffff, I1)
    addOpcode("mult",    "d,s,t",    0x00000018, 0xfc0007ff, G1)
    addOpcode("multu",   "s,t",      0x00000019, 0xfc00ffff, I1)
    addOpcode("multu",   "d,s,t",    0x00000019, 0xfc0007ff, G1)
    addOpcode("mulu",    "d,s,t",   0x00000059, 0xfc0007ff, N5)
    addOpcode("neg",     "d,w", 0x00000022, 0xffe007ff, I1) # sub 0
    addOpcode("negu",    "d,w", 0x00000023, 0xffe007ff, I1) # subu 0
    addOpcode("neg.d",   "D,V", 0x46200007, 0xffff003f, I1)
    addOpcode("neg.s",   "D,V", 0x46000007, 0xffff003f, I1)
    addOpcode("neg.ps",  "D,V", 0x46c00007, 0xffff003f, I5)
    addOpcode("nmadd.d", "D,R,S,T", 0x4c000031, 0xfc00003f, I4)
    addOpcode("nmadd.s", "D,R,S,T", 0x4c000030, 0xfc00003f, I4)
    addOpcode("nmadd.ps","D,R,S,T", 0x4c000036, 0xfc00003f, I5)
    addOpcode("nmsub.d", "D,R,S,T", 0x4c000039, 0xfc00003f, I4)
    addOpcode("nmsub.s", "D,R,S,T", 0x4c000038, 0xfc00003f, I4)
    addOpcode("nmsub.ps","D,R,S,T", 0x4c00003e, 0xfc00003f, I5)
    # nop is at the start of the table.
    addOpcode("nor",     "d,v,t",   0x00000027, 0xfc0007ff, I1)
    addOpcode("nor.ob",  "X,Y,Q",   0x7800000f, 0xfc20003f, MX|SB1)
    addOpcode("nor.ob",  "D,S,T",   0x4ac0000f, 0xffe0003f, N54)
    addOpcode("nor.ob",  "D,S,T[e]",    0x4800000f, 0xfe20003f, N54)
    addOpcode("nor.ob",  "D,S,k",   0x4bc0000f, 0xffe0003f, N54)
    addOpcode("nor.qh",  "X,Y,Q",   0x7820000f, 0xfc20003f, MX)
    addOpcode("not",     "d,v", 0x00000027, 0xfc1f07ff, I1)#nor d,s,0
    addOpcode("or",      "d,v,t",   0x00000025, 0xfc0007ff, I1)
    addOpcode("or.ob",   "X,Y,Q",   0x7800000e, 0xfc20003f, MX|SB1)
    addOpcode("or.ob",   "D,S,T",   0x4ac0000e, 0xffe0003f, N54)
    addOpcode("or.ob",   "D,S,T[e]",    0x4800000e, 0xfe20003f, N54)
    addOpcode("or.ob",   "D,S,k",   0x4bc0000e, 0xffe0003f, N54)
    addOpcode("or.qh",   "X,Y,Q",   0x7820000e, 0xfc20003f, MX)
    addOpcode("ori",     "t,r,i",   0x34000000, 0xfc000000, I1)
    addOpcode("pabsdiff.ob", "X,Y,Q",0x78000009, 0xfc20003f,    SB1)
    addOpcode("pabsdiffc.ob", "Y,Q",    0x78000035, 0xfc2007ff, SB1)
    addOpcode("pavg.ob", "X,Y,Q",   0x78000008, 0xfc20003f, SB1)
    addOpcode("pickf.ob", "X,Y,Q",  0x78000002, 0xfc20003f, MX|SB1)
    addOpcode("pickf.ob", "D,S,T",  0x4ac00002, 0xffe0003f, N54)
    addOpcode("pickf.ob", "D,S,T[e]",0x48000002, 0xfe20003f,    N54)
    addOpcode("pickf.ob", "D,S,k",  0x4bc00002, 0xffe0003f, N54)
    addOpcode("pickf.qh", "X,Y,Q",  0x78200002, 0xfc20003f, MX)
    addOpcode("pickt.ob", "X,Y,Q",  0x78000003, 0xfc20003f, MX|SB1)
    addOpcode("pickt.ob", "D,S,T",  0x4ac00003, 0xffe0003f, N54)
    addOpcode("pickt.ob", "D,S,T[e]",0x48000003, 0xfe20003f,    N54)
    addOpcode("pickt.ob", "D,S,k",  0x4bc00003, 0xffe0003f, N54)
    addOpcode("pickt.qh", "X,Y,Q",  0x78200003, 0xfc20003f, MX)
    addOpcode("pll.ps",  "D,V,T",   0x46c0002c, 0xffe0003f, I5)
    addOpcode("plu.ps",  "D,V,T",   0x46c0002d, 0xffe0003f, I5)
    addOpcode("pop",     "d,s",      0x7000002c, 0xfc1f07ff, IOCTEON)
      # pref and prefx are at the start of the table.
    addOpcode("pul.ps",  "D,V,T",   0x46c0002e, 0xffe0003f, I5)
    addOpcode("puu.ps",  "D,V,T",   0x46c0002f, 0xffe0003f, I5)
    addOpcode("rach.ob", "X",   0x7a00003f, 0xfffff83f, MX|SB1)
    addOpcode("rach.ob", "D",   0x4a00003f, 0xfffff83f, N54)
    addOpcode("rach.qh", "X",   0x7a20003f, 0xfffff83f, MX)
    addOpcode("racl.ob", "X",   0x7800003f, 0xfffff83f, MX|SB1)
    addOpcode("racl.ob", "D",   0x4800003f, 0xfffff83f, N54)
    addOpcode("racl.qh", "X",   0x7820003f, 0xfffff83f, MX)
    addOpcode("racm.ob", "X",   0x7900003f, 0xfffff83f, MX|SB1)
    addOpcode("racm.ob", "D",   0x4900003f, 0xfffff83f, N54)
    addOpcode("racm.qh", "X",   0x7920003f, 0xfffff83f, MX)
    addOpcode("recip.d", "D,S", 0x46200015, 0xffff003f, I4)
    addOpcode("recip.ps","D,S", 0x46c00015, 0xffff003f, SB1)
    addOpcode("recip.s", "D,S", 0x46000015, 0xffff003f, I4)
    addOpcode("recip1.d",  "D,S",   0x4620001d, 0xffff003f, M3D)
    addOpcode("recip1.ps", "D,S",   0x46c0001d, 0xffff003f, M3D)
    addOpcode("recip1.s",  "D,S",   0x4600001d, 0xffff003f, M3D)
    addOpcode("recip2.d",  "D,S,T", 0x4620001c, 0xffe0003f, M3D)
    addOpcode("recip2.ps", "D,S,T", 0x46c0001c, 0xffe0003f, M3D)
    addOpcode("recip2.s",  "D,S,T", 0x4600001c, 0xffe0003f, M3D)
    addOpcode("rem",     "z,s,t",    0x0000001a, 0xfc00ffff, I1)
    addOpcode("remu",    "z,s,t",    0x0000001b, 0xfc00ffff, I1)
    addOpcode("rdhwr",   "t,K", 0x7c00003b, 0xffe007ff, I33)
    addOpcode("rdpgpr",  "d,w", 0x41400000, 0xffe007ff, I33)
    addOpcode("rfe",     "",        0x42000010, 0xffffffff, I1|T3)
    addOpcode("rnas.qh", "X,Q", 0x78200025, 0xfc20f83f, MX)
    addOpcode("rnau.ob", "X,Q", 0x78000021, 0xfc20f83f, MX|SB1)
    addOpcode("rnau.qh", "X,Q", 0x78200021, 0xfc20f83f, MX)
    addOpcode("rnes.qh", "X,Q", 0x78200026, 0xfc20f83f, MX)
    addOpcode("rneu.ob", "X,Q", 0x78000022, 0xfc20f83f, MX|SB1)
    addOpcode("rneu.qh", "X,Q", 0x78200022, 0xfc20f83f, MX)
    addOpcode("ror",        "d,w,<",    0x00200002, 0xffe0003f, N5|I33)
    addOpcode("rorv",    "d,t,s",   0x00000046, 0xfc0007ff, N5|I33)
    addOpcode("rotrv",   "d,t,s",   0x00000046, 0xfc0007ff, I33)
    addOpcode("round.l.d", "D,S",   0x46200008, 0xffff003f, I3)
    addOpcode("round.l.s", "D,S",   0x46000008, 0xffff003f, I3)
    addOpcode("round.w.d", "D,S",   0x4620000c, 0xffff003f, I2)
    addOpcode("round.w.s", "D,S",   0x4600000c, 0xffff003f, I2)
    addOpcode("rsqrt.d", "D,S", 0x46200016, 0xffff003f, I4)
    addOpcode("rsqrt.ps","D,S", 0x46c00016, 0xffff003f, SB1)
    addOpcode("rsqrt.s", "D,S", 0x46000016, 0xffff003f, I4)
    addOpcode("rsqrt1.d",  "D,S",   0x4620001e, 0xffff003f, M3D)
    addOpcode("rsqrt1.ps", "D,S",   0x46c0001e, 0xffff003f, M3D)
    addOpcode("rsqrt1.s",  "D,S",   0x4600001e, 0xffff003f, M3D)
    addOpcode("rsqrt2.d",  "D,S,T", 0x4620001f, 0xffe0003f, M3D)
    addOpcode("rsqrt2.ps", "D,S,T", 0x46c0001f, 0xffe0003f, M3D)
    addOpcode("rsqrt2.s",  "D,S,T", 0x4600001f, 0xffe0003f, M3D)
    addOpcode("rzs.qh",  "X,Q", 0x78200024, 0xfc20f83f, MX)
    addOpcode("rzu.ob",  "X,Q", 0x78000020, 0xfc20f83f, MX|SB1)
    addOpcode("rzu.ob",  "D,k", 0x4bc00020, 0xffe0f83f, N54)
    addOpcode("rzu.qh",  "X,Q", 0x78200020, 0xfc20f83f, MX)
    addOpcode("sb",      "t,o(b)",  0xa0000000, 0xfc000000, I1)
    addOpcode("sc",     "t,o(b)",   0xe0000000, 0xfc000000, I2)
    addOpcode("scd",        "t,o(b)",   0xf0000000, 0xfc000000, I3)
    addOpcode("sd",     "t,o(b)",   0xfc000000, 0xfc000000, I3)
    addOpcode("sdbbp",   "",        0x0000000e, 0xffffffff, G2)
    addOpcode("sdbbp",   "c",   0x0000000e, 0xfc00ffff, G2)
    addOpcode("sdbbp",   "c,q", 0x0000000e, 0xfc00003f, G2)
    addOpcode("sdbbp",   "",         0x7000003f, 0xffffffff, I32)
    addOpcode("sdbbp",   "B",        0x7000003f, 0xfc00003f, I32)
    addOpcode("sdc1",    "T,o(b)",  0xf4000000, 0xfc000000, I2)
    addOpcode("sdc1",    "E,o(b)",  0xf4000000, 0xfc000000, I2)
    addOpcode("sdc2",    "E,o(b)",  0xf8000000, 0xfc000000, I2)
    addOpcode("sdc3",    "E,o(b)",  0xfc000000, 0xfc000000, I2)
    addOpcode("s.d",     "T,o(b)",  0xf4000000, 0xfc000000, I2)
    addOpcode("sdl",     "t,o(b)",  0xb0000000, 0xfc000000, I3)
    addOpcode("sdr",     "t,o(b)",  0xb4000000, 0xfc000000, I3)
    addOpcode("sdxc1",   "S,t(b)",   0x4c000009, 0xfc0007ff, I4)
    addOpcode("seb",     "d,w", 0x7c000420, 0xffe007ff, I33)
    addOpcode("seh",     "d,w", 0x7c000620, 0xffe007ff, I33)
    addOpcode("selsl",   "d,v,t",   0x00000005, 0xfc0007ff, L1)
    addOpcode("selsr",   "d,v,t",   0x00000001, 0xfc0007ff, L1)
    addOpcode("seq",     "d,v,t",    0x7000002a, 0xfc0007ff, IOCTEON)
    addOpcode("seqi",    "t,r,y",    0x7000002e, 0xfc00003f, IOCTEON)
    addOpcode("sh",      "t,o(b)",  0xa4000000, 0xfc000000, I1)
    addOpcode("shfl.bfla.qh", "X,Y,Z", 0x7a20001f, 0xffe0003f, MX)
    addOpcode("shfl.mixh.ob", "X,Y,Z", 0x7980001f, 0xffe0003f, MX|SB1)
    addOpcode("shfl.mixh.ob", "D,S,T", 0x4980001f, 0xffe0003f, N54)
    addOpcode("shfl.mixh.qh", "X,Y,Z", 0x7820001f, 0xffe0003f, MX)
    addOpcode("shfl.mixl.ob", "X,Y,Z", 0x79c0001f, 0xffe0003f, MX|SB1)
    addOpcode("shfl.mixl.ob", "D,S,T", 0x49c0001f, 0xffe0003f, N54)
    addOpcode("shfl.mixl.qh", "X,Y,Z", 0x78a0001f, 0xffe0003f, MX)
    addOpcode("shfl.pach.ob", "X,Y,Z", 0x7900001f, 0xffe0003f, MX|SB1)
    addOpcode("shfl.pach.ob", "D,S,T", 0x4900001f, 0xffe0003f, N54)
    addOpcode("shfl.pach.qh", "X,Y,Z", 0x7920001f, 0xffe0003f, MX)
    addOpcode("shfl.pacl.ob", "D,S,T", 0x4940001f, 0xffe0003f, N54)
    addOpcode("shfl.repa.qh", "X,Y,Z", 0x7b20001f, 0xffe0003f, MX)
    addOpcode("shfl.repb.qh", "X,Y,Z", 0x7ba0001f, 0xffe0003f, MX)
    addOpcode("shfl.upsl.ob", "X,Y,Z", 0x78c0001f, 0xffe0003f, MX|SB1)
    addOpcode("sllv",    "d,t,s",   0x00000004, 0xfc0007ff, I1)
    addOpcode("sll",     "d,w,s",   0x00000004, 0xfc0007ff, I1) # sllv
    addOpcode("sll",     "d,w,<",   0x00000000, 0xffe0003f, I1)
    addOpcode("sll.ob",  "X,Y,Q",   0x78000010, 0xfc20003f, MX|SB1)
    addOpcode("sll.ob",  "D,S,T[e]",    0x48000010, 0xfe20003f, N54)
    addOpcode("sll.ob",  "D,S,k",   0x4bc00010, 0xffe0003f, N54)
    addOpcode("sll.qh",  "X,Y,Q",   0x78200010, 0xfc20003f, MX)
    addOpcode("slt",     "d,v,t",   0x0000002a, 0xfc0007ff, I1)
    addOpcode("slti",    "t,r,j",   0x28000000, 0xfc000000, I1)
    addOpcode("sltiu",   "t,r,j",   0x2c000000, 0xfc000000, I1)
    addOpcode("sltu",    "d,v,t",   0x0000002b, 0xfc0007ff, I1)
    addOpcode("sne",     "d,v,t",    0x7000002b, 0xfc0007ff, IOCTEON)
    addOpcode("snei",    "t,r,y",    0x7000002f, 0xfc00003f, IOCTEON)
    addOpcode("sqrt.d",  "D,S", 0x46200004, 0xffff003f, I2)
    addOpcode("sqrt.s",  "D,S", 0x46000004, 0xffff003f, I2)
    addOpcode("sqrt.ps", "D,S", 0x46c00004, 0xffff003f, SB1)
    addOpcode("srav",    "d,t,s",   0x00000007, 0xfc0007ff, I1)
    addOpcode("sra",     "d,w,s",   0x00000007, 0xfc0007ff, I1) # srav
    addOpcode("sra",     "d,w,<",   0x00000003, 0xffe0003f, I1)
    addOpcode("sra.qh",  "X,Y,Q",   0x78200013, 0xfc20003f, MX)
    addOpcode("srlv",    "d,t,s",   0x00000006, 0xfc0007ff, I1)
    addOpcode("srl",     "d,w,s",   0x00000006, 0xfc0007ff, I1) # srlv
    addOpcode("srl",     "d,w,<",   0x00000002, 0xffe0003f, I1)
    addOpcode("srl.ob",  "X,Y,Q",   0x78000012, 0xfc20003f, MX|SB1)
    addOpcode("srl.ob",  "D,S,T[e]",    0x48000012, 0xfe20003f, N54)
    addOpcode("srl.ob",  "D,S,k",   0x4bc00012, 0xffe0003f, N54)
    addOpcode("srl.qh",  "X,Y,Q",   0x78200012, 0xfc20003f, MX)
    # ssnop is at the start of the table.
    addOpcode("standby", "",         0x42000021, 0xffffffff,    V1)
    addOpcode("sub",     "d,v,t",   0x00000022, 0xfc0007ff, I1)
    addOpcode("sub.d",   "D,V,T",   0x46200001, 0xffe0003f, I1)
    addOpcode("sub.s",   "D,V,T",   0x46000001, 0xffe0003f, I1)
    addOpcode("sub.ob",  "X,Y,Q",   0x7800000a, 0xfc20003f, MX|SB1)
    addOpcode("sub.ob",  "D,S,T",   0x4ac0000a, 0xffe0003f, N54)
    addOpcode("sub.ob",  "D,S,T[e]",    0x4800000a, 0xfe20003f, N54)
    addOpcode("sub.ob",  "D,S,k",   0x4bc0000a, 0xffe0003f, N54)
    addOpcode("sub.ps",  "D,V,T",   0x46c00001, 0xffe0003f, I5)
    addOpcode("sub.qh",  "X,Y,Q",   0x7820000a, 0xfc20003f, MX)
    addOpcode("suba.ob", "Y,Q", 0x78000036, 0xfc2007ff, MX|SB1)
    addOpcode("suba.qh", "Y,Q", 0x78200036, 0xfc2007ff, MX)
    addOpcode("subl.ob", "Y,Q", 0x78000436, 0xfc2007ff, MX|SB1)
    addOpcode("subl.qh", "Y,Q", 0x78200436, 0xfc2007ff, MX)
    addOpcode("subu",    "d,v,t",   0x00000023, 0xfc0007ff, I1)
    addOpcode("suspend", "",         0x42000022, 0xffffffff,    V1)
    addOpcode("suxc1",   "S,t(b)",   0x4c00000d, 0xfc0007ff, I5|N55)
    addOpcode("sw",      "t,o(b)",  0xac000000, 0xfc000000, I1)
    addOpcode("swc0",    "E,o(b)",  0xe0000000, 0xfc000000, I1)
    addOpcode("swc1",    "T,o(b)",  0xe4000000, 0xfc000000, I1)
    addOpcode("swc1",    "E,o(b)",  0xe4000000, 0xfc000000, I1)
    addOpcode("s.s",     "T,o(b)",  0xe4000000, 0xfc000000, I1) # swc1
    addOpcode("swc2",    "E,o(b)",  0xe8000000, 0xfc000000, I1)
    addOpcode("swc3",    "E,o(b)",  0xec000000, 0xfc000000, I1)
    addOpcode("swl",     "t,o(b)",  0xa8000000, 0xfc000000, I1)
    addOpcode("scache",  "t,o(b)",  0xa8000000, 0xfc000000, I2) # same
    addOpcode("swr",     "t,o(b)",  0xb8000000, 0xfc000000, I1)
    addOpcode("invalidate", "t,o(b)",0xb8000000, 0xfc000000,    I2) # same
    addOpcode("swxc1",   "S,t(b)",   0x4c000008, 0xfc0007ff, I4)
    addOpcode("sync",    "",        0x0000000f, 0xffffffff, I2|G1)
    addOpcode("sync.p",  "",        0x0000040f, 0xffffffff, I2)
    addOpcode("sync.l",  "",        0x0000000f, 0xffffffff, I2)
    addOpcode("synci",   "o(b)",    0x041f0000, 0xfc1f0000, I33)
    addOpcode("syncio",  "",         0x0000004f, 0xffffffff, IOCTEON)
    addOpcode("synciobdma", "",      0x0000008f, 0xffffffff, IOCTEON)
    addOpcode("syncioall", "",       0x000000cf, 0xffffffff, IOCTEON)
    addOpcode("syncw",   "",         0x0000010f, 0xffffffff, IOCTEON)
    addOpcode("syscall", "",        0x0000000c, 0xffffffff, I1)
    addOpcode("syscall", "B",   0x0000000c, 0xfc00003f, I1)
    addOpcode("teqi",    "s,j", 0x040c0000, 0xfc1f0000, I2)
    addOpcode("teq",        "s,t",  0x00000034, 0xfc00ffff, I2)
    addOpcode("teq",        "s,t,q",    0x00000034, 0xfc00003f, I2)
    addOpcode("teq",     "s,j", 0x040c0000, 0xfc1f0000, I2) # teqi
    addOpcode("tgei",    "s,j", 0x04080000, 0xfc1f0000, I2)
    addOpcode("tge",        "s,t",  0x00000030, 0xfc00ffff, I2)
    addOpcode("tge",        "s,t,q",    0x00000030, 0xfc00003f, I2)
    addOpcode("tge",     "s,j", 0x04080000, 0xfc1f0000, I2) # tgei
    addOpcode("tgeiu",   "s,j", 0x04090000, 0xfc1f0000, I2)
    addOpcode("tgeu",    "s,t", 0x00000031, 0xfc00ffff, I2)
    addOpcode("tgeu",    "s,t,q",   0x00000031, 0xfc00003f, I2)
    addOpcode("tgeu",    "s,j", 0x04090000, 0xfc1f0000, I2) # tgeiu
    addOpcode("tlbp",    "",         0x42000008, 0xffffffff, I1)
    addOpcode("tlbr",    "",         0x42000001, 0xffffffff, I1)
    addOpcode("tlbwi",   "",         0x42000002, 0xffffffff, I1)
    addOpcode("tlbwr",   "",         0x42000006, 0xffffffff, I1)
    addOpcode("tlti",    "s,j", 0x040a0000, 0xfc1f0000, I2)
    addOpcode("tlt",     "s,t", 0x00000032, 0xfc00ffff, I2)
    addOpcode("tlt",     "s,t,q",   0x00000032, 0xfc00003f, I2)
    addOpcode("tlt",     "s,j", 0x040a0000, 0xfc1f0000, I2) # tlti
    addOpcode("tltiu",   "s,j", 0x040b0000, 0xfc1f0000, I2)
    addOpcode("tltu",    "s,t", 0x00000033, 0xfc00ffff, I2)
    addOpcode("tltu",    "s,t,q",   0x00000033, 0xfc00003f, I2)
    addOpcode("tltu",    "s,j", 0x040b0000, 0xfc1f0000, I2) # tltiu
    addOpcode("tnei",    "s,j", 0x040e0000, 0xfc1f0000, I2)
    addOpcode("tne",     "s,t", 0x00000036, 0xfc00ffff, I2)
    addOpcode("tne",     "s,t,q",   0x00000036, 0xfc00003f, I2)
    addOpcode("tne",     "s,j", 0x040e0000, 0xfc1f0000, I2) # tnei
    addOpcode("trunc.l.d", "D,S",   0x46200009, 0xffff003f, I3)
    addOpcode("trunc.l.s", "D,S",   0x46000009, 0xffff003f, I3)
    addOpcode("trunc.w.d", "D,S",   0x4620000d, 0xffff003f, I2)
    addOpcode("trunc.w.d", "D,S,x", 0x4620000d, 0xffff003f, I2)
    addOpcode("trunc.w.s", "D,S",   0x4600000d, 0xffff003f, I2)
    addOpcode("trunc.w.s", "D,S,x", 0x4600000d, 0xffff003f, I2)
    addOpcode("uld",     "t,o(b)",   0x68000000, 0xfc000000, I3)
    addOpcode("ulw",     "t,o(b)",   0x88000000, 0xfc000000, I1)
    addOpcode("usd",     "t,o(b)",   0xb0000000, 0xfc000000, I3)
    addOpcode("usw",     "t,o(b)",   0xa8000000, 0xfc000000, I1)
    addOpcode("v3mulu",  "d,v,t",    0x70000011, 0xfc0007ff, IOCTEON)
    addOpcode("vmm0",    "d,v,t",    0x70000010, 0xfc0007ff, IOCTEON)
    addOpcode("vmulu",   "d,v,t",    0x7000000f, 0xfc0007ff, IOCTEON)
    addOpcode("wach.ob", "Y",   0x7a00003e, 0xffff07ff, MX|SB1)
    addOpcode("wach.ob", "S",   0x4a00003e, 0xffff07ff, N54)
    addOpcode("wach.qh", "Y",   0x7a20003e, 0xffff07ff, MX)
    addOpcode("wacl.ob", "Y,Z", 0x7800003e, 0xffe007ff, MX|SB1)
    addOpcode("wacl.ob", "S,T", 0x4800003e, 0xffe007ff, N54)
    addOpcode("wacl.qh", "Y,Z", 0x7820003e, 0xffe007ff, MX)
    addOpcode("wait",    "",         0x42000020, 0xffffffff, I3|I32)
    addOpcode("wait",    "J",        0x42000020, 0xfe00003f, I32|N55)
    addOpcode("waiti",   "",        0x42000020, 0xffffffff, L1)
    addOpcode("wb",         "o(b)", 0xbc040000, 0xfc1f0000, L1)
    addOpcode("wrpgpr",  "d,w", 0x41c00000, 0xffe007ff, I33)
    addOpcode("wsbh",    "d,w", 0x7c0000a0, 0xffe007ff, I33)
    addOpcode("xor",     "d,v,t",   0x00000026, 0xfc0007ff, I1)
    addOpcode("xor.ob",  "X,Y,Q",   0x7800000d, 0xfc20003f, MX|SB1)
    addOpcode("xor.ob",  "D,S,T",   0x4ac0000d, 0xffe0003f, N54)
    addOpcode("xor.ob",  "D,S,T[e]",    0x4800000d, 0xfe20003f, N54)
    addOpcode("xor.ob",  "D,S,k",   0x4bc0000d, 0xffe0003f, N54)
    addOpcode("xor.qh",  "X,Y,Q",   0x7820000d, 0xfc20003f, MX)
    addOpcode("xori",    "t,r,i",   0x38000000, 0xfc000000, I1)

    # No hazard protection on coprocessor instructions--they shouldn't
    # change the state of the processor and if they do it's up to the
    # user to put in nops as necessary.  These are at the end so that the
    # disassembler recognizes more specific versions first.
    addOpcode("c0",      "C",   0x42000000, 0xfe000000, I1)
    addOpcode("c1",      "C",   0x46000000, 0xfe000000, I1)
    addOpcode("c2",      "C",   0x4a000000, 0xfe000000, I1)
    addOpcode("c3",      "C",   0x4e000000, 0xfe000000, I1)

    # Conflicts with the 4650's "mul" instruction.  Nobody's using the
    # 4010 any more, so move this insn out of the way.  If the object
    # format gave us more info, we could do this right.
    addOpcode("addciu",  "t,r,j",   0x70000000, 0xfc000000, L1)

def decodeArgs(instruction, args, pc):
    result = ""
    b26 = getBits(instruction, 31, 26)
    b21 = getBits(instruction, 25, 21)
    b16 = getBits(instruction, 20, 16)
    b11 = getBits(instruction, 15, 11)
    b6  = getBits(instruction, 10,  6)
    b0  = getBits(instruction,  5,  0)
    im  = getBits(instruction, 15,  0)

    loc = 0
    while loc < len(args):
        c = args[loc]
        loc += 1
        if c in [',', '(', ')', '[', ']']:
            result += c
        elif c == '+':
            d = args[loc]
            loc += 1
            if d == 'A':
                lsb = (instruction >> OP_SH_SHAMT) & OP_MASK_SHAMT
                result += hex(lsb)
            elif d == 'B':
                msb = (instruction >> OP_SH_INSMSB) & OP_MASK_INSMSB
                result += hex(msb - lsb + 1)
            elif d in ['C', 'H']:
                msbd = (instruction >> OP_SH_EXTMSBD) & OP_MASK_EXTMSBD
                result += hex(msbd + 1)
            elif d == 'D':
                cp0reg = (instruction >> OP_SH_RD) & OP_MASK_RD
                sel = (instruction >> OP_SH_SEL) & OP_MASK_SEL
                # CP0 register including 'sel' code for mtcN (et al.), to be
                # printed textually if known.  If not known, print both
                # CP0 register name and sel numerically since CP0 register
                # with sel 0 may have a name unrelated to register being
                # printed.
                # FIXME names
                result += "$" + str(cp0reg) + "," + str(sel)
            elif d == 'E':
                lsb = ((instruction >> OP_SH_SHAMT) & OP_MASK_SHAMT) + 32
                result += hex(lsb)
            elif d == 'F':
                msb = ((instruction >> OP_SH_INSMSB) & OP_MASK_INSMSB) + 32
                result += hex(msb - lsb + 1)
            elif d == 'G':
                msbd = ((instruction >> OP_SH_EXTMSBD) & OP_MASK_EXTMSBD) + 32
                result += hex(msbd + 1)
            else:
                raise Exception("Unsupported + modifier: +" + d)
        elif c in ['s', 'b', 'r', 'v']:
            result += REGISTER_NAMES[(instruction >> OP_SH_RS) & OP_MASK_RS]
        elif c in ['t', 'w']:
            result += REGISTER_NAMES[(instruction >> OP_SH_RT) & OP_MASK_RT]
        elif c in ['i', 'u']:
            result += hex((instruction >> OP_SH_IMMEDIATE) & OP_MASK_IMMEDIATE)
        elif c in ['j', 'o']:
            delta = (instruction >> OP_SH_DELTA) & OP_MASK_DELTA
            if (delta & 0x8000):
                delta |= ~0xffff
            result += str(delta)
        elif c in ['h']:
            result += hex(((instruction >> OP_SH_PREFX) & OP_MASK_PREFX))
        elif c in ['k']:
            result += hex(((instruction >> OP_SH_CACHE) & OP_MASK_CACHE))
        elif c in ['a']:
            address = (((pc + 4) & ~0x0fffffff) | (((instruction >> OP_SH_TARGET) & OP_MASK_TARGET) << 2))
            result += "0x" + toHex(address, 16)
        elif c in ['p']:
            # Sign extend the displacement.
            delta = (instruction >> OP_SH_DELTA) & OP_MASK_DELTA
            if (delta & 0x8000):
                delta = delta - 65536
            address = (delta<<2) + pc + 4;
            result += "0x" + toHex(address, 16)
        elif c in ['d']:
            result += REGISTER_NAMES[(instruction >> OP_SH_RD) & OP_MASK_RD]
        elif c in ['U']:
            # First check for both rd and rt being equal.
            reg = (instruction >> OP_SH_RD) & OP_MASK_RD;
            if (reg == ((instruction >> OP_SH_RT) & OP_MASK_RT)):
                result += REGISTER_NAMES[reg]
            else:
                # If one is zero use the other.
                if (reg == 0):
                    result += REGISTER_NAMES[(instruction >> OP_SH_RT) & OP_MASK_RT]
                elif (((instruction >> OP_SH_RT) & OP_MASK_RT) == 0):
                    result += REGISTER_NAMES[reg]
                else:
                    # Bogus, result depends on processor.
                    result += REGISTER_NAMES[reg] + "or " + REGISTER_NAMES[(instruction >> OP_SH_RT) & OP_MASK_RT]
        elif c in ['z']:
            result += REGISTER_NAMES[0]
        elif c in ['<']:
            result += hex((instruction >> OP_SH_SHAMT) & OP_MASK_SHAMT)
        elif c in ['^']:
            result += hex((instruction >> OP_SH_BITIND) & OP_MASK_BITIND)
        elif c in ['c']:
            result += hex((instruction >> OP_SH_CODE) & OP_MASK_CODE)
        elif c in ['q']:
            result += hex((instruction >> OP_SH_CODE2) & OP_MASK_CODE2)
        elif c in ['y']:
            ttt = (instruction >> OP_SH_CODE2) & OP_MASK_CODE2
            ttt <<= 22
            ttt >>= 22
            result += str(ttt)
        elif c in ['C']:
            result += hex((instruction >> OP_SH_COPZ) & OP_MASK_COPZ)
        elif c in ['B']:
            result += hex((instruction >> OP_SH_CODE20) & OP_MASK_CODE20)
        elif c in ['J']:
            result += hex((instruction >> OP_SH_CODE19) & OP_MASK_CODE19)
        elif c in ['S', 'V']:
            result += "f" + str((instruction >> OP_SH_FS) & OP_MASK_FS)
        elif c in ['T', 'W']:
            result += "f" + str((instruction >> OP_SH_FT) & OP_MASK_FT)
        elif c in ['D']:
            result += "f" + str((instruction >> OP_SH_FD) & OP_MASK_FD)
        elif c in ['R']:
            result += "f" + str((instruction >> OP_SH_FR) & OP_MASK_FR)
        elif c in ['E']:
            # Coprocessor register for lwcN instructions, et al.
            #
            # Note that there is no load/store cp0 instructions, and
            # that FPU (cp1) instructions disassemble this field using
            # 'T' format.  Therefore, until we gain understanding of
            # cp2 register names, we can simply print the register
            # numbers.
            result += "%" + str((instruction >> OP_SH_RT) & OP_MASK_RT)
        elif c in ['G']:
            # Coprocessor register for mtcN instructions, et al.  Note
            # that FPU (cp1) instructions disassemble this field using
            # 'S' format.  Therefore, we only need to worry about cp0,
            # cp2, and cp3.
            op = (instruction >> OP_SH_OP) & OP_MASK_OP
            if (op == OP_OP_COP0):
                # FIXME mips_cp0_names
                result += "$" + str((instruction >> OP_SH_RD) & OP_MASK_RD)
            else:
                result += "$" + str((instruction >> OP_SH_RD) & OP_MASK_RD)
        elif c in ['K']:
            # FXIME hwr register names here
            result += "$" + str((instruction >> OP_SH_RD) & OP_MASK_RD)
        elif c in ['N']:
            result += "$fcc" + str((instruction >> OP_SH_BCC) & OP_MASK_BCC)
        elif c in ['M']:
            result += "$fcc" + str((instruction >> OP_SH_CCC) & OP_MASK_CCC)
        elif c in ['P']:
            result += str((instruction >> OP_SH_PERFREG) & OP_MASK_PERFREG)
        elif c in ['e']:
            result += str((instruction >> OP_SH_VECBYTE) & OP_MASK_VECBYTE)
        elif c in ['%']:
            result += str((instruction >> OP_SH_VECALIGN) & OP_MASK_VECALIGN)
        elif c in ['H']:
            result += str((instruction >> OP_SH_SEL) & OP_MASK_SEL)
        elif c in ['O']:
            result += str((instruction >> OP_SH_ALN) & OP_MASK_ALN)
        elif c in ['Q']:
            vsel = (instruction >> OP_SH_VSEL) & OP_MASK_VSEL
            if ((vsel & 0x10) == 0):
                vsel &= 0x0f;
                for fmt in xrange(3):
                    if ((vsel & 1) == 0):
                        break;
                    vsel >>= 1
                result += "$v" + str((instruction >> OP_SH_FT) & OP_MASK_FT) + "[" + str(vsel >> 1) + "]"
            elif ((vsel & 0x08) == 0):
                result += "$v" + str((instruction >> OP_SH_FT) & OP_MASK_FT)
            else:
                result += hex((instruction >> OP_SH_FT) & OP_MASK_FT)
        elif c in ['X']:
            result += "$v" + str((instruction >> OP_SH_FD) & OP_MASK_FD)
        elif c in ['Y']:
            result += "$v" + str((instruction >> OP_SH_FS) & OP_MASK_FS)
        elif c in ['Z']:
            result += "$v" + str((instruction >> OP_SH_FT) & OP_MASK_FT)
        elif c in ['&', '>', 'x']:
            result += c # FIXME
        else:
            raise Exception("Invalid argument format character: " + c)
    return result



def decode(instruction, pc):
    for o in OPCODE_LIST:
        if instruction & o.mask == o.opcode:
            return o.name.ljust(8) + decodeArgs(instruction, o.args, pc)

buildOpcodeTable()
if __name__ == "__main__":
    for arg in sys.argv[1:]:
        print decode(fromHex(arg), 0)

