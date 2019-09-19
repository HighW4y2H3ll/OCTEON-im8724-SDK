/***********************license start************************************
 * Copyright (c) 2012 Cavium (support@cavium.com). All rights
 * reserved.
 *
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials provided
 *       with the distribution.
 *
 *     * Neither the name of Cavium Inc. nor the names of
 *       its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written
 *       permission.
 *
 * TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
 * AND WITH ALL FAULTS AND CAVIUM INC. MAKES NO PROMISES, REPRESENTATIONS
 * OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH
 * RESPECT TO THE SOFTWARE, INCLUDING ITS CONDITION, ITS CONFORMITY TO ANY
 * REPRESENTATION OR DESCRIPTION, OR THE EXISTENCE OF ANY LATENT OR PATENT
 * DEFECTS, AND CAVIUM SPECIFICALLY DISCLAIMS ALL IMPLIED (IF ANY) WARRANTIES
 * OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR
 * PURPOSE, LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET
 * POSSESSION OR CORRESPONDENCE TO DESCRIPTION.  THE ENTIRE RISK ARISING OUT
 * OF USE OR PERFORMANCE OF THE SOFTWARE LIES WITH YOU.
 *
 *
 * For any questions regarding licensing please contact support@cavium.com
 *
 ***********************license end**************************************/

/*
   @cavium octeon chip
   @asus na-o38 board
   @u-boot set the ddr clock
   @RichardXY_Huang@asus.com.cn
 */

#include <common.h>
#include <command.h>

#include <asm/arch/octeon_boot.h>
#include <i2c.h>

#define OUTPUT printf ("\n\tinput a kind of ddr clock \n\t\t1): 400\n\t\t2): " \
		       "533\n\t\t3): 667\n\t\t4): 800\n")
#define CKG_SETUP_TIME	10
#define CKG_HOLD_TIME	10

#define GPIO_WRITE_EN	0x00001ull

#define GPIO_9_BIT	0x00200ull
#define GPIO_10_BIT	0x00400ull
#define GPIO_11_BIT	0x00800ull
#define GPIO_12_BIT	0x01000ull
#define GPIO_13_BIT	0x02000ull

/* simple bus use octeon GPIO 9 (CK) GPIO 10 (DA)  GPIO 11 (EN) as signal. */
static inline void init_bus (void)
{
	/* set GPIO  9 to write  (CK) */
	cvmx_write_csr (CVMX_GPIO_BIT_CFGX(9), cvmx_read_csr (CVMX_GPIO_BIT_CFGX(9))
				       | GPIO_WRITE_EN);
	/* set GPIO9 output 0 */
	cvmx_write_csr (CVMX_GPIO_TX_CLR, GPIO_9_BIT);

	/* set GPIO 10 to write  (DA) */
	cvmx_write_csr (CVMX_GPIO_BIT_CFGX(10), cvmx_read_csr (CVMX_GPIO_BIT_CFGX(10))
					| GPIO_WRITE_EN);


	/* set GPIO10 output 0 */
	cvmx_write_csr (CVMX_GPIO_TX_CLR, GPIO_10_BIT);

	/* set GPIO 11 to write  (EN) */
	cvmx_write_csr (CVMX_GPIO_BIT_CFGX(11), cvmx_read_csr (CVMX_GPIO_BIT_CFGX(11))
					| GPIO_WRITE_EN);

	/* set GPIO11 output 0 */
	cvmx_write_csr (CVMX_GPIO_TX_CLR, GPIO_11_BIT);
}

static inline void send_bit (unsigned char bit)
{
	/* set DA output 0 */
	cvmx_write_csr ((bit ? CVMX_GPIO_TX_SET : CVMX_GPIO_TX_CLR), GPIO_10_BIT);

	/* delay CPU clock */
	udelay (CKG_SETUP_TIME);

	/* set CK output 1 */
	cvmx_write_csr (CVMX_GPIO_TX_SET, GPIO_9_BIT);

	/* delay CPU clock */
	udelay (CKG_HOLD_TIME);

	/* set CK output 0 */
	cvmx_write_csr (CVMX_GPIO_TX_CLR, GPIO_9_BIT);
}

#define W837942D_ADDR   0x2c

#define GPIOB_OUT_CNL_W83792    0x1d
#define GPIOB_DATA_W83792       0x1e
#define GPIO_EN_W83792          0x1a

/*
	DDR2_PLL_DIV2_EN : (W83792G GPOB7)
	DDR2 400  (default) or  Fout <= 500
	Bank 0, Index 1D bit 7 = 0 , input mode
	Bank 0, Index 1E bit 7 = 0 , no function
	Bank 0, Index 1A bit 1 = 0 , GPIOB7  Disable

	DDR2 533/667/800 or Fout >  501
	Bank 0, Index 1D bit 7 = 1 , output mode
	Bank 0, Index 1E bit 7 = 1 ,  output "1"
	Bank 0, Index 1A bit 1 = 1 , GPIOB7  Enable
 */

static inline void ddr_pll_div_set (void)
{
	uint8_t temp;
	i2c_read (W837942D_ADDR, GPIOB_OUT_CNL_W83792, 1, &temp, 1);
	temp |= 0x01 << 7;
	i2c_write (W837942D_ADDR, GPIOB_OUT_CNL_W83792, 1, &temp, 1);

	i2c_read (W837942D_ADDR, GPIOB_DATA_W83792, 1, &temp, 1);
	temp |= 0x01 << 7;
	i2c_write (W837942D_ADDR, GPIOB_DATA_W83792, 1, &temp, 1);

	i2c_read (W837942D_ADDR, GPIO_EN_W83792, 1, &temp, 1);
	temp |= 0x01 << 1;
	i2c_write (W837942D_ADDR, GPIO_EN_W83792, 1, &temp, 1);

}

static inline void ddr_pll_div_clr (void)
{
	uint8_t temp;
	i2c_read (W837942D_ADDR, GPIOB_OUT_CNL_W83792, 1, &temp, 1);
	temp &= ~(0x01 << 7);
	i2c_write (W837942D_ADDR, GPIOB_OUT_CNL_W83792, 1, &temp, 1);

	i2c_read (W837942D_ADDR, GPIOB_DATA_W83792, 1, &temp, 1);
	temp &= ~(0x01 << 7);
	i2c_write (W837942D_ADDR, GPIOB_DATA_W83792, 1, &temp, 1);

	i2c_read (W837942D_ADDR, GPIO_EN_W83792, 1, &temp, 1);
	temp &= ~(0x01 << 1);
	i2c_write (W837942D_ADDR, GPIO_EN_W83792, 1, &temp, 1);

}

/* use w837942d GPB1 as signal. */
static inline void set_status_signal (void)
{
	uint8_t temp;
	i2c_read (W837942D_ADDR, GPIOB_OUT_CNL_W83792, 1, &temp, 1);
	temp |= 0x02;
	/* set output mode */
	i2c_write (W837942D_ADDR, GPIOB_OUT_CNL_W83792, 1, &temp, 1);

	i2c_read (W837942D_ADDR, GPIOB_DATA_W83792, 1, &temp, 1);
	temp |= 0x02;
	/* set 1 */
	i2c_write (W837942D_ADDR, GPIOB_DATA_W83792, 1, &temp, 1);

	/* enable GPB1 */
	i2c_read (W837942D_ADDR, GPIO_EN_W83792, 1, &temp, 1);
	temp |= 0x08;
	i2c_write (W837942D_ADDR, GPIO_EN_W83792, 1, &temp, 1);
}

inline int get_status_signal (void)
{
	uint8_t temp;

	/* set into input mode */
	i2c_read (W837942D_ADDR, GPIOB_OUT_CNL_W83792, 1, &temp, 1);
	temp &= ~0x02;
	i2c_write (W837942D_ADDR, GPIOB_OUT_CNL_W83792, 1, &temp, 1);

	/* enable */
	i2c_read (W837942D_ADDR, GPIO_EN_W83792, 1, &temp, 1);
	temp |= 0x08;
	i2c_write (W837942D_ADDR, GPIO_EN_W83792, 1, &temp, 1);

	/* read the status of the GPB1 */
	i2c_read (W837942D_ADDR, GPIOB_DATA_W83792, 1, &temp, 1);

	return temp & 0x02;
}

static inline void pll_reset (void)
{
	/* load M,N */
	udelay (40000000); /* delay CPU clock */

	/* set GPIO13 to write  (Reset PLL) */
	cvmx_write_csr (CVMX_GPIO_BIT_CFGX(13), GPIO_WRITE_EN);

	/* set GPIO13 output 1 */
	cvmx_write_csr (CVMX_GPIO_TX_SET, GPIO_13_BIT);
}

/*
/*           T2 T1 T0 N1 N0 M8 M7 M6 M5 M4 M3 M2 M1 M0 */
/* DDRII 400  0  0  0  1  0  1  0  0  0  0  0  0  0  0 */
/* DDRII 533           1  0  0  1  0  1  0  1  0  1  1 */
/* DDRII 667           1  0  0  1  1  0  1  0  1  0  1 */
/* DDRII 800           1  0  1  0  0  0  0  0  0  0  0 */

 */

#define DDR_400_CMD	0x0028
#define DDR_533_CMD	0x3548
#define DDR_667_CMD	0x2ac8
#define DDR_800_CMD	0x0028

#define COMMAND_MASK 0x3fff /* it [0:13] */

static inline void send_cmd (unsigned int cmd)
{
	unsigned int i;
	unsigned int mask = 0x1;
	cmd &= COMMAND_MASK;

 /* init_bus(); */

	for (i = 0; i < 14; i++) {
		/* used for test cmd */
		printf ("%d", (cmd & mask) ? 1 : 0);
/*      send_bit((cmd & mask )? 1 : 0); */
		mask <<= 1;
	}

}

void ddr_clock_setting (unsigned char a)
{
	switch (a) {
	case '1':
		printf ("ddr 400\n");
		send_cmd (DDR_400_CMD);
/*          ddr_pll_div_clr(); */
		break;
	case '2':
		printf ("ddr 533\n");
		send_cmd (DDR_533_CMD);
/*          ddr_pll_div_set(); */
		break;
	case '3':
		printf ("ddr 667\n");
		send_cmd (DDR_667_CMD);
/*          ddr_pll_div_set(); */
		break;
	case '4':
		printf ("ddr 800\n");
		send_cmd (DDR_800_CMD);
/*          ddr_pll_div_set(); */
		break;
	default:
		printf ("error type of ddr2 clock\n");
		OUTPUT;
		return;
	}

	/* set the statue's figture */
	set_status_signal ();

	printf ("\t reboot system\n");
	pll_reset ();

}

int do_set_ddr_clock (cmd_tbl_t * cmdtp, int flag, int argc, char *argv[])
{
	DECLARE_GLOBAL_DATA_PTR;
	unsigned char a;

	init_bus ();
	set_status_signal ();
	printf ("\t reboot system\n");
 /* pll_reset(); */
	{
		int temp;
		temp = get_status_signal ();
		printf ("temp = %d\n", temp);
	}

	return 1;

}

U_BOOT_CMD (set_ddr_clock, 5, 1, do_set_ddr_clock,
	    "set ddr2 clock and reset\n", "just test\n");
