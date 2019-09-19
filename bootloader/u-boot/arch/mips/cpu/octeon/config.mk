#
# (C) Copyright 2003
# Wolfgang Denk, DENX Software Engineering, <wd@denx.de>
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
CROSS_COMPILE ?= mips64-octeon-linux-
ENDIANESS = -EB

MIPSFLAGS += $(ENDIANESS)

PLATFORM_CPPFLAGS += $(MIPSFLAGS) -mabi=n32 \
			-DCVMX_ENABLE_CSR_ADDRESS_CHECKING=0 \
			-finline-functions -finline-functions-called-once \
			-fearly-inlining \
			-finline-small-functions -fmerge-all-constants \
			-mtune=octeon2 -Os -Wdeclaration-after-statement \
			-std=gnu90 -Wno-long-long -mno-check-zero-division

PLATFORM_LDFLAGS +=
PLATFORM_RELFLAGS += -ffunction-sections -fdata-sections
LDFLAGS_FINAL += --gc-sections
