#!/bin/sh

assert_good ()
{
   if [ $? -ne 0 ]; then exit 255;fi
}

assert_bad ()
{
   if [ $? -eq 0 ]; then exit 255;fi
}

assert ()
{
   "$@"
   assert_good
}

require_good ()
{
   if [ $? -ne 0 ]; then exit 254;fi
}

require_bad ()
{
   if [ $? -eq 0 ]; then exit 254;fi
}

require ()
{
   "$@"
   require_good
}

unimplemented ()
{
   exit 253
}

pass_if_not()
{
   if ! "$@"; then exit 0;fi
}

get_num_cores()
{
   grep processor /proc/cpuinfo | wc -l
}

have_cn30xx()
{
   grep "system type" /proc/cpuinfo | grep -q "CN30"
}

have_cn5xxx()
{
   grep "system type" /proc/cpuinfo | grep -q "CN5"
}

have_rtc()
{
   grep -e "CN5[0|2|4-7][0-9][0-9]p" -e "CN3[0|1]" /proc/cpuinfo | grep -vq NIC_XLE_4G
}

have_cn63xx()
{
   grep -q "CN63[0-9][0-9]p" /proc/cpuinfo
}
have_cn6xxx()
{
   grep "system type" /proc/cpuinfo | grep -q "CN6"
}

have_cn68xx()
{
   grep -q "CN68[0-9][0-9]p" /proc/cpuinfo
}

have_cn56xx()
{
   grep -q "CN5[4-7][0-9][0-9]p" /proc/cpuinfo
}

have_cn52xx()
{
   grep -q "CN52[0-9][0-9]p" /proc/cpuinfo
}

have_cn50xx()
{
   grep -q "CN50[0-9][0-9]p" /proc/cpuinfo
}

have_cn58xx_pass1()
{
  grep -q "CN58[0-9][0-9]p1\.0" /proc/cpuinfo
}

have_cnf71xx()
{
   grep -q "CNF71[0-9][0-9]p" /proc/cpuinfo
}

have_cn7xxx()
{
   grep "system type" /proc/cpuinfo | grep -q "CN7\|CNF75"
}

have_cn70xx()
{
   grep -q "CN70[0-9][0-9]p" /proc/cpuinfo
}

have_cn78xx()
{
   grep -q "CN78[0-9][0-9]p" /proc/cpuinfo
}

have_cn73xx()
{
   grep -q "CN73[0-9][0-9]p" /proc/cpuinfo
}

have_cavium_pci_device()
{
   grep -q "177d" /proc/bus/pci/devices
   [ $? -ne 1 ]
}

find_bootloader_for_pci_device()
{
   BOOTLOADER=
   device=`grep "177d" /proc/bus/pci/devices | cut -f2`

   if test "x$device" != x ; then
       case $device in 
           177d0004 | 177d0005)
                  BOOTLOADER=/u-boot-octeon_ebt3000.bin
                  ;;
           177d0040)
                  BOOTLOADER=/u-boot-octeon_ebt5800.bin
                  ;;
	   177d0050)
		  BOOTLOADER=/u-boot-octeon_nic_xle_4g.bin
		  ;;
	   177d0080)
		  BOOTLOADER=/u-boot-octeon_ebt5200.bin
		  ;;
	   177d1001)
		  BOOTLOADER=/u-boot-octeon_thunder.bin
		  ;;
           177d0090)
                  BOOTLOADER=/u-boot-octeon_ebb6300.bin
                  ;;
        esac
   fi

   echo $BOOTLOADER
}

# When kernel is booted with mem=0, their isn't enough memory to run 
# simple-exec apps.
check_memory()
{
  test `free | grep "Mem:" | awk -F"      " '{print $2}'` -lt 640000 || exit 254
}
  
# When kernel is booted with large shared memory(1536 KB), their isn't enough
# memory to run simple-exec apps built for n32 abi.
check_large_shared_mem()
{
  require grep -q 32bit_shared_mem_size: /proc/octeon_info
  let T=`grep 32bit_shared_mem_size: /proc/octeon_info | awk '{print $2}'`
  test $T -ge 1610612736 && exit 254
}

# When kernel is booted with 64k pagesize, their isn't enough memory 
# to run hugetlb tests. (Hugepage size is greater than 128MB).
check_pagesize()
{
  require grep -q Hugepagesize: /proc/meminfo
  test `grep Hugepagesize: /proc/meminfo | awk '{print $2}'` -lt 131073 || exit 254
}
