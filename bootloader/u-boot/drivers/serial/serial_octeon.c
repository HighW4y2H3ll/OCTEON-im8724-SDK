/***********************license start************************************
 * Copyright (c) 2003-2013 Cavium Inc. (support@cavium.com). All rights
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
 * For any questions regarding licensing please contact
 * marketing@cavium.com
 *
 ***********************license end**************************************/

#include <common.h>
#include <serial.h>
#include <watchdog.h>
#include <asm/arch/octeon_boot.h>
#include <asm/arch/lib_octeon.h>
#ifdef CONFIG_OCTEON_LPC_UART
# include <ns16550.h>
#endif

#define GET_UART_INDEX(uart)	(uart & 3)
#define GET_UART_NODE(uart)	((uart >> 2) & 3)

static int octeon_serial_init(void);
static void octeon_serial_setbrg(void);
static void octeon_serial_putc(const char c);
static void octeon_serial_puts(const char *s);
static int octeon_serial_getc(void);
static int octeon_serial_tstc(void);

#ifdef CONFIG_OCTEON_LPC_UART
static int lpc_serial_init(void);
static void lpc_serial_setbrg(void);
static void lpc_serial_putc(const char c);
static void lpc_serial_puts(const char *s);
static int lpc_serial_getc(void);
static int lpc_serial_tstc(void);
#endif

DECLARE_GLOBAL_DATA_PTR;

struct serial_device octeon_serial0_device = {
	.name = "serial",
	.start = octeon_serial_init,
	.stop = NULL,
	.setbrg = octeon_serial_setbrg,
	.getc = octeon_serial_getc,
	.tstc = octeon_serial_tstc,
	.putc = octeon_serial_putc,
	.puts = octeon_serial_puts,
	.next = NULL
};

#ifdef CONFIG_OCTEON_LPC_UART
struct serial_device lpc_serial0_device = {
	.name = "lpc_serial",
	.start = lpc_serial_init,
	.stop = NULL,
	.setbrg = lpc_serial_setbrg,
	.getc = lpc_serial_getc,
	.tstc = lpc_serial_tstc,
	.putc = lpc_serial_putc,
	.puts = lpc_serial_puts,
	.next = NULL
};
#endif

struct serial_device *default_serial_console(void)
{
#ifdef CONFIG_OCTEON_LPC_UART
	if (gd->arch.lpc_uart_addr == 0) {
		return &octeon_serial0_device;
	} else {
		return &lpc_serial0_device;
	}
#else
	return &octeon_serial0_device;
#endif
}

void octeon_board_poll(void)
{

}

void octeon_board_poll(void) __attribute__ ((weak));

/******************************************************************************
*
* serial_init - initialize a channel
*
* This routine initializes the number of data bits, parity
* and set the selected baud rate. Interrupts are disabled.
* Set the modem control signals if the option is selected.
*
* RETURNS: N/A
*/

static uint16_t compute_divisor(uint32_t eclock_hz, uint32_t baud)
{
	uint16_t divisor;
#if CONFIG_OCTEON_SIM_SPEED
	divisor = 1;		/* We're simulating, go fast! */
#else
	divisor = ((ulong) (eclock_hz + 8 * baud) / (ulong) (16 * baud));
#endif

	return divisor;
}

/**
 * Function that does the real work of setting up the Octeon uart.
 * Takes all parameters as arguments, so it does not require gd
 * structure to be set up.
 *
 * @param uart_index Index of uart to configure
 * @param cpu_clock_hertz
 *                   CPU clock frequency in Hz
 * @param baudrate   Baudrate to configure
 *
 * @return 0 on success
 *         !0 on error
 */
int octeon_uart_setup2(int uart, int cpu_clock_hertz, int baudrate)
{
	uint16_t divisor;
	cvmx_uart_fcr_t fcrval;
	cvmx_uart_mcr_t mcrval;
	cvmx_uart_lcr_t lcrval;
	uint8_t uart_index = GET_UART_INDEX(uart);
	uint8_t node = GET_UART_NODE(uart);
#if !CONFIG_OCTEON_SIM_SPEED
	uint64_t read_cycle;
#endif

	fcrval.u64 = 0;
	fcrval.s.en = 1;	/* enable the FIFO's */
	fcrval.s.rxfr = 1;	/* reset the RX fifo */
	fcrval.s.txfr = 1;	/* reset the TX fifo */

	divisor = compute_divisor(cpu_clock_hertz, baudrate);

	cvmx_write_csr_node(node, CVMX_MIO_UARTX_FCR(uart_index), fcrval.u64);

	mcrval.u64 = 0;
#if CONFIG_OCTEON_SIM_SETUP
	if (uart_index == 1)
		mcrval.s.afce = 1;	/* enable auto flow control for
					 * simulator. Needed for gdb
					 * regression callfuncs.exp.
					 */
	else
		mcrval.s.afce = 0;	/* disable auto flow control so board
					 * can power on without serial port
					 * connected
					 */
#else
	mcrval.s.afce = 0;	/* disable auto flow control so board can power
				 * on without serial port connected
				 */
#endif
	mcrval.s.rts = 1;	/* looks like this must be set for auto flow
				 * control to work
				 */

	cvmx_read_csr_node(node, CVMX_MIO_UARTX_LSR(uart_index));

	lcrval.u64 = 0;
	lcrval.s.cls = CVMX_UART_BITS8;
	lcrval.s.stop = 0;	/* stop bit included? */
	lcrval.s.pen = 0;	/* no parity? */
	lcrval.s.eps = 1;	/* even parity? */
	lcrval.s.dlab = 1;	/* temporary to program the divisor */
	cvmx_write_csr_node(node, CVMX_MIO_UARTX_LCR(uart_index), lcrval.u64);

	cvmx_write_csr_node(node, CVMX_MIO_UARTX_DLL(uart_index),
			    divisor & 0xff);
	cvmx_write_csr_node(node, CVMX_MIO_UARTX_DLH(uart_index),
			    (divisor >> 8) & 0xff);

	/* divisor is programmed now, set this back to normal */
	lcrval.s.dlab = 0;
	cvmx_write_csr_node(node, CVMX_MIO_UARTX_LCR(uart_index), lcrval.u64);

#if !CONFIG_OCTEON_SIM_SPEED
	/* spec says need to wait after you program the divisor */
	read_cycle = octeon_get_cycles() + (2 * divisor * 16) + 10000;
	while (octeon_get_cycles() < read_cycle) {
		/* Spin */
	}
#endif

	/* Don't enable flow control until after baud rate is configured. - we
	 * don't want to allow characters in until after the baud rate is
	 * fully configured
	 */
	cvmx_write_csr_node(node, CVMX_MIO_UARTX_MCR(uart_index), mcrval.u64);
	return 0;

}

/*****************************************************************************/
/**
 * Setup a uart for use
 *
 * @param uart_index Uart to setup (0 or 1)
 * @return Zero on success
 */
int octeon_uart_setup(int uart)
{
	int ret;

	ret = octeon_uart_setup2(uart, octeon_get_ioclk_hz(), gd->baudrate);
#ifdef CONFIG_OCTEON_LPC_UART
	if (gd->arch.lpc_uart_addr != 0) {
		serial_register(&lpc_serial0_device);
	}
#endif
	return ret;
}

void octeon_set_baud(uint32_t uart, uint32_t baud)
{
	uint16_t divisor;
	uint8_t uart_index = GET_UART_INDEX(uart);
	uint8_t node = GET_UART_NODE(uart);
	cvmx_uart_lcr_t lcrval;

	divisor = compute_divisor(octeon_get_ioclk_hz(), baud);

	lcrval.u64 = cvmx_read_csr_node(node, CVMX_MIO_UARTX_LCR(uart_index));

	cvmx_wait((2 * divisor * 16) + 10000);

	lcrval.s.dlab = 1;	/* temporary to program the divisor */
	cvmx_write_csr_node(node, CVMX_MIO_UARTX_LCR(uart_index), lcrval.u64);

	cvmx_write_csr_node(node, CVMX_MIO_UARTX_DLL(uart_index),
			    divisor & 0xff);
	cvmx_write_csr_node(node, CVMX_MIO_UARTX_DLH(uart_index),
			    (divisor >> 8) & 0xff);
	/* divisor is programmed now, set this back to normal */
	lcrval.s.dlab = 0;
	cvmx_write_csr_node(node, CVMX_MIO_UARTX_LCR(uart_index), lcrval.u64);

#ifndef CONFIG_OCTEON_SIM_SPEED
	/* spec says need to wait after you program the divisor */
	cvmx_wait((2 * divisor * 16) + 10000);
	/* Wait a little longer..... */
	mdelay(5);
#endif
}

/**
 * Put a single byte to uart port.
 *
 * @param uart_index Uart to write to (0 or 1)
 * @param ch         Byte to write
 */
static inline void uart_write_byte(int uart, uint8_t ch)
{
	cvmx_uart_lsr_t lsrval;
	uint8_t uart_index = GET_UART_INDEX(uart);
	uint8_t node = GET_UART_NODE(uart);

	/* Spin until there is room */
	do {
		lsrval.u64 = cvmx_read_csr_node(node,
						CVMX_MIO_UARTX_LSR(uart_index));
	} while (lsrval.s.thre == 0);

	WATCHDOG_RESET();
	/* Write the byte */
	cvmx_write_csr_node(node, CVMX_MIO_UARTX_THR(uart_index), ch);
}

/**
 * Get a single byte from serial port.
 *
 * @param uart_index Uart to read from (0 or 1)
 * @return The byte read
 */
static inline uint8_t uart_read_byte(int uart)
{
	cvmx_uart_lsr_t lsrval;
	uint8_t uart_index = GET_UART_INDEX(uart);
	uint8_t node = GET_UART_NODE(uart);

	/* Spin until data is available */
	do {
		lsrval.u64 = cvmx_read_csr_node(node,
						CVMX_MIO_UARTX_LSR(uart_index));
		WATCHDOG_RESET();
		octeon_board_poll();
	} while (!lsrval.s.dr);

	/* Read and return the data */
	return cvmx_read_csr_node(node, CVMX_MIO_UARTX_RBR(uart_index));
}

/**
 * Initializes all of the serial ports
 */
static int octeon_serial_init(void)
{
	int i;

	gd->arch.console_uart = getenv_ulong("console_uart", 0,
					     CONFIG_OCTEON_DEFAULT_CONSOLE_UART_PORT);
	/*
	 * Initialize all UARTS even though we only use one for the
	 * console.  Some applications (Linux second kernel) expect both
	 * to work.
	 */
	for (i = CONFIG_OCTEON_MIN_CONSOLE_UART;
	     i <= CONFIG_OCTEON_MAX_CONSOLE_UART; i++)
		octeon_uart_setup(i);

	return 0;
}

static void octeon_serial_setbrg(void)
{
	octeon_set_baud(gd->arch.console_uart, gd->baudrate);
}

static void octeon_serial_putc(const char c)
{
#if !CONFIG_OCTEON_SIM_HW_DIFF
	if (c == '\n') {
		uart_write_byte(gd->arch.console_uart, '\r');
	}
#endif

	uart_write_byte(gd->arch.console_uart, c);
}

static void octeon_serial_puts(const char *s)
{
	while (*s)
		octeon_serial_putc(*s++);
}

static int octeon_serial_getc(void)
{
	return uart_read_byte(gd->arch.console_uart);
}

static int octeon_serial_tstc(void)
{
	cvmx_uart_lsr_t lsrval;
	uint8_t uart_index = GET_UART_INDEX(gd->arch.console_uart);
	uint8_t node = GET_UART_NODE(gd->arch.console_uart);

	octeon_board_poll();
	WATCHDOG_RESET();

	lsrval.u64 = cvmx_read_csr_node(node,
					CVMX_MIO_UARTX_LSR(uart_index));
	return lsrval.s.dr;
}

#ifdef CONFIG_OCTEON_LPC_UART
static int calc_lpc_uart_divisor(NS16550_t port)
{
#ifdef CONFIG_OMAP1510
	/* If can't cleanly clock 115200 set div to 1 */
	if ((CONFIG_SYS_NS16550_CLK == 12000000) && (gd->baudrate == 115200)) {
		port->osc_12m_sel = OSC_12M_SEL;	/* enable 6.5 * divisor */
		return (1);	/* return 1 for base divisor */
	}
	port->osc_12m_sel = 0;	/* clear if previsouly set */
#endif
#ifdef CONFIG_OMAP1610
	/* If can't cleanly clock 115200 set div to 1 */
	if ((CONFIG_SYS_NS16550_CLK == 48000000) && (gd->baudrate == 115200)) {
		return (26);	/* return 26 for base divisor */
	}
#endif

#define MODE_X_DIV 16
	/* Compute divisor value. Normally, we should simply return:
	 *   CONFIG_SYS_NS16550_CLK) / MODE_X_DIV / gd->baudrate
	 * but we need to round that value by adding 0.5.
	 * Rounding is especially important at high baud rates.
	 */
	return (CONFIG_SYS_NS16550_CLK + (gd->baudrate * (MODE_X_DIV / 2))) /
	    (MODE_X_DIV * gd->baudrate);
}

static int lpc_serial_init(void)
{
	int clock_divisor;

	clock_divisor =
	    calc_lpc_uart_divisor((NS16550_t) gd->arch.lpc_uart_addr);
	NS16550_init((NS16550_t) gd->arch.lpc_uart_addr, clock_divisor);

	return 0;
}

static void lpc_serial_setbrg(void)
{
	int clock_divisor;

	clock_divisor =
	    calc_lpc_uart_divisor((NS16550_t) gd->arch.lpc_uart_addr);
	NS16550_reinit((NS16550_t) gd->arch.lpc_uart_addr, clock_divisor);
}

static int lpc_serial_getc(void)
{
	int ret;

	ret = NS16550_getc((NS16550_t) gd->arch.lpc_uart_addr);

	return ret;
}

static int lpc_serial_tstc(void)
{
	int ret;

	ret = NS16550_tstc((NS16550_t) gd->arch.lpc_uart_addr);

	return ret;
}

static void lpc_serial_putc(const char c)
{
	if (c == '\n') {
		NS16550_putc((NS16550_t) gd->arch.lpc_uart_addr, '\r');
	}

	NS16550_putc((NS16550_t) gd->arch.lpc_uart_addr, c);
}

static void lpc_serial_puts(const char *s)
{
	while (*s) {
		lpc_serial_putc(*s++);
	}
}
#endif
