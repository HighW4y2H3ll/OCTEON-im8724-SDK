#!/usr/bin/python
# Copyright (c) 2003-2015, Cavium Inc.. All rights reserved.
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
# File version info: $Id: build_data.sh 153709 2017-02-06 00:06:50Z cchavva $
#
# Simple script to make some fake data
#
import os
import sys

def procid(model):
   "returns the string identifying the procid part of model"
   l = model.split("_")
   return "_".join(l[:2])

model = procid(os.environ['OCTEON_MODEL'])
if model == "OCTEON_CN68XX":
   #print "argv [0] = %s len = %d" % (sys.argv[0], len(sys.argv))
   if len(sys.argv) == 1:
        start_port = 2048
        end_port = start_port + 49
        incr = 16
   else:
       if sys.argv[1] == "sgmii2":
           start_port = 2560
           incr = 16
           end_port = start_port + 49
       elif sys.argv[1] == "sgmii3":
           start_port = 2816
           incr = 16
           end_port = start_port + 49
       elif sys.argv[1] == "sgmii4":
           start_port = 3072
           incr = 16
           end_port = start_port + 49
       elif sys.argv[1] == "xaui0":
           start_port = 2112
           incr = 1
           end_port = start_port + 1
       elif sys.argv[1] == "xaui2":
           start_port = 2624
           incr = 1
           end_port = start_port + 1
       elif sys.argv[1] == "xaui3":
           start_port = 2880
           incr = 1
           end_port = start_port + 1
       elif sys.argv[1] == "xaui4":
           start_port = 3136
           incr = 1
           end_port = start_port + 1
       elif sys.argv[1] == "ilk0":
           start_port = 1024
           incr = 1
           end_port = start_port + 9;
       elif sys.argv[1] == "ilk1":
           start_port = 1280
           incr = 1
           end_port = start_port + 9;
       elif sys.argv[1] == "npi":
           start_port = 256
           incr = 1
           end_port = start_port + 4;
       else:
           start_port = 2048
           incr = 16
           end_port = start_port + 49

   port_range = (start_port, end_port)
elif model == "OCTEON_CN78XX" or model == "OCTEON_CN76XX":
   #print "argv [0] = %s len = %d" % (sys.argv[0], len(sys.argv))
   if len(sys.argv) == 1:
        start_port = 2048
        end_port = start_port + 49
        incr = 16
   else:
       if sys.argv[1] == "sgmii1":
           start_port = 2304
           incr = 16
           end_port = start_port + 49
       elif sys.argv[1] == "sgmii2":
           start_port = 2560
           incr = 16
           end_port = start_port + 49
       elif sys.argv[1] == "sgmii3":
           start_port = 2816
           incr = 16
           end_port = start_port + 49
       elif sys.argv[1] == "sgmii4":
           start_port = 3072
           incr = 16
           end_port = start_port + 49
       elif sys.argv[1] == "sgmii5":
           start_port = 3328
           incr = 16
           end_port = start_port + 49
       elif sys.argv[1] == "xaui0":
           start_port = 2048
           incr = 1
           end_port = start_port + 1
       elif sys.argv[1] == "xaui1":
           start_port = 2304
           incr = 1
           end_port = start_port + 1
       elif sys.argv[1] == "xaui2":
           start_port = 2560
           incr = 1
           end_port = start_port + 1
       elif sys.argv[1] == "xaui3":
           start_port = 2816
           incr = 1
           end_port = start_port + 1
       elif sys.argv[1] == "xaui4":
           start_port = 3072
           incr = 1
           end_port = start_port + 1
       elif sys.argv[1] == "xaui5":
           start_port = 3328
           incr = 1
           end_port = start_port + 1
       elif sys.argv[1] == "ilk0":
           start_port = 1024
           incr = 1
           end_port = start_port + 9;
       elif sys.argv[1] == "ilk1":
           start_port = 1280
           incr = 1
           end_port = start_port + 9;
       elif sys.argv[1] == "npi":
           start_port = 256
           incr = 1
           end_port = start_port + 4;
       else:
           start_port = 2048
           incr = 16
           end_port = start_port + 49

   port_range = (start_port, end_port)
elif model == "OCTEON_CN73XX" or model == "OCTEON_CN23XX":
   #print "argv [0] = %s len = %d" % (sys.argv[0], len(sys.argv))
   if len(sys.argv) == 1:
        start_port = 2048
        end_port = start_port + 49
        incr = 16
   else:
       if sys.argv[1] == "sgmii1":
           start_port = 2304
           incr = 16
           end_port = start_port + 49
       elif sys.argv[1] == "sgmii2":
           start_port = 2560
           incr = 16
           end_port = start_port + 49
       elif sys.argv[1] == "xaui0":
           start_port = 2048
           incr = 1
           end_port = start_port + 1
       elif sys.argv[1] == "xaui1":
           start_port = 2304
           incr = 1
           end_port = start_port + 1
       else:
           start_port = 2048
           incr = 16
           end_port = start_port + 49

   port_range = (start_port, end_port)
elif model == "OCTEON_CNF75XX":
   # Use all 4 LMACs as a single interface
   start_port = 2048
   end_port = start_port + 49
   incr = 16
   port_range = (start_port, end_port)
else:
   # Default to GMX1. Simulator defaults interface 0 to PCIe gen2.
   if model == "OCTEON_CN66XX" or model == "OCTEON_CN61XX" or model == "OCTEON_CN70XX" or model == "OCTEON_CN71XX":
      #print "argv [0] = %s len = %d" % (sys.argv[0], len(sys.argv))
      port_range = (0, 4)
      if len(sys.argv) == 1:
          port_range = (0, 4)
      else:
          if sys.argv[1] == "sgmii1":
              port_range = (16, 20)
          elif sys.argv[1] == "xaui0":
              port_range = (0, 1)
          elif sys.argv[1] == "xaui1":
              port_range = (16, 17)
          elif sys.argv[1] == "agl":
              port_range = (24, 25)
   else:
       port_range = (0, 4)

   if len(sys.argv) > 1 and sys.argv[1] == "npi":
       port_range = (32, 36) 

   incr = 1

for port in range(port_range[0], port_range[-1], incr):
    packet = (str(port) + " 233 1\n" +
                str(port) + " 0xffffffffffff0003\n" +
                str(port) + " 0xba12de3d08004500\n" +
                str(port) + " 0x00d7244d40000111\n" +
                str(port) + " 0xb1acc0a810cdc0a8\n" +
                str(port) + " 0x10ff008a008a00c3\n" +
                str(port) + " 0x9322110a10fcc0a8\n" +
                str(port) + " 0x10cd008a00bb0000\n" +
                str(port) + " 0x2046484546464446\n" +
                str(port) + " 0x4543414341434143\n" +
                str(port) + " 0x4143414341434143\n" +
                str(port) + " 0x4143414341434141\n" +
                str(port) + " 0x4100204648455046\n" +
                str(port) + " 0x43454c4548464345\n" +
                str(port) + " 0x5046464641434143\n" +
                str(port) + " 0x4143414341434143\n" +
                str(port) + " 0x41424f00ff534d42\n" +
                str(port) + " 0x2500000000000000\n" +
                str(port) + " 0x0000000000000000\n" +
                str(port) + " 0x0000000000000000\n" +
                str(port) + " 0x0000000011000013\n" +
                str(port) + " 0x0000000000000000\n" +
                str(port) + " 0x0000000000000000\n" +
                str(port) + " 0x0000001300560003\n" +
                str(port) + " 0x0001000100020024\n" +
                str(port) + " 0x005c4d41494c534c\n" +
                str(port) + " 0x4f545c42524f5753\n" +
                str(port) + " 0x45000801070f0114\n" +
                str(port) + " 0x90a1b5bf00000000\n" +
                str(port) + " 0x5745535400c41503\n" +
                str(port) + " 0xfe00000000000000\n")
    out = open("input-" + str(port) + ".data", "w")
    for count in range(0,758*2):
        out.write(packet)
    out.close() 
