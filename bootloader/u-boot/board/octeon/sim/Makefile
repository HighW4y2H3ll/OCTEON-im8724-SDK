#
# (C) Copyright 2003-2015
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

CPPFLAGS += -I$(TOPDIR)/include/asm/arch

LIB	= $(obj)lib$(BOARD).o 

COBJS	= $(BOARD)_board.o
SOBJS	=

SRCS	:= $(SOBJS:.o=.S) $(COBJS:.o=.c) $(COBJS-y:.o=.c)
OBJS	:= $(addprefix $(obj),$(COBJS))
SOBJS	:= $(addprefix $(obj),$(SOBJS))

all:	$(obj).depend $(LIB)

DTB	= 3xxx.dtb.o 68xx.dtb.o 78xx.dtb.o 73xx.dtb.o 75xx.dtb.o
DTBOBJS := $(addprefix $(obj),$(DTB))

%.dtb.o: %.dtb.S

3xxx.dtb.S: 3xxx.dtb
	echo '.section .rodata, "a"' > $@
	echo '.balign 8' >> $@
	echo '.global __dtb_begin' >> $@
	echo '__dtb_begin:' >> $@
	echo '.incbin "$<" ' >> $@
	echo '__dtb_end:' >> $@
	echo '.global __dtb_end' >> $@
	echo '.balign 8' >> $@

68xx.dtb.S: 68xx.dtb
	echo '.section .rodata, "a"' > $@
	echo '.balign 8' >> $@
	echo '.global __dtb_68xx_begin' >> $@
	echo '__dtb_68xx_begin:' >> $@
	echo '.incbin "$<" ' >> $@
	echo '__dtb_68xx_end:' >> $@
	echo '.global __dtb_68xx_end' >> $@
	echo '.balign 8' >> $@

78xx.dtb.S: 78xx.dtb
	echo '.section .rodata, "a"' > $@
	echo '.balign 8' >> $@
	echo '.global __dtb_78xx_begin' >> $@
	echo '__dtb_78xx_begin:' >> $@
	echo '.incbin "$<" ' >> $@
	echo '__dtb_78xx_end:' >> $@
	echo '.global __dtb_78xx_end' >> $@
	echo '.balign 8' >> $@

73xx.dtb.S: 73xx.dtb
	echo '.section .rodata, "a"' > $@
	echo '.balign 8' >> $@
	echo '.global __dtb_73xx_begin' >> $@
	echo '__dtb_73xx_begin:' >> $@
	echo '.incbin "$<" ' >> $@
	echo '__dtb_73xx_end:' >> $@
	echo '.global __dtb_73xx_end' >> $@
	echo '.balign 8' >> $@

75xx.dtb.S: 75xx.dtb
	echo '.section .rodata, "a"' > $@
	echo '.balign 8' >> $@
	echo '.global __dtb_75xx_begin' >> $@
	echo '__dtb_75xx_begin:' >> $@
	echo '.incbin "$<" ' >> $@
	echo '__dtb_75xx_end:' >> $@
	echo '.global __dtb_75xx_end' >> $@
	echo '.balign 8' >> $@

%.dtb: %.dts
	$(TOPDIR)/tools/dtc -f -O dtb -p 1024 -o $@ $<

$(LIB):	$(obj).depend $(OBJS) $(SOBJS) $(DTBOBJS)
	$(call cmd_link_o_target, $(OBJS) $(SOBJS) $(DTBOBJS))

#########################################################################

#.depend:	Makefile $(SOBJS:.o=.S) $(OBJS:.o=.c)
#		$(CC) -M $(CFLAGS) $(SOBJS:.o=.S) $(OBJS:.o=.c) > $@
include $(SRCTREE)/rules.mk
sinclude $(obj).depend

#########################################################################

clean:
	-rm -f $(BOARD).dtb $(BOARD).dtb.S
