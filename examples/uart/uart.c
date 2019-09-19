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
 * Sample uart code showing input and output using the uart.
 *
 * File version info: $Id: uart.c 135349 2016-03-21 18:58:58Z cchavva $
 *
 */

#include <stdio.h>
#include <string.h>
#include "cvmx-config.h"
#include "cvmx.h"
#include "cvmx-uart.h"
#include "cvmx-sysinfo.h"


/**
 * Put a single byte to uart port.
 * 
 * @param uart_index Uart to write to (0 or 1)
 * @param ch         Byte to write
 */
static inline void uart_write_byte(int uart_index, uint8_t ch)
{
    cvmx_uart_lsr_t lsrval;

    /* Spin until there is room */
    do
    {
        lsrval.u64 = cvmx_read_csr(CVMX_MIO_UARTX_LSR(uart_index));
    }
    while (lsrval.s.thre == 0);

    /* Write the byte */
    cvmx_write_csr(CVMX_MIO_UARTX_THR(uart_index), ch);
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
static inline uint8_t uart_read_byte(int uart_index) 
{
    cvmx_uart_lsr_t lsrval;

    /* Spin until data is available */
    do 
    {
        lsrval.u64 = cvmx_read_csr(CVMX_MIO_UARTX_LSR(uart_index));
    } while (!lsrval.s.dr);

    /* Read and return the data */
    return cvmx_read_csr(CVMX_MIO_UARTX_RBR(uart_index));
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


/**
 * Main entry point
 * 
 * @return 
 */
int main()
{
    const int uart_index = 0;
    char buffer[256];

    /* This is a single threaded application.  If started on more than
       a single core, make the unwanted cores exit. */
    if (!cvmx_is_init_core()) {
	return 0;
    }

#define NL "\r\n"
    if (cvmx_sysinfo_get()->board_type == CVMX_BOARD_TYPE_SIM)
    {
       simprintf("Output from this example goes to the uart. Make sure you connect\n");
       simprintf("to the simulator with a tcp connection. The following should work:\n");
       simprintf("    $ telnet localhost 2020\n");
    }

    /* no need to setup uart as bootloader has done so already */

    /* Display a banner to the user */
    uart_write_string(uart_index, NL NL "Hello from the Cavium Octeon uart example." NL);

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

    return 0;
}
