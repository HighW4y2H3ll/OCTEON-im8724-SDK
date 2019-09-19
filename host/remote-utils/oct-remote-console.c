/***********************license start************************************
 * Copyright (c) 2003-2008 Cavium Inc. (support@cavium.com). All rights
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
 * AND WITH ALL FAULTS AND CAVIUM NETWORKS MAKES NO PROMISES, REPRESENTATIONS
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
 * For any questions regarding licensing please contact marketing@cavium.com
 *
 **********************license end**************************************/

/**
 * @file
 *
 * Access an Octeon console over a remote interface
 *
 * $Id: oct-remote-console.c 100470 2014-06-24 00:10:13Z terenditsov $
 */
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <getopt.h>
#if !defined(__mips__) || !defined(CVMX_BUILD_FOR_LINUX_HOST)
#include <curses.h>
#endif
#include <fcntl.h>
#include <signal.h>

#include "cvmx.h"
#include "cvmx-version.h"
#include "octeon-remote.h"
#include "octeon-pci-console.h"
#include <lib_octeon_shared.h>
#include <octeon_mem_map.h>
#include "cvmx-bootmem.h"


static sighandler_t original_sig_int = NULL;
static sighandler_t original_sig_term = NULL;


/**
 * Signal handler to cleanup if we are interrupted
 *
 * @param num    Signal number
 */
void octeon_remote_console_cleanup(int num)
{
#if !defined(__mips__) || !defined(CVMX_BUILD_FOR_LINUX_HOST)
    endwin();
#endif
    octeon_remote_close();
    if (num == SIGINT && original_sig_int)
        original_sig_int(num);
    if (num == SIGTERM && original_sig_term)
        original_sig_term(num);
    exit(0);
}


#define MIN(a,b) (((a)<(b))?(a):(b))


/* Check to see if there is data to read on STDIN, since we don't want to block
** reading from it. */
int stdin_avail(void)
{
  fd_set rset;
  struct timeval tv;
  int ready_fds;

  FD_ZERO(&rset);
  FD_SET(fileno(stdin), &rset);

  tv.tv_sec = 0;
  tv.tv_usec = 1000;

  ready_fds = select(fileno(stdin) + 1, &rset, NULL, NULL, &tv);

  return (ready_fds > 0);
}

void usage(char *argv[])
{
    printf("\n"
           "Usage: %s [--noraw] <CONSOLE_NUMBER>\n"
           "    Connect to a remote console.\n"
           "\n", argv[0]);
}
int main(int argc, char *argv[])
{
    int c;
    int console_num = -1;
    int raw_flag = 1;
    uint64_t addr, size;
    int ret;

    while (1)
    {
        static struct option long_options[] =
        {
            /* These options set a flag. */
            {"noraw", no_argument, 0, 0},
            {"help", no_argument, 0, 'h'},
            {"version", no_argument, 0, 'v'},
            {0, 0, 0, 0}
        };
        /* getopt_long stores the option index here. */
        int option_index = 0;
                                                                                
        c = getopt_long (argc, (char * const *)argv, "hv",
                         long_options, &option_index);

        /* Detect the end of the options. */
        if (c == -1)
            break;

        switch (c)
        {
        /* All long options handled in case 0 */
        case 0:
            /* If this option set a flag, do nothing else now. */
            if (long_options[option_index].flag != 0)
                break;
#ifdef DEBUG
            printf ("option(l) %s", long_options[option_index].name);
            if (optarg)
                printf (" with arg %s", optarg);
            printf ("\n");
#endif

            if (!strcmp(long_options[option_index].name, "noraw"))
                raw_flag = 0;

            break;

        case 'h':
        case '?':
            /* getopt_long already printed an error message. */
            usage(argv);
        
        case 'v':
            printf("SDK version: %s\n", OCTEON_SDK_VERSION_STRING); 
            return -1;
            break;
                                                                                
        default:
            abort ();
        }
    }

    if (optind < argc)
    {
        /* We only support on argument - the console number */
        if (argc - optind > 1)
        {
#ifdef DEBUG
            printf ("non-option ARGV-elements: ");
            while (optind < argc)
                printf ("%s ", argv[optind++]);
            putchar ('\n');
#endif
            usage(argv);
            return -1;
        }
        console_num = atoi(argv[optind]);
    }

    if (console_num < 0)
    {
        usage(argv);
        exit(-1);
    }



    if (octeon_remote_open(OCTEON_REMOTE_DEFAULT, 0))
        return -1;

    if (!octeon_remote_mem_access_ok())
    {
        printf("ERROR: DRAM not setup, board needs to be booted\n");
        return -1;
    }

    if (!octeon_remote_named_block_find(OCTEON_PCI_CONSOLE_BLOCK_NAME, &addr, &size))
    {
        printf("ERROR: PCI console buffers not found, exiting\n");
        exit(-1);
    }

    if (octeon_remote_named_block_find(OCTEON_PCI_CONSOLE_BLOCK_NAME, &addr, &size))
    {
        char buffer[128];
//        printf("Block found, base: 0x%llx, size: 0x%llx\n", (long long) addr, (long long)size);

        // TODO - validate console number!
        /* Make sure that we are trying to connect to a valid console number */
        uint32_t num_consoles = octeon_remote_read_mem32(addr + offsetof(octeon_pci_console_desc_t, num_consoles));
        if (console_num >= (int)num_consoles)
        {
            printf("ERROR: trying to read from console number %d when only 0 to %d exist\n", console_num, num_consoles - 1);
            goto cleanup;
        }


        printf("Connecting to PCI console %d\n", console_num);

        original_sig_int  = signal(SIGINT, octeon_remote_console_cleanup);
        original_sig_term = signal(SIGTERM, octeon_remote_console_cleanup);
#if !defined(__mips__) || !defined(CVMX_BUILD_FOR_LINUX_HOST)
        if (raw_flag)
        {
            if (!initscr())
                printf("Error calling initscr\n");
            if (ERR == raw())
                printf("ERROR putting terminal in raw mode\n");
        }
#endif

        if (raw_flag)
            printf("Using raw terminal mode, escape character is ^A, use ^A D  to exit, ^A A to send ^A\r\n");
        else
            printf("Using cooked terminal mode (^C to exit).\r\n");

        while (1)
        {
            int i;
            int bytes_read = octeon_remote_console_host_read(addr, console_num, buffer, sizeof(buffer), 0);
            fflush(stdout); /* We're mixing C and raw output. Make sure the printfs get pushed out */
            if (bytes_read > 0)
            {
                /* We are in raw mode, so we need to do some translations
                ** * insert a \r when we see a bare \n */
                char r = '\r';
                for (i = 0; i < bytes_read; i++)
                {
                    if (buffer[i] == '\n' && (i == 0 || buffer[i-1] != '\r')) {
                        ret = write(fileno(stdout), &r, 1);
                        if (ret < 0) {
                            printf("Error reading from pci console\n");
                            goto cleanup;
                        }
                    }
                    ret = write(fileno(stdout), buffer + i, 1);
                    if (ret < 0) {
                        printf("Error reading from pci console\n");
                        goto cleanup;
                    }

                }
            }
            else if (bytes_read == 0)
            {
                //printf("No data to read\n");
            }
            else
            {
                printf("Error reading from pci console\n");
                goto cleanup;
            }

            /* Read from stdin and write to PCI */
            while (stdin_avail())
            {

                bytes_read = read(fileno(stdin), buffer, 1);
                if (buffer[0] == 0x1)  /* ^A */
                {
                    char esc_ch;
                    if (read(fileno(stdin), &esc_ch, 1))
                    {
                        if (esc_ch == 'a' || esc_ch == 'A')
                        {
                            /* Do nothing, fall through and let ^A go through */
                        }
                        else if (esc_ch == 'd' || esc_ch == 'd' || esc_ch == 'Q' || esc_ch == 'q')
                        {
                            goto cleanup;
                        }
                    }
                    else
                    {
                        printf("Error reading escape sequence from stdin\n");
                    }
                }

                if (bytes_read > 0)
                {
                    int i = 0;
                    while (bytes_read--)
                        octeon_remote_console_host_write(addr, console_num, buffer + i++, 1, 0);
                }
                else
                {
                   // printf("ERROR reading from stdin: %d\n", bytes_read);
                   break;
                }

            }


            usleep(1000);
        }
    }
    else
        printf("Block not found\n");
cleanup:
#if !defined(__mips__) || !defined(CVMX_BUILD_FOR_LINUX_HOST)
    if (raw_flag)
    {
        endwin();
    }
#endif
    signal(SIGINT, original_sig_int);
    signal(SIGTERM, original_sig_term);

    octeon_remote_close();
    return 0;
}

