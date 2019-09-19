/***********************license start***************
 * Copyright (c) 2016 Cavium Inc. (support@cavium.com). All rights
 * reserved.
 *
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.

 *   * Neither the name of Cavium Inc. nor the names of
 *     its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written
 *     permission.

 * This Software, including technical data, may be subject to U.S. export  control
 * laws, including the U.S. Export Administration Act and its  associated
 * regulations, and may be subject to export or import  regulations in other
 * countries.

 * TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
 * AND WITH ALL FAULTS AND CAVIUM INC. MAKES NO PROMISES, REPRESENTATIONS OR
 * WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH RESPECT TO
 * THE SOFTWARE, INCLUDING ITS CONDITION, ITS CONFORMITY TO ANY REPRESENTATION OR
 * DESCRIPTION, OR THE EXISTENCE OF ANY LATENT OR PATENT DEFECTS, AND CAVIUM
 * SPECIFICALLY DISCLAIMS ALL IMPLIED (IF ANY) WARRANTIES OF TITLE,
 * MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, LACK OF
 * VIRUSES, ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR
 * CORRESPONDENCE TO DESCRIPTION. THE ENTIRE  RISK ARISING OUT OF USE OR
 * PERFORMANCE OF THE SOFTWARE LIES WITH YOU.
 ***********************license end**************************************/

/**
 * @file
 *
 * Functions to capture eye diagram and print the diagram.
 *
 * <hr>$Id$<hr>
 */

#include <stdint.h>
#include <math.h>
#include <cvmx.h>
#include <cvmx-qlm.h>
#include <cvmx-gserx-defs.h>

/* Overview
 * The eye capture hardware provides a window that "rotates" to find the eye.
 * The time axis (X) ranges from 0-127.  When the X axis location changes
 * from 127 to 0, the windows is rotated to the next position.  There are a
 * total of 20 rotations before we repeat.  The voltage axis (Y) ranges from
 * -31 to 31, where the eye should be centered at 0.  According to the very
 * meager documentation, moving the eye sample location should be done one
 * location at a time to prevent loss of sync.  We scan the eye out by
 * traversing time (X) while sweeping the full voltage range (Y).  First we
 * sweep min Y to max Y, then we step X one, and scan max Y to min Y.  This
 * requires the least movements.  Before any of this scanning can be done, we
 * have to find the right window rotation by search at Y=0 for a sequence of
 * zero errors.
 */

#define EYE_MAX_ROTATIONS    20 /* Number of "rotations" available in the hardware */
#define EYE_MAX_X	    128 /* Maximum X location (time axis) */
#define EYE_MIN_Y	    -31 /* Minimum Y location (voltage axis) */
#define EYE_MAX_Y	     31 /* Maximum Y location (voltage axis) */

/**
 * Get the error counter for the current sample location
 *
 * @param node   Node to access
 * @param qlm    QLM to access
 * @param lane   Lane to access
 *
 * @return Number of errors
 */
static uint32_t cvmx_geterrorcount(int node, int qlm, int lane)
{
	uint32_t ErrorCount = 0;  // Resulting Error Counts
	uint32_t E[4];            // catch the 16 bit results
	int x;

	for (x = 0; x < 4; x++) {
		/*
		* Set in register BDK_GSERX_LANEX_RX_CDR_MISC_CTRL_0[21:20] to select
		* the RT-Eyemon bit counter
		*/
		uint64_t reg = cvmx_read_csr_node(node, CVMX_GSERX_LANEX_RX_CDR_MISC_CTRL_0(lane, qlm));
		/* clear the bit field   (bit 20 - bit 16 = bit 4) */
		reg &= ~(0x03 << 4);
		/*  mask in the register Index; */
		reg |= (x << 4);
		cvmx_write_csr_node(node, CVMX_GSERX_LANEX_RX_CDR_MISC_CTRL_0(lane, qlm), reg);
		cvmx_read_csr_node(node, CVMX_GSERX_LANEX_RX_CDR_MISC_CTRL_0(lane, qlm));
		/* read the status register */
		E[x] = cvmx_read_csr_node(node, CVMX_GSERX_LANEX_RX_CDR_STATUS_1(lane, qlm));
		ErrorCount |= (((E[x] >> 7) & 0xff) << (x * 8));
	}
	return ErrorCount;
}

/**
 * Enable or disable the eye error counter
 *
 * @param node   Node to access
 * @param qlm    QLM to access
 * @param lane   Lane to access
 * @param enable 1=Enable error counter, 0=disable
 */
static void cvmx_errorcounterenable(int node, int qlm, int lane, int enable)
{
	/* Set in register BDK_GSERX_LANEX_RX_CDR_MISC_CTRL_0 [7] to enable */
	const uint64_t EYE_MONITOR_COUNTER_ENABLE = 0x01 << 7;
	cvmx_gserx_lanex_rx_cdr_misc_ctrl_0_t cdr;

	cdr.u64 = cvmx_read_csr_node(node, CVMX_GSERX_LANEX_RX_CDR_MISC_CTRL_0(lane, qlm));
	    if (enable)
	        cdr.u64 |= EYE_MONITOR_COUNTER_ENABLE;
	    else
	        cdr.u64 &= ~EYE_MONITOR_COUNTER_ENABLE;
	cvmx_write_csr_node(node, CVMX_GSERX_LANEX_RX_CDR_MISC_CTRL_0(lane, qlm), cdr.u64);
}

/**
 * Enable or disable the eye monitor
 *
 * @param node   Node to access
 * @param qlm    QLM to access
 * @param lane   Lane to access
 * @param enable 1=enable, 0=disable
 */
static void cvmx_rtenable(int node, int qlm, int lane, int enable)
{
	cvmx_gserx_lanex_rx_cfg_0_t cfg;

	cfg.u64 = cvmx_read_csr_node(node, CVMX_GSERX_LANEX_RX_CFG_0(lane, qlm));
	cfg.s.pcs_sds_rx_eyemon_en = enable;
	cvmx_write_csr_node(node, CVMX_GSERX_LANEX_RX_CFG_0(lane, qlm), cfg.u64);
}

/**
 * Enable or disable the eye shadow PIV
 *
 * @param node   Node to access
 * @param qlm    QLM to access
 * @param lane   Lane to access
 * @param enable 1=enable, 0=disable
 */
static void cvmx_shadowpivenable(int node, int qlm, int lane, int enable)
{
	/* Set in register BDK_GSERX_LANEX_RX_CDR_MISC_CTRL_0[6] to enable the Shadow PI Clock */
	const uint64_t EYE_MONITOR_SHADOW_PI_CONTROL_ENABLE = 0x01 << 6;
	cvmx_gserx_lanex_rx_cdr_misc_ctrl_0_t cdr;

	cdr.u64 = cvmx_read_csr_node(node, CVMX_GSERX_LANEX_RX_CDR_MISC_CTRL_0(lane, qlm));
	if (enable)
		cdr.u64 |= EYE_MONITOR_SHADOW_PI_CONTROL_ENABLE;
	else
		cdr.u64 &= ~EYE_MONITOR_SHADOW_PI_CONTROL_ENABLE;
	cvmx_write_csr_node(node, CVMX_GSERX_LANEX_RX_CDR_MISC_CTRL_0(lane, qlm), cdr.u64);
}

/**
 * Move the eye sample location. The hardware may lose lock if we move the shadow
 * sampler in large steps. This code reads the current location and moves the
 * sampler one step at a time. Note that if told to move to 128 (MAX_X), we will
 * move up to 127 and then jump to 0, causing a rotation.
 *
 * @param node   Node to access
 * @param qlm    QLM to access
 * @param lane   Lane to access
 * @param x      Location along the X axis (time) (0-127, or 128)
 * @param y      Location along Y axis (-31,31)
 */
static void cvmx_eye_move_location(int node, int qlm, int lane, int x, int y)
{
	/* Time (us) to settle between location movements (Default 50us) */
	const int SETTLE_TIME = 50;
	/* Extract the current Y location from DWC_RX_CFG_4[13:8] */
	uint64_t reg = cvmx_read_csr_node(node, CVMX_GSERX_LANEX_RX_CFG_4(lane, qlm));
	int current_y, current_x, step_x, step_y;

	current_y = cvmx_bit_extract(reg, 8, 6);
	if (current_y & 0x20) {
		/* Convert negative */
		current_y &= 0x1f;
		current_y = -current_y;
	}

	/* Calculate step direction. The hardware may lose sync if we step by more
	   than one */
	step_y = (y > current_y) ? 1 : -1;

	while (current_y != y) {
		int tmp_y;
		current_y += step_y;
		tmp_y = current_y;
		/* Convert negative */
		if (current_y < 0)
			tmp_y = -current_y | 0x20;
		reg = cvmx_read_csr_node(node, CVMX_GSERX_LANEX_RX_CFG_4(lane, qlm));
		reg = cvmx_bit_insert(reg, tmp_y, 8, 6);
		cvmx_write_csr_node(node, CVMX_GSERX_LANEX_RX_CFG_4(lane, qlm), reg);
		cvmx_wait_usec(SETTLE_TIME);
	}

	/* Extract the current X location */
	reg = cvmx_read_csr_node(node, CVMX_GSERX_LANEX_RX_CDR_CTRL_2(lane, qlm));
	current_x = cvmx_bit_extract(reg, 8, 7);

	/* Calculate step direction. The hardware may lose sync if we step by more
	   than one */
	step_x = (x > current_x) ? 1 : -1;

	while (current_x != x) {
		current_x += step_x;
		reg = cvmx_read_csr_node(node, CVMX_GSERX_LANEX_RX_CDR_CTRL_2(lane, qlm));
		reg = cvmx_bit_insert(reg, current_x, 8, 7);
		cvmx_write_csr_node(node, CVMX_GSERX_LANEX_RX_CDR_CTRL_2(lane, qlm), reg);
		cvmx_wait_usec(SETTLE_TIME);
	}
}

/**
 * Move the eye sample location and measure the errors at that location. See
 * eye_move_location() for more detail about the moving of the sample location as
 * this function wraps eye_move_location().
 *
 * @param node   Node to access
 * @param qlm    QLM to access
 * @param lane   Lane to access
 * @param x      Location along the X axis (time) (0-127, or 128)
 * @param y      Location along Y axis (-31,31)
 *
 * @return Number of errors
 */
static uint32_t cvmx_eye_measure_errors(int node, int qlm, int lane, int x, int y)
{
	/* Time (us) to count errors at each location (default 400us) */
	const int SAMPLE_TIME = 400;
	uint32_t errors;

	cvmx_eye_move_location(node, qlm, lane, x, y);
	cvmx_errorcounterenable(node, qlm, lane, 1);

	cvmx_wait_usec(SAMPLE_TIME);

	errors = cvmx_geterrorcount(node, qlm, lane);
	cvmx_errorcounterenable(node, qlm, lane, 0);
	return errors;
}

/**
 * Capture an eye diagram
 *
 * @param node     Node to access
 * @param qlm      QLM to access
 * @param lane     Lane to access
 * @param eye_data Location where the eye data will be stored
 *
 * @return Zero on success, negative on failure
 */
int __cvmx_qlm_eye_capture(int node, int qlm, int lane, cvmx_qlm_eye_t *eye_data)
{
	uint64_t reg;
	const uint64_t Q_QB_ERR = 0x3f << 8;
	uint64_t Q_QB_ES_Original;
	/* Number of consecutive zeros that signals an eye (default 2) */
	const int NUM_ZEROS = 2;
	int rotation, zero_location, x;

	/* Eye capture is supported only on cn78xx and new models. */
	if ((OCTEON_IS_MODEL(OCTEON_CN3XXX)) ||
		(OCTEON_IS_MODEL(OCTEON_CN5XXX)) ||
		(OCTEON_IS_MODEL(OCTEON_CN6XXX)) ||
		(OCTEON_IS_MODEL(OCTEON_CN70XX)))
		return -1;

	/*  this errata removes the read timeout.
	    BUG BUG  CN78 1.0 SILICON -> DISABLE READ TIMEOUT */
	reg = cvmx_read_csr_node(node, CVMX_GSERX_SCRATCH(qlm));
	reg |= 0x01 << 15;
	cvmx_write_csr_node(node, CVMX_GSERX_SCRATCH(qlm), reg);

	/* Freeze the rx equalization */
	reg = cvmx_read_csr_node(node, CVMX_GSERX_LANEX_PWR_CTRL(lane, qlm));
	reg |= 0x40;
	cvmx_write_csr_node(node, CVMX_GSERX_LANEX_PWR_CTRL(lane, qlm), reg);
	reg = cvmx_read_csr_node(node, CVMX_GSERX_LANEX_RX_LOOP_CTRL(lane, qlm));
	reg |= 0x01;
	cvmx_write_csr_node(node, CVMX_GSERX_LANEX_RX_LOOP_CTRL(lane, qlm), reg);

	/* Save the Voltage Slicer.  It needs to be restored at the end. Else,
	   calibration and other functions will fail */
	Q_QB_ES_Original = Q_QB_ERR & cvmx_read_csr_node(node,
					CVMX_GSERX_LANEX_RX_CFG_4(lane, qlm));

	/* All chips after CN78XX pass 1.x use newer IP, requiring extra steps */
	if (!OCTEON_IS_MODEL(OCTEON_CN78XX_PASS1_X)) {
		/* Assert Shadow PI power enable bit:
		   DWC_RX_MISC_CTRL.pcs_sds_rx_misc_ctrl[0] = 1'b1 */
		reg = cvmx_read_csr_node(node, CVMX_GSERX_LANEX_RX_MISC_CTRL(lane, qlm));
		reg = cvmx_bit_insert(reg, 1, 0, 1);
		cvmx_write_csr_node(node, CVMX_GSERX_LANEX_RX_MISC_CTRL(lane, qlm), reg);

		/* Set Shadow PI phase to the middle of first UI:
		   DWC_RX_CDR_CTRL_2.cfg_rx_cdr_ctrl_ovrrd_val_15_0[14:8] = 7'd32 */
		reg = cvmx_read_csr_node(node, CVMX_GSERX_LANEX_RX_CDR_CTRL_2(lane, qlm));
		reg = cvmx_bit_insert(reg, 32, 8, 7);
		cvmx_write_csr_node(node, CVMX_GSERX_LANEX_RX_CDR_CTRL_2(lane, qlm), reg);

		/* Enable the shadow relative mode PI phase control by setting the following field:
		   DWC_RX_CDR_CTRL_2.cfg_rx_cdr_ctrl_ovrrd_val_15_0[15] = 1'b0
		   Note: A bit value of 1 is the absolute mode and 0 is the relative mode. */
		reg = cvmx_read_csr_node(node, CVMX_GSERX_LANEX_RX_CDR_CTRL_2(lane, qlm));
		reg = cvmx_bit_insert(reg, 0, 15, 1);
		cvmx_write_csr_node(node, CVMX_GSERX_LANEX_RX_CDR_CTRL_2(lane, qlm), reg);
	}

	/* Turn on the RT-eye monitor test circuit by setting the following field
	   to one:
	   DWC_RX_CFG_0.pcs_sds_rx_eyemon_en = 1'b1 */
	cvmx_rtenable(node, qlm, lane, 1);

	/* Enable the shadow PI clock and error sampler by setting bit 22 of the
	   23:16 range. That is, modify bit 6 of the 8 bits in the following
	   register as shown below:
	   DWC_RX_CDR_MISC_CTRL_0.pcs_sds_rx_cdr_misc_ctrl_23_16[6] = 1'b1 */
	cvmx_shadowpivenable(node, qlm, lane, 1);

	printf("Searching for eye...\n");

	zero_location = -1; /* Location of the eye */
	/* Step through all rotations looking for the eye */
	for (rotation = 0; rotation < EYE_MAX_ROTATIONS; rotation++) {
		int zero_count = 0;
		for (x = 0; x < EYE_MAX_X; x++) {
			uint32_t errors = cvmx_eye_measure_errors(node, qlm, lane, x, 0);
			if (errors == 0) {
				zero_count++;
				if (zero_count >= NUM_ZEROS) {
					/* We have enough zeros, mark the eye as found */
					zero_location = x - (zero_count - 1);
					break;
				}
			} else
				zero_count = 0;
		}
		/* Don't rotate the window if we found the eye */
		if (zero_location != -1)
			break;
		/* rotate the window */
		cvmx_eye_move_location(node, qlm, lane, EYE_MAX_X, 0);
	}

	/* Warn the user if we were unable to find the eye */
	if (zero_location == -1)
		cvmx_dprintf("N%d.QLM%d: Unable to find eye on lane %d\n", node, qlm, lane);

	printf("Capturing eye...\n");
	/* Loop through time with a step of two. See comment below about why it is
	   two */
	for (x = 0; x < EYE_MAX_X; x += 2) {
		int y;
		/* We need to sweep through all Y locations. Since it takes time to
		   move the sample point, we first scan up, then down. This way we
		   only ever move one spot at a time. X must increment by two
		   because of this */
		for (y = EYE_MIN_Y; y <= EYE_MAX_Y; y++)
			eye_data->data[y - EYE_MIN_Y][x] =
				cvmx_eye_measure_errors(node, qlm, lane, x, y);
		for (y = EYE_MAX_Y; y >= EYE_MIN_Y; y--)
			eye_data->data[y - EYE_MIN_Y][x + 1] =
				cvmx_eye_measure_errors(node, qlm, lane, x + 1, y);
	}

	/* Save the overall size */
	eye_data->width = EYE_MAX_X;
	eye_data->height = EYE_MAX_Y - EYE_MIN_Y + 1;

	/* Move back to the middle Y position. This covers this step from the docs:
	   Set Shadow PI phase to middle of UI. Walk the value back to 7'd32 in
	   steps of 2 or 1 until:
	   DWC_RX_CDR_CTRL_2.cfg_rx_cdr_ctrl_ovrrd_val_15_0[14:8] = 7'd32 */
	cvmx_eye_move_location(node, qlm, lane, EYE_MAX_X - 1, 0);

	/* Disable the shadow PI clock and Error Sampler by setting bit 22 of the
	   23:16 range. That is, modify bit 6 of the 8 bits in the following
	   register as shown below:
	   DWC_RX_CDR_MISC_CTRL_0[6] = 0 */
	cvmx_shadowpivenable(node, qlm, lane, 0);

	/* Turn off the RT-eye monitor test circuit by setting the following field
	   to one:
	   DIG.DWC_RX_CFG_0.PCS_SDS_RX_EYEMON_EN = 0 */
	cvmx_rtenable(node, qlm, lane, 0);

	/* All chips after CN78XX pass 1.x use newer IP, requiring extra steps */
	if (!OCTEON_IS_MODEL(OCTEON_CN78XX_PASS1_X)) {
		/* De-assert Shadow PI power enable bit:
		pcs_sds_rx_misc_ctrl[0] = 1'b0 */
		reg = cvmx_read_csr_node(node, CVMX_GSERX_LANEX_RX_MISC_CTRL(lane, qlm));
		reg = cvmx_bit_insert(reg, 0, 0, 1);
		cvmx_write_csr_node(node, CVMX_GSERX_LANEX_RX_MISC_CTRL(lane, qlm), reg);
	}

	/* Restore the Voltage Slicer */
	reg = cvmx_read_csr_node(node, CVMX_GSERX_LANEX_RX_CFG_4(lane, qlm));
	reg &= ~Q_QB_ERR;          // clear the bit field
	reg |= Q_QB_ES_Original;   // mask in the Starting Value;
	cvmx_write_csr_node(node, CVMX_GSERX_LANEX_RX_CFG_4(lane, qlm), reg);

	/* Un-freeze the rx equalization */
	reg = cvmx_read_csr_node(node, CVMX_GSERX_LANEX_PWR_CTRL(lane, qlm));
	reg &= ~0x40;
	cvmx_write_csr_node(node, CVMX_GSERX_LANEX_PWR_CTRL(lane, qlm), reg);
	reg = cvmx_read_csr_node(node, CVMX_GSERX_LANEX_RX_LOOP_CTRL(lane, qlm));
	reg &= ~0x01;
	cvmx_write_csr_node(node, CVMX_GSERX_LANEX_RX_LOOP_CTRL(lane, qlm), reg);

	/* Reset Lane RX circuitry */
	reg = cvmx_read_csr_node(node, CVMX_GSERX_LANEX_PWR_CTRL(lane, qlm));
	reg |= 0x100;
	cvmx_write_csr_node(node, CVMX_GSERX_LANEX_PWR_CTRL(lane, qlm), reg);
	cvmx_wait_usec(10);
	reg = cvmx_read_csr_node(node, CVMX_GSERX_LANEX_PWR_CTRL(lane, qlm));
	reg &= ~0x100;
	cvmx_write_csr_node(node, CVMX_GSERX_LANEX_PWR_CTRL(lane, qlm), reg);

	/* Per Joe V. request, add a delay so reset completes */
	cvmx_wait_usec(1000); /* 1ms */
	return 0;
}

/**
 * Display an eye diagram for the given QLM lane. The eye data can be in "eye", or
 * captured during the call if "eye" is NULL.
 *
 * @param node     Node to use in numa setup
 * @param qlm      QLM to use
 * @param qlm_lane Which lane
 * @param format   Display format. 0 = raw, 1 = Color ASCII
 * @param eye      Eye data to display, or NULL if the data should be captured.
 *
 * @return Zero on success, negative on failure
 */
int cvmx_qlm_eye_display(int node, int qlm, int qlm_lane, int format, const cvmx_qlm_eye_t *eye)
{
	int result;
	int need_free = 0;
	int x, y;

	if (eye == NULL) {
		cvmx_qlm_eye_t *eye_data = malloc(sizeof(cvmx_qlm_eye_t));
		if (eye_data == NULL) {
			cvmx_dprintf("Failed to allocate space for eye\n");
			return -1;
		}
		if (__cvmx_qlm_eye_capture(node, qlm, qlm_lane, eye_data))
			return -1;
		eye = eye_data;
	}

	printf("\nEye Diagram for Node %d, QLM %d, Lane %d\n", node, qlm, qlm_lane);

	if (format == 0) /* Raw */ {
		for (y = 0; y < eye->height; y++) {
			for (x = 0; x < eye->width; x++)
				printf("%u\t", (unsigned int)eye->data[y][x]);
			printf("\n");
		}
		result = 0;
	} else if (format == 1) /* Color ASCII */ {
		int last_color = -1;
		char color_str[] = "\33[40m"; /* Note: This is modified, not constant */

		for (y = 0; y < eye->height-1; y++) {
			for (x = 0; x < eye->width-1; x++) {
#define DIFF(a,b) (a<b) ? b-a : a-b
				int64_t dy = DIFF(eye->data[y][x], eye->data[y + 1][x]);
				int64_t dx = DIFF(eye->data[y][x], eye->data[y][x + 1]);
#undef DIFF
				int64_t dist = dx * dx + dy * dy;
				double f = log10(sqrt(dist) + 1);
				const double MAX_LOG = 6;
				int level, color;
				if (f > MAX_LOG)
					f = MAX_LOG;
				level = 9 * f / MAX_LOG + 0.5;
				color = 7.0 * f / MAX_LOG + 0.5;
				if (color != last_color) {
					color_str[3] = '0' + color;
					fputs(color_str, stdout);
					last_color = color;
				}
				fputc('0' + level, stdout);
			}

			fputs("\33[0m\n", stdout);
			last_color = -1;
		}
		result = 0;
	} else
		result = -1;

	if (need_free)
		free((void*)eye);
	return result;
}
