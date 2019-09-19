oct-top - Simple Executive top utility
          This utility displays frequently called routines and associated no. of hits 
          of Linux/SE application in sorted order 

Usage: 
--------
oct-top [--line] [--address] [--perf1=counter] [--l2perf1=L2counter] [--dwell=<seconds>]
        [--core_mask=mask] [--cpu=coreno]  <elffile>"

<elf file>             : Mips ELF file to load debugging information from."
--line                 : Show line number information."
--address              : Show exact PC addresses."
--perf1=<Core counter> : Setup Core performance counter 1"
--perf2=<Core counter> : Setup Core performance counter 2"
--l2perf1=<L2 counter> : Setup L2 performance counter 1"
--l2perf2=<L2 counter> : Setup L2 performance counter 2"
--l2perf3=<L2 counter> : Setup L2 performance counter 3"
--l2perf4=<L2 counter> : Setup L2 performance counter 4"
--dwell=<seconds>      : Number of seconds to refresh display"
--core_mask=<mask>     : Mask which specifies the cores to sample"
--cpu=<core no>        : Per cpu 'top' information, [0-n cores]"


Note: 
o On embedded rootfs, set CONFIG_python=y, CONFIG_octeon-top=y in default.config to have oct-top compiled in 
  to the kernel. Also set CONFIG_oct-app-ctl=y to enable oct-app-boot utility, which is needed to load SE Applications 
  on other cores.

o Also elf file passed to oct-top shouldn't be stripped of Debug info.

Caveats:
---------
o When invoked with elf file, passing --core_mask is highly recommended as we may have cores running
  Linux and SE apps at same time and resolving symbols would be a problem in this case and may lead 
  to significant unresolved symbols.

eg:

   (i) oct-app-boot -mask=0x6 named-block (SE booted on 1,2cores)
       oct-top named-block --core_mask=0x6 (This would profile only cores 1-2 running SE)

  (ii) oct-app-boot -mask=0x30 named-block (SE booted on 4,5 cores)
       oct-top (Display PC values for all cores sorted in descending order)
       oct-top named-block --core_mask=0x30 --cpu=4 (Display top information for core 4)
       oct-top named-block --core_mask=0x30 --cpu=5 --dwell=1 
	(Display top info for last core and prints for every 1 sec)

