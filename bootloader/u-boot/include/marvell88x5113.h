/*
 * (C) Copyright 2017 Cavium, Inc. <support@cavium.com>
 *
 * (C) Copyright 2018 Interface Masters
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef MARVELL88X5113_H
#define MARVELL88X5113_H

/* CN88XX family can control max 2 MDIO buses  */
/* First bus will serve the motherboard        */
/* Second bus will serve the module            */

#define CN78XXMDIOBUS0	"mdio-octeon0"
#define CN78XXMDIOBUS1	"mdio-octeon1"
#define CN78XXMDIOBUS2	"mdio-octeon2"
#define CN78XXMDIOBUS3	"mdio-octeon3"

/* There are two 88X5113 devices on 25G module */
#define MDIOADDR0	0x00
#define MDIOADDR1	0x0F

#define	DEVICE1		1
#define	DEVICE4		4
#define	DEVICE31	31

#define	DEVID1		0x1002
#define	DEVID2		0x1003

int cn78xx_mdio_read    ( const char * devname, int phyaddr, int device, int regaddr, u16 * data ) ;
int cn78xx_mdio_write   ( const char * devname, int phyaddr, int device, int regaddr, unsigned short data ) ;
int check_5113_presence ( int ) ;
int initialize_5113     ( int ) ;

#endif

