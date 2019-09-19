/*
 * Copyright 2008 Extreme Engineering Solutions, Inc.
 *
 * Copyright 2015 Cavium, Inc.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __PCA9554_H_
#define __PCA9554_H_

#define PCA9554_IN0		0x00
#define PCA9554_OUT0		0x01
#define PCA9554_POL0		0x02
#define PCA9554_CONF0		0x03

#define PCA9554_OUT_LOW		0
#define PCA9554_OUT_HIGH	1
#define PCA9554_POL_NORMAL	0
#define PCA9554_POL_INVERT	1
#define PCA9554_DIR_OUT		0
#define PCA9554_DIR_IN		1

int pca9554_set_val(u8 bus, u8 chip, uint mask, uint data);
int pca9554_set_pol(u8 bus, u8 chip, uint mask, uint data);
int pca9554_set_dir(u8 bus, u8 chip, uint mask, uint data);
int pca9554_get_val(u8 bus, u8 chip);

#endif /* __PCA9554_H_ */
