/*
 * Copyright 2008 Extreme Engineering Solutions, Inc.
 *
 * Copyright 2015 Cavium, Inc.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __PCA9555_H_
#define __PCA9555_H_

#define PCA9555_IN0		0x00
#define PCA9555_IN1		0x01
#define PCA9555_OUT0		0x02
#define PCA9555_OUT1		0x03
#define PCA9555_POL0		0x04
#define PCA9555_POL1		0x05
#define PCA9555_CONF0		0x06
#define PCA9555_CONF1		0x07

#define PCA9555_OUT_LOW		0
#define PCA9555_OUT_HIGH	1
#define PCA9555_POL_NORMAL	0
#define PCA9555_POL_INVERT	1
#define PCA9555_DIR_OUT		0
#define PCA9555_DIR_IN		1

int pca9555_set_val(u8 bus, u8 chip, uint mask, uint data);
int pca9555_set_pol(u8 bus, u8 chip, uint mask, uint data);
int pca9555_set_dir(u8 bus, u8 chip, uint mask, uint data);
int pca9555_get_val(u8 bus, u8 chip);

#endif /* __PCA9555_H_ */
