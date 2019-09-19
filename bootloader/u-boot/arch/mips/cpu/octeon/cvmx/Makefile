#
# (C) Copyright 2003-2006
# Wolfgang Denk, DENX Software Engineering, wd@denx.de.
#
# See file CREDITS for list of people who contributed to this
# project.
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; either version 2 of
# the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston,
# MA 02111-1307 USA
#

include $(TOPDIR)/config.mk

LOCAL_CFLAGS = -I$(TOPDIR)/include/asm/arch -D__U_BOOT__

CFLAGS	+= $(LOCAL_CFLAGS)
CPPFLAGS += $(LOCAL_CFLAGS)
HOSTCFLAGS += $(LOCAL_CFLAGS)

LIB	= $(obj)libcvmx.o

SOBJS-y	+=

COBJS-$(CONFIG_OCTEON) +=	\
	cvmx-bootmem.o		\
	cvmx-boot-vector.o	\
	cvmx-clock.o		\
	cvmx-cmd-queue.o 	\
	cvmx-config-init.o 	\
	cvmx-core.o		\
	cvmx-coremask.o		\
	cvmx-fpa.o		\
	cvmx-fpa-resource.o	\
	cvmx-global-resources.o \
	cvmx-helper.o		\
	cvmx-helper-fdt.o	\
	cvmx-helper-fpa.o 	\
	cvmx-helper-jtag.o	\
	cvmx-interrupt.o	\
	cvmx-l2c.o		\
	cvmx-pcie.o 		\
	cvmx-power-throttle.o	\
	cvmx-profiler.o		\
	cvmx-range.o	        \
	cvmx-sysinfo.o		\
	cvmx-twsi.o		\
	cvmx-warn.o		\
	octeon-feature.o	\
	octeon-model.o		\
	octeon-pci-console.o

ifndef CONFIG_OCTEON_DISABLE_QLM
COBJS-y +=	cvmx-qlm.o		\
		cvmx-qlm-tables.o
endif
ifndef CONFIG_OCTEON_DISABLE_NETWORKING
COBJS-y	+=	cvmx-agl.o		\
		cvmx-fau.o		\
		cvmx-fau-compat.o	\
		cvmx-helper-agl.o 	\
		cvmx-helper-bgx.o	\
		cvmx-helper-board.o	\
		cvmx-helper-cfg.o	\
		cvmx-helper-errata.o	\
		cvmx-helper-ilk.o	\
		cvmx-helper-ipd.o	\
		cvmx-helper-loop.o	\
		cvmx-helper-npi.o 	\
		cvmx-helper-pki.o 	\
		cvmx-helper-pko.o 	\
		cvmx-helper-pko3.o 	\
		cvmx-helper-rgmii.o	\
		cvmx-helper-sfp.o	\
		cvmx-helper-sgmii.o	\
		cvmx-helper-spi.o 	\
		cvmx-helper-srio.o	\
		cvmx-helper-util.o	\
		cvmx-helper-xaui.o	\
		cvmx-ilk.o		\
		cvmx-ipd.o		\
		cvmx-ixf18201.o		\
		cvmx-pki.o		\
		cvmx-pki-resources.o	\
		cvmx-pko.o		\
		cvmx-pko-internal-ports-range.o \
		cvmx-pko3.o		\
		cvmx-pko3-compat.o	\
		cvmx-pko3-queue.o	\
		cvmx-pko3-resources.o	\
		cvmx-spi.o		\
		cvmx-spi4000.o		\
		cvmx-srio.o

endif

COBJS-$(CONFIG_OCTEON_ADMA)		+=	cvmx-adma.o
COBJS-$(CONFIG_CMD_OCTEON_CSR)		+= 	cvmx-csr-db-support.o	\
						cvmx-csr-db.o

COBJS-$(CONFIG_CMD_OCTEON_NAND)		+=	cvmx-nand.o
COBJS-$(CONFIG_OCTEON_HW_BCH)		+=	cvmx-bch.o
COBJS-$(CONFIG_USB_OCTEON)		+=	cvmx-usb.o
COBJS-$(CONFIG_CMD_IDE)			+=	cvmx-compactflash.o
COBJS-$(CONFIG_OCTEON_MGMT_ENET)	+=	cvmx-mgmt-port.o

SRCS	:= $(SOBJS:.o=.S) $(COBJS-y:.o=.c)
OBJS	:= $(addprefix $(obj),$(COBJS-y))

$(LIB):	$(obj).depend $(OBJS)
	$(call cmd_link_o_target, $(OBJS))

#########################################################################

# defines $(obj).depend target
include $(SRCTREE)/rules.mk

sinclude $(obj).depend

#########################################################################
