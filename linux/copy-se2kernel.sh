#!/bin/bash
#
# READ THIS BEFORE MODIFYING THIS FILE:
#
# Please keep the file lists in alphabetical order.
#

if [ -z "$OCTEON_ROOT" ] ; then
    echo "OCTEON_ROOT not set"
    exit 1
fi
se_dir=$OCTEON_ROOT/executive


if [ -n "$2" ] ; then
    kernel_dir=$2
else
    kernel_dir=`pwd`
fi

if [ "$1" = "copy" ] ; then
    action0="rm -f"
    action=cp
    action_desc="Copying"
elif [ "$1" = "diff" ] ; then
    action0=:
    action="diff -u"
    action_desc="Diffing"
elif [ "$1" = "link" ] ; then
    action0="rm -f"
    action="ln -s -f"
    action_desc="Linking"
else
    echo "Usage: copy-se2kernel.sh [copy | diff | link] {kernel-dir}"
    exit 1
fi

echo "$action_desc from $kernel_dir to $se_dir"

(
cd $kernel_dir/arch/mips/include/asm/octeon
echo "Working in asm/octeon"

{ while read file ; do

    case $file in
	cvmx.h)
		echo skip $file
	    ;;
	*)
	    if [ -e $se_dir/$file ] && ! cmp -s $file $se_dir/$file ; then
		$action0 $file
		$action $se_dir/$file .
	    else
		echo skip $file
	    fi
	    ;;
    esac

done } <<EOF
cvmx-address.h
cvmx-agl.h
cvmx-agl-defs.h
cvmx-app-hotplug.h
cvmx-app-init.h
cvmx-app-config.h
cvmx-ase-defs.h
cvmx-asm.h
cvmx-asxx-defs.h
cvmx-atomic.h
cvmx-bch.h
cvmx-bch-defs.h
cvmx-bgx.h
cvmx-bgxx-defs.h
cvmx-bootmem.h
cvmx-ciu2-defs.h
cvmx-ciu3-defs.h
cvmx-ciu-defs.h
cvmx-clock.h
cvmx-cmd-queue.h
cvmx-core.h
cvmx-coremask.h
cvmx-csr-enums.h
cvmx-debug.h
cvmx-dbg-defs.h
cvmx-dma-engine.h
cvmx-dpi-defs.h
cvmx-dtx-defs.h
cvmx-hwfau.h
cvmx-fpa-defs.h
cvmx-fpa1.h
cvmx-fpa3.h
cvmx-fpa.h
cvmx-global-resources.h
cvmx-gmx.h
cvmx-gmxx-defs.h
cvmx-gpio-defs.h
cvmx-gser.h
cvmx-gserx-defs.h
cvmx-helper.h
cvmx-helper-agl.h
cvmx-helper-bgx.h
cvmx-helper-board.h
cvmx-helper-cfg.h
cvmx-helper-errata.h
cvmx-helper-fpa.h
cvmx-helper-ilk.h
cvmx-helper-ipd.h
cvmx-helper-jtag.h
cvmx-helper-loop.h
cvmx-helper-npi.h
cvmx-helper-pki.h
cvmx-helper-pko.h
cvmx-helper-pko3.h
cvmx-helper-rgmii.h
cvmx-helper-sgmii.h
cvmx-helper-spi.h
cvmx-helper-srio.h
cvmx-helper-util.h
cvmx-helper-xaui.h
cvmx-hna-defs.h
cvmx-ila.h
cvmx-ila-defs.h
cvmx-ilk-defs.h
cvmx-ilk.h
cvmx-iob-defs.h
cvmx-iobn-defs.h
cvmx-iobp-defs.h
cvmx-ipd-defs.h
cvmx-ipd.h
cvmx-l2c-defs.h
cvmx-l2c.h
cvmx-l2d-defs.h
cvmx-l2t-defs.h
cvmx-lap.h
cvmx-lapx-defs.h
cvmx-lbk-defs.h
cvmx-led-defs.h
cvmx-lmcx-defs.h
cvmx-lut-defs.h
cvmx-mdio.h
cvmx-mio-defs.h
cvmx-mixx-defs.h
cvmx-mpi-defs.h
cvmx-nand.h
cvmx-ncb-defs.h
cvmx-ndf-defs.h
cvmx-npei-defs.h
cvmx-npi-defs.h
cvmx-ocla.h
cvmx-oclax-defs.h
cvmx-ocx-defs.h
cvmx-osm.h
cvmx-osm-defs.h
cvmx-packet.h
cvmx-pci-defs.h
cvmx-pcie.h
cvmx-pcieepvfx-defs.h
cvmx-pcieepx-defs.h
cvmx-pciercx-defs.h
cvmx-pcsx-defs.h
cvmx-pcsxx-defs.h
cvmx-pdm-defs.h
cvmx-peb-defs.h
cvmx-pemx-defs.h
cvmx-pescx-defs.h
cvmx-pexp-defs.h
cvmx-pip-defs.h
cvmx-pip.h
cvmx-pki.h
cvmx-pki-cluster.h
cvmx-pki-defs.h
cvmx-pki-resources.h
cvmx-pko3.h
cvmx-pko3-queue.h
cvmx-pko3-resources.h
cvmx-pko-defs.h
cvmx-pko-internal-ports-range.h
cvmx-hwpko.h
cvmx-pko3.h
cvmx-pko3-queue.h
cvmx-pko3-resources.h
cvmx-pow-defs.h
cvmx-pow.h
cvmx-pse-defs.h
cvmx-qlm.h
cvmx-range.h
cvmx-rnm-defs.h
cvmx-rst-defs.h
cvmx-sata-defs.h
cvmx-scratch.h
cvmx-sli-defs.h
cvmx-smix-defs.h
cvmx-spemx-defs.h
cvmx-spi.h
cvmx-spinlock.h
cvmx-spxx-defs.h
cvmx-srio.h
cvmx-sriomaintx-defs.h
cvmx-sriox-defs.h
cvmx-srxx-defs.h
cvmx-sso-defs.h
cvmx-stxx-defs.h
cvmx-swap.h
cvmx-sysinfo.h
cvmx-twsi.h
cvmx-uahcx-defs.h
cvmx-uart.h
cvmx-uctlx-defs.h
cvmx-usbcx-defs.h
cvmx-usb.h
cvmx-usbnx-defs.h
cvmx-usbdrdx-defs.h
cvmx-wqe.h
cvmx-xcv-defs.h
octeon-boot-info.h
octeon-feature.h
octeon-model.h
EOF
)

(
cd $kernel_dir/arch/mips/cavium-octeon/executive
echo "Working in cavium-octeon/executive"

{ while read file ; do

    case $file in
	*)
	    if [ -e $se_dir/$file ]  && ! cmp -s $file $se_dir/$file ; then
		$action0 $file
		$action $se_dir/$file .
	    else
		echo skip $file
	    fi
	    ;;
    esac

done } <<EOF
cvmx-agl.c
cvmx-appcfg-transport.c
cvmx-bch.c
cvmx-bootmem.c
cvmx-clock.c
cvmx-cmd-queue.c
cvmx-debug.c
cvmx-debug-handler.S
cvmx-debug-remote.c
cvmx-debug-uart.c
cvmx-dma-engine.c
cvmx-error-trees.c
cvmx-fpa-resource.c
cvmx-global-resources.c
cvmx-helper-board.c
cvmx-helper.c
cvmx-helper-agl.c
cvmx-helper-bgx.c
cvmx-helper-cfg.c
cvmx-helper-errata.c
cvmx-helper-ilk.c
cvmx-helper-ipd.c
cvmx-helper-jtag.c
cvmx-helper-loop.c
cvmx-helper-npi.c
cvmx-helper-pki.c
cvmx-helper-pko.c
cvmx-helper-pko3.c
cvmx-helper-rgmii.c
cvmx-helper-sgmii.c
cvmx-helper-spi.c
cvmx-helper-srio.c
cvmx-helper-util.c
cvmx-helper-xaui.c
cvmx-ila.c
cvmx-ilk.c
cvmx-ipd.c
cvmx-lap.c
cvmx-l2c.c
cvmx-nand.c
cvmx-ocla.c
cvmx-osm.c
cvmx-pcie.c
cvmx-pki3.c
cvmx-pki.c
cvmx-pki-resources.c
cvmx-pko.c
cvmx-pko3.c
cvmx-pko3-resources.c
cvmx-pko3-queue.c
cvmx-pko-internal-ports-range.c
cvmx-pko3.c
cvmx-pko3-queue.c
cvmx-pko3-resources.c
cvmx-qlm.c
cvmx-qlm-tables.c
cvmx-range.c
cvmx-spi4000.c
cvmx-spi.c
cvmx-srio.c
cvmx-sso-resources.c
cvmx-sysinfo.c
cvmx-twsi.c
cvmx-usb.c
cvmx-gser.c
cvmx-bgx.c
octeon-feature.c
octeon-model.c
EOF

)
