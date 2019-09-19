/***********************license start***************
 * Copyright (c) 2003-2010  Cavium Inc. (support@cavium.com). All rights 
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
 * @file uart.c
 *
 * Sample uart code showing input and output using the uart with interrupts.
 *
 * File version info: $Id: uart_irq.c 135591 2016-03-25 04:41:59Z cchavva $
 *
 */

#include <stdio.h>
#include <string.h>
#include "cvmx-config.h"
#include "cvmx.h"
#include "cvmx-uart.h"
#include "cvmx-interrupt.h"
#include "cvmx-spinlock.h"
#include "cvmx-sysinfo.h"

#define BUFFER_SIZE 1024    /* Size of the uart buffers. Must be a power of 2 */

/**
 * Structure to use for TX and RX buffers
 */
typedef struct
{
    uint8_t data[BUFFER_SIZE];  /* Data buffered for the uart */
    volatile int head;          /* New characters are put here */
    volatile int tail;          /* Characters are removed from here */
    cvmx_spinlock_t lock;
} uart_buffer_t;

/**
 * Buffer used for pending TX data to the uart
 */
static uart_buffer_t tx_buffer;

/**
 * Buffer used to store data from the uart
 */
static uart_buffer_t rx_buffer;


/**
 * Initialize a buffer to empty
 *
 * @param buffer Buffer to initialize
 */
static void uart_buffer_initalize(uart_buffer_t *buffer)
{
    cvmx_spinlock_init(&buffer->lock);
    buffer->head = 0;
    buffer->tail = 0;
}


/**
 * Check if a buffer is empty
 *
 * @param buffer Buffer to check
 * @return True if the buffer is empty
 */
static int uart_buffer_empty_nolock(const uart_buffer_t *buffer)
{
    return (buffer->head == buffer->tail);
}

static int uart_buffer_empty(uart_buffer_t *buffer)
{
    int r;
    uint32_t flags;

    cvmx_local_irq_save(flags);
    cvmx_spinlock_lock(&buffer->lock);
    r = uart_buffer_empty_nolock(buffer);
    cvmx_spinlock_unlock(&buffer->lock);
    cvmx_local_irq_restore(flags);
    return r;
}



/**
 * Check if a buffer is full
 *
 * @param buffer Buffer to check
 * @return True of the buffer is full
 */
static int uart_buffer_full_nolock(const uart_buffer_t *buffer)
{
    return ((buffer->head + 1) % BUFFER_SIZE)  == buffer->tail;
}

static int uart_buffer_full(uart_buffer_t *buffer)
{
    int r;
    uint32_t flags;

    cvmx_local_irq_save(flags);
    cvmx_spinlock_lock(&buffer->lock);
    r = uart_buffer_empty_nolock(buffer);
    cvmx_spinlock_unlock(&buffer->lock);
    cvmx_local_irq_restore(flags);
    return r;
}

/**
 * Read a byte from the buffer. It returns zero if the buffer
 * if empty. You should check uart_buffer_empty() before calling
 * this function.
 *
 * @param buffer Buffer to read from
 * @return Data or zero on error (buffer empty)
 */
static uint8_t uart_buffer_read_nolock(uart_buffer_t *buffer)
{
    uint8_t result;

    if (uart_buffer_empty_nolock(buffer))
        return 0;

    result = buffer->data[buffer->tail];
    buffer->tail = (buffer->tail + 1) % BUFFER_SIZE;
    return result;
}


/**
 * Write a byte to the buffer. The byte will be lost if the
 * buffer is full. Check uart_buffer_full() before calling
 * this function.
 *
 * @param buffer Buffer to write to
 * @param data
 */
static void uart_buffer_write_nolock(uart_buffer_t *buffer, uint8_t data)
{
    if (uart_buffer_full_nolock(buffer))
        return;

    buffer->data[buffer->head] = data;
    buffer->head = (buffer->head + 1) % BUFFER_SIZE;
}

/**
 * Setup a uart for use
 *
 * @param uart_index Uart to setup (0 or 1)
 * @return Zero on success
 */
static int uart_setup(int uart_index)
{
    cvmx_uart_fcr_t fcrval;
    cvmx_uart_ier_t ierval;
    cvmx_uart_mcr_t mcrval;
    cvmx_uart_lcr_t lcrval;

    /* Empty the RX and TX buffers */
    uart_buffer_initalize(&rx_buffer);
    uart_buffer_initalize(&tx_buffer);

    /* no need to initialize uart since bootloader has done it already */
    /* only need to initialize the additional uart interrupt enables */

    ierval.u64 = 0;
    ierval.s.ptime = 1; /* Enable the THRE programmable interrupts */
    ierval.s.etbei = 1; /* Interrupt when the TX buffer is empty */
    ierval.s.erbfi = 1; /* Interrupt when there is RX data */
    cvmx_write_csr(CVMX_MIO_UARTX_IER(uart_index), ierval.u64);

    return 0;
}

struct uart_interrupt_irq {
	struct cvmx_interrupt irq;
	int uart_index;
};

/**
 * Uart interrupt handler. Currently this function only
 * handles the RX and TX data well. Uart errors print a
 * message, but do nothing else. RX data is stored into
 * the buffer "rx_buffer". TX data is read from the buffer
 * "tx_buffer". Remember that TX interrupts only occur when
 * we transition to empty. The first byte sent when the Uart
 * is idle must be sent directly to the uart and not buffer
 * in tx_buffer.
 *
 * @param irq The interruptthat occurred. This will be 2 for the
 *                   uarts.
 * @param registers  CPU registers at time of interrupt
 */
static void uart_interrupt_handler(struct cvmx_interrupt *irq, uint64_t registers[32])
{
	struct uart_interrupt_irq *uirq = CVMX_CONTAINTER_OF(irq, struct uart_interrupt_irq, irq);
	int count;
	cvmx_uart_iir_t iir;
	cvmx_uart_lsr_t lsr;
	iir.u64 = cvmx_read_csr(CVMX_MIO_UARTX_IIR(uirq->uart_index));
	lsr.u64 = cvmx_read_csr(CVMX_MIO_UARTX_LSR(uirq->uart_index));

	switch (iir.s.iid)
	{
        case CVMX_UART_IID_NONE:
		/* Nothing to do */
		cvmx_safe_printf("UART NONE\n");
		break;
        case CVMX_UART_IID_RX_ERROR:
		cvmx_safe_printf("UART RX ERROR lsr=%llx\n", (unsigned long long)lsr.u64);
		break;
        case CVMX_UART_IID_RX_TIMEOUT:
		cvmx_safe_printf("UART RX TIMEOUT lsr=%llx\n", (unsigned long long)lsr.u64);
		break;
        case CVMX_UART_IID_MODEM:
		cvmx_safe_printf("UART MODEM STATUS lsr=%llx\n", (unsigned long long)lsr.u64);
		break;
        case CVMX_UART_IID_RX_DATA:
		/* Handled below */
		break;
        case CVMX_UART_IID_TX_EMPTY:
		/* We know the uart is empty, so write out the max amount of data,
		   64 bytes */
		count = 64;
		cvmx_spinlock_lock(&tx_buffer.lock);
		while ((!uart_buffer_empty_nolock(&tx_buffer)) && (count--))
		{
			cvmx_write_csr(CVMX_MIO_UARTX_THR(uirq->uart_index), uart_buffer_read_nolock(&tx_buffer));
		}
		cvmx_spinlock_unlock(&tx_buffer.lock);
		break;
        case CVMX_UART_IID_BUSY:
		/* This is needed on CN30XX and CN31XX Pass1 chips. This is a
		   workaround for Errata UART-300 */
		cvmx_read_csr(CVMX_MIO_UARTX_USR(uirq->uart_index));
	}

	/* Regardless of why we got here read in any pending data */
	while (lsr.s.dr)
	{
		uint64_t input_char = cvmx_read_csr(CVMX_MIO_UARTX_RBR(uirq->uart_index));

		cvmx_spinlock_lock(&rx_buffer.lock);
		if (uart_buffer_full_nolock(&rx_buffer))
			cvmx_safe_printf("UART RX BUFFER OVERFLOW\n");
		else
			uart_buffer_write_nolock(&rx_buffer, input_char);
		cvmx_spinlock_unlock(&rx_buffer.lock);
		lsr.u64 = cvmx_read_csr(CVMX_MIO_UARTX_LSR(uirq->uart_index));
	}
}

/**
 * Put a single byte to uart port.
 *
 * @param uart_index Uart to write to (0 or 1)
 * @param ch         Byte to write
 */
static void uart_write_byte(int uart_index, uint8_t ch)
{
    uint32_t flags;

    cvmx_local_irq_save(flags);
    cvmx_spinlock_lock(&tx_buffer.lock);
    /* if the TX buffer is empty we may need to send it directly to the
        UART. */
    if (uart_buffer_empty_nolock(&tx_buffer))
    {
        cvmx_uart_lsr_t lsr;
        lsr.u64 = cvmx_read_csr(CVMX_MIO_UARTX_LSR(uart_index));

        if (!lsr.s.thre)
        {
            /* Write the byte now. The uart has room */
            cvmx_write_csr(CVMX_MIO_UARTX_THR(uart_index), ch);
            goto out;
        }
        /* UART didn't have room so we need to buffer it */
    }

    while (uart_buffer_full_nolock(&tx_buffer))
    {
        cvmx_spinlock_unlock(&tx_buffer.lock);
	cvmx_local_irq_restore(flags);
        /* Spin until there is room */
	cvmx_local_irq_save(flags);
	cvmx_spinlock_lock(&tx_buffer.lock);
    }
    uart_buffer_write_nolock(&tx_buffer, ch);
out:
    cvmx_spinlock_unlock(&tx_buffer.lock);
    cvmx_local_irq_restore(flags);
    return;
}


/**
 * Write a string to the uart
 *
 * @param uart_index Uart to use (0 or 1)
 * @param str        String to write
 */
static void uart_write_string(int uart_index, const char *str)
{
    /* Just loop writing one byte at a time */
    while (*str)
    {
        uart_write_byte(uart_index, *str);
        str++;
    }
}


/**
 * Get a single byte from serial port.
 *
 * @param uart_index Uart to read from (0 or 1)
 * @return The byte read
 */
static uint8_t uart_read_byte(int uart_index)
{
    uint32_t flags;
    uint8_t r;

    cvmx_local_irq_save(flags);
    cvmx_spinlock_lock(&rx_buffer.lock);
    while (uart_buffer_empty_nolock(&rx_buffer))
    {
        cvmx_spinlock_unlock(&rx_buffer.lock);
	cvmx_local_irq_restore(flags);
        /* Spin until there is room */
	cvmx_local_irq_save(flags);
	cvmx_spinlock_lock(&rx_buffer.lock);
    }
    r = uart_buffer_read_nolock(&rx_buffer);
    cvmx_spinlock_unlock(&rx_buffer.lock);
    cvmx_local_irq_restore(flags);
    return r;
}


/**
 * Read a line from the uart and return it as a string.
 * Carriage returns and newlines are removed.
 *
 * @param uart_index Uart to read from (0 or 1)
 * @param buffer     Buffer to put the result in
 * @param bufferSize Size of the buffer
 * @return Length of the string
 */
static int uart_read_string(int uart_index, char *buffer, int bufferSize)
{
    int count = 0;

    /* Read until we get a newline or run out of room */
    while (count < bufferSize - 1)
    {
        buffer[count] = uart_read_byte(uart_index);
	if (cvmx_sysinfo_get()->board_type == CVMX_BOARD_TYPE_SIM)
	{
           if (buffer[count] == '\r')
               continue;
           if (buffer[count] == '\n')
               break;
	}
	else
	{
           uart_write_byte(uart_index, buffer[count]);  /* echo */
           if (buffer[count] == '\r') {
               uart_write_byte(uart_index, '\n');  /* linefeed */
               break;
           }
	}
        count++;
    }

    /* Null terminate the string to make life easy */
    buffer[count] = 0;

    return count;
}

static CVMX_SHARED struct uart_interrupt_irq uart_interrupt_irq = {
  .irq = {.handler = uart_interrupt_handler, .level_trigged = 1}
};

/**
 * Main entry point
 *
 * @return
 */
int main()
{
    const unsigned long uart_index = 0;
    char buffer[256];
    cvmx_ciu_intx0_t irq_control;


    /* This is a single threaded application.  If started on more than
       a single core, make the unwanted cores exit. */
    if (!cvmx_is_init_core()) {
	return 0;
    }

	/* For compatibility with the OCTEONs not supporting multinode configuration.*/
	if (cvmx_get_node_num() > 0) {
		return 0;
	}
#define NL "\r\n"
    if (cvmx_sysinfo_get()->board_type == CVMX_BOARD_TYPE_SIM)
    {
      simprintf("Output from this example goes to the uart. Make sure you connect\n");
      simprintf("to the simulator with a tcp connection. The following should work:\n");
      simprintf("    $ telnet localhost 2020\n");
    }

    /* Setup the interrupt handler */
    uart_interrupt_irq.uart_index = uart_index;
    cvmx_interrupt_register(cvmx_interrupt_map(CVMX_IRQ_UART0 + uart_index), &uart_interrupt_irq.irq);

    /* Setup the uart */
    uart_setup(uart_index);

    /* Enable the CIU uart interrupt */
    uart_interrupt_irq.irq.unmask(&uart_interrupt_irq.irq);

    /* Display a banner to the user */
    uart_write_string(uart_index, NL NL "Hello from the Cavium Octeon uart_irq example." NL);

    /* Loop until they get tired and tell us to quit */
    buffer[0] = 0;
    while (strcmp(buffer, "quit") != 0)
    {
        uart_write_string(uart_index, NL "What is your name?" NL);

        uart_read_string(uart_index, buffer, sizeof(buffer));

        if (strcmp(buffer, "quit") == 0)
        {
            uart_write_string(uart_index, "Ok, I can take a hint. Exiting..." NL NL);
        }
        else
        {
            uart_write_string(uart_index, "Nice to meet you ");
            uart_write_string(uart_index, buffer);
            uart_write_string(uart_index, "." NL);
            uart_write_string(uart_index, "This is a really simple example. It is going to" NL
                                          "continue to ask for your name until you enter \"quit\"." NL);
        }
    }

    while (!uart_buffer_empty(&tx_buffer))
    {
        /* Wait for the tx buffer to empty */
    }

    return 0;
}
