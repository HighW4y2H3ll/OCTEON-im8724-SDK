This document explains how to configure and build kernel to run little-endian
user space applications on Octeon.

The OCTEON-LINUX-LE rpm has toolchain for little endian. This toolchain is
required for both compiling linux kernel and user space, embedded_rootfs.
The little-endian toolchain is only supported for linux target. The 
little endian is not supported for standalone simple-executive applications.

To use little endian toolchain source the environment with '--little-endian'
option.

  cd OCTEON-SDK
  source env-setup OCTEON_CN63XX --little-endian

This will set OCTEON_LE enviornment variable.

Configure kernel for little endian:

   $ cd linux
   $ make clean
   $ cd kernel/linux
   $ cp ../kernel.config .config
   $ make menuconfig
      Endianness selection (Big Endian) --->
           <X> Little Endian
   $ cd ../../


Also configure embedded_rootfs to build user space with little endian:

   $ cd embedded_rootfs
   $ cp default.config .config
   $ make menuconfig
        Global Options --->
            Endianess selection (Big endian toolchain) --->
                <X> Little endian toolchain
   $ make oldconfig
   $ cd ..


Now build kernel and load and boot on OCTEONII and OCTEONIII eval boards.

To use debian rootfs, build deian kernel and prepare the compact flash
as documented in Octeon Software Development Kit, their is a separate
Debian Squeeze package is available with little-endian user space.


