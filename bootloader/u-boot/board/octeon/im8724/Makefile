#
# (C) Copyright 2003
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
#
include $(TOPDIR)/config.mk

CFLAGS += -I$(TOPDIR)/arch/mips/include/asm/arch-octeon -Wno-unused-value -Wno-unused-but-set-variable -Wno-unused-variable

LIB	= $(obj)lib$(BOARD).o
CSLIB   = $(BOARD)_cs_phys.o
COBJS	= $(BOARD)_board.o

SOBJS	=
COBJS-y	=
SRCS	:= $(SOBJS:.o=.S) $(COBJS:.o=.c) $(COBJS-y:.o=.c)
OBJS	:= $(addprefix $(obj),$(COBJS))
SOBJS	:= $(addprefix $(obj),$(SOBJS))

all:	$(obj).depend $(LIB)

DTB	= $(BOARD).dtb.o
DTBOBJS	:= $(addprefix $(obj),$(DTB))

$(BOARD).dtb.o:	$(BOARD).dtb.S

$(BOARD).dtb.S:	$(BOARD).dtb
	echo '.section .rodata, "a"' > $@
	echo '.balign 8' >> $@
	echo '.global __dtb_begin' >> $@
	echo '__dtb_begin:' >> $@
	echo '.incbin "$<" ' >> $@
	echo '__dtb_end:' >> $@
	echo '.global __dtb_end' >> $@
	echo '.balign 8' >> $@

$(BOARD).dtb: $(BOARD).dts
	$(TOPDIR)/tools/dtc -O dtb -p 1024 -o $@ $<

$(LIB):	$(obj).depend $(OBJS) $(SOBJS) $(DTBOBJS) $(CSLIB)
	$(call cmd_link_o_target, $(OBJS) $(SOBJS) $(DTBOBJS) $(CSLIB))

$(CSLIB):
	cp -f $(BOARD)_cs_phys.lib $(CSLIB)

#########################################################################
include $(SRCTREE)/rules.mk

sinclude $(obj).depend

clean:
	-rm -f $(BOARD).dtb $(BOARD).dtb.S $(BOARD)_cs_phys.o
