#
# (C) Copyright 2003-2006
# Wolfgang Denk, DENX Software Engineering, wd@denx.de.
#
# (C) Copyright 2011
# Cavium Inc., Inc. <support@cavium.com>
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

ifneq ($(OBJTREE),$(SRCTREE))
$(shell mkdir -p $(obj)cvmx)
$(shell mkdir -p $(obj)commands)
endif

CPPFLAGS	+= -I$(SRCTREE)/include/asm/arch -D__U_BOOT__
CFLAGS		+= -I$(SRCTREE)/include/asm/arch -D__U_BOOT__

LIB	= $(obj)lib$(CPU).o

START	= start.o

SOBJS-y	=
COBJS-y	= 	cpu.o dfm.o interrupts.o lib_octeon.o lib_octeon_shared.o \
		memset.o memcmp.o memcpy.o octeon_bist.o octeon_boot.o \
		octeon_console.o \
		octeon_crc32.o octeon_dram.o octeon_env.o octeon_errata.o \
		octeon_exec.o timer.o

COBJS-$(CONFIG_OF_LIBFDT)		+= octeon_fdt.o
COBJS-$(CONFIG_OCTEON_PCI)		+= octeon_pci.o octeon_pcie.o
COBJS-$(CONFIG_CMD_OCTEON)		+= commands/cmd_octeon.o
COBJS-$(CONFIG_CMD_OCTEON_ELF)		+= commands/cmd_elf64.o
COBJS-$(CONFIG_CMD_OCTEON_BOOTLOADER_UPDATE) += \
					   commands/cmd_octeon_bootloader.o
COBJS-$(CONFIG_CMD_OCTEON_REGINFO)	+= commands/cmd_octeon_reginfo.o
COBJS-$(CONFIG_CMD_OCTEON_MEM)		+= commands/cmd_octeon_mem.o \
					   commands/cmd_octeon_rw.o \
					   commands/cmd_mem64.o
COBJS-$(CONFIG_CMD_OCTEON_LINUX)	+= commands/cmd_octeon_linux.o
COBJS-$(CONFIG_OCTEON_FLASH)		+= octeon_flash.o
COBJS-$(CONFIG_CMD_OCTEON_CSR)		+= commands/cmd_octeon_csr.o
COBJS-$(CONFIG_CMD_OCTEON_ERASEENV)	+= commands/cmd_octeon_eraseenv.o
COBJS-$(CONFIG_CMD_OCTEON_NAND)		+= commands/cmd_octeon_nand.o
COBJS-$(CONFIG_CMD_OCTEON_NVME)		+= commands/cmd_octeon_nvme.o
COBJS-$(CONFIG_CMD_OCTEON_TLVEEPROM)	+= commands/cmd_octeon_tlveeprom.o
COBJS-$(CONFIG_CMD_QLM)			+= commands/cmd_qlm.o
COBJS-$(CONFIG_CMD_IDE)			+= octeon_cf.o
COBJS-$(CONFIG_CMD_OCTEON_NAND_BOOT_STAGE3) \
					+= commands/cmd_octeon_nand_boot_stage3.o
COBJS-$(CONFIG_CMD_OCTEON_BOOTBUS)	+= commands/cmd_octeon_bootbus.o
COBJS-$(CONFIG_CMD_OCTEON_VPD)		+= commands/cmd_octeon_vpd.o
COBJS-$(CONFIG_HW_WATCHDOG)		+= octeon_wd.o \
					   commands/cmd_octeon_wd.o
COBJS-$(CONFIG_OCTEON_MD5)		+= octeon_md5.o
COBJS-$(CONFIG_CMD_NET)			+= commands/cmd_octeon_tftp.o
COBJS-$(CONFIG_OCTEON_SHA1)		+= octeon_sha1.o
COBJS-$(CONFIG_OCTEON_SHA256)		+= octeon_sha256.o
COBJS-$(CONFIG_SYS_PCI_CONSOLE)		+= octeon_pci_console.o
COBJS-$(CONFIG_CMD_OCTEON_BOOT_STAGE3)	+= commands/cmd_octeon_boot_stage3.o
COBJS-$(CONFIG_CMD_OCTEON_MII45)	+= commands/cmd_mii45.o
COBJS-$(CONFIG_OCTEON_QLM)		+= octeon_qlm.o
COBJS-$(CONFIG_OCTEON_OCX)		+= octeon3_ocx.o octeon3_ocx_train.o
COBJS-$(CONFIG_OCTEON_SE_NAPI)		+= octeon_seapi.o

SRCS	:= $(START:.o=.S) $(SOBJS-y:.o=.S) $(COBJS-y:.o=.c)
OBJS	:= $(addprefix $(obj),$(SOBJS-y) $(COBJS-y))
START	:= $(addprefix $(obj),$(START))

all:	$(obj).depend $(START) $(LIB)

$(LIB):	$(OBJS)
	$(call cmd_link_o_target, $(OBJS))

#########################################################################

# defines $(obj).depend target
include $(SRCTREE)/rules.mk

sinclude $(obj).depend

#########################################################################
