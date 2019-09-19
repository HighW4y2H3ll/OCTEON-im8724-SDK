/*
 * Copyright (C) 2014 Cavium, Inc.
 * Copyright 2008 Extreme Engineering Solutions, Inc.
 *
 * Based loosly on the pca853x driver
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */
#ifndef __PCF857X_H__
#define __PCF857X_H__
/* Driver for NXP's pcf857x and related GPIO expander chips */

/**
 * Returns the values from the chip
 *
 * @param bus	I2C bus number
 * @param chip	I2C chip address
 *
 * @return	8 bits of GPIO data read from the pins or -1 on error
 */
int pcf857x_get_val(uint8_t bus, uint8_t chip);

/**
 * Sets values on the output of the chip
 * @param bus	I2C bus number
 * @param chip	I2C chip address
 * @param mask	Mask for the data
 * @param data	Bits to set
 *
 * @return -1 on error, 0 for success
 */
int pcf857x_set_val(uint8_t bus, uint8_t chip, uint mask, uint data);

#endif /* __PCF857X_H__ */
