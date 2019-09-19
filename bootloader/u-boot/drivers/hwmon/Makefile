#
# (C) Copyright 2006
# Wolfgang Denk, DENX Software Engineering, wd@denx.de.
#
# (C) Copyright 2001
# Erik Theisen, Wave 7 Optics, etheisen@mindspring.com.
#
# SPDX-License-Identifier:	GPL-2.0+
#

include $(TOPDIR)/config.mk

#CFLAGS += -DDEBUG

LIB	= $(obj)libhwmon.o

COBJS-$(CONFIG_DTT_ADM1021) += adm1021.o
COBJS-$(CONFIG_DTT_ADT7460) += adt7460.o
COBJS-$(CONFIG_DTT_DS1621) += ds1621.o
COBJS-$(CONFIG_DTT_DS1722) += ds1722.o
COBJS-$(CONFIG_DTT_DS1775) += ds1775.o
COBJS-$(CONFIG_DTT_LM63) += lm63.o
COBJS-$(CONFIG_DTT_LM73) += lm73.o
COBJS-$(CONFIG_DTT_LM75) += lm75.o
COBJS-$(CONFIG_DTT_LM81) += lm81.o
COBJS-$(CONFIG_DTT_LM83) += lm83.o
COBJS-$(CONFIG_DTT_TMP42X) += tmp42x.o
COBJS-$(CONFIG_DTT_SAA56004X) += saa56004x.o

COBJS	:= $(COBJS-y)
SRCS	:= $(COBJS:.o=.c)
OBJS	:= $(addprefix $(obj),$(COBJS))

all:	$(LIB)

$(LIB):	$(obj).depend $(OBJS)
	$(call cmd_link_o_target, $(OBJS))

#########################################################################

# defines $(obj).depend target
include $(SRCTREE)/rules.mk

sinclude $(obj).depend

#########################################################################
