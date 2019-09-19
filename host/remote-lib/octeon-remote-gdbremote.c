/***********************license start************************************
 * Copyright (c) 2003-2009 Cavium Inc. (support@cavium.com). All rights
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
 * Interface to Octeon through a remote gdb stub.
 *
 * $Id: octeon-remote-gdbremote.c 41605 2009-03-20 01:13:49Z kreese $
 */
#include <stdio.h>
#include <netdb.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <resolv.h>
#include <sys/time.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include "octeon-remote.h"
#define CVMX_ADD_IO_SEG(a) (a)
#include "cvmx.h"
#include "cvmx-swap.h"

#define ULL unsigned long long
#define MAX_MEMORY_TRANSFER_SIZE 1016 /* MDI seems to be limited to 1024 including checksum. */
#define BAD_VALUE 0x0bad0bad0bad0badull
#define CORE_IN_MASK(core, mask) ((mask) & (1ull<<(core)))

typedef enum
{
    GDB_PROTOCOL_BASIC,
    GDB_PROTOCOL_MACRAIGOR,
    GDB_PROTOCOL_MAJIC,
    GDB_PROTOCOL_BDI3000,
    GDB_PROTOCOL_TRACE32,
} gdb_protocol_extension_t;

typedef enum
{
    GDB_CORE_DEAD,      /* Core doesn't exist or can't be accessed */
    GDB_CORE_RESET,     /* Core exists but is in reset */
    GDB_CORE_RUNNING,   /* Core is running code */
    GDB_CORE_STOPPED,   /* Core is stopped and under GDB control */
    GDB_CORE_PAUSED     /* Core should be running, but we stopped it temporarily to issue comamnds */
} gdb_core_state_t;

typedef struct
{
    int sock_fd;            /* Socket for talking to the core */
    gdb_core_state_t state; /* Current core state */
    struct sockaddr_in remote_addr;
} gdb_core_info_t;

/* Each bit represents a valid COP0 register bit=reg*8+sel VALID_COP0[bit/64] |= 1<<(bit%63) */
extern const uint64_t OCTEON_REMOTE_VALID_COP0[];

static const char *MACRAIGOR_COP0_NAMES[0x100];
static gdb_protocol_extension_t gdb_protocol = GDB_PROTOCOL_BASIC;
static gdb_core_info_t gdb_core[64];

static void enable_64bit_addressing(int core);
static int do_command(const int core, const char *command, char *response,
                      const int response_size);


/**
 * Used by do_command() to open a socket to the gdb stub on
 * demand. Only core zero connects when open is called, the
 * other cores connect on demand.
 *
 * @param core   Core to create a connection for
 *
 * @return Zero on success, negative on failure.
 */
static int open_connection(int core)
{
    char response[1024];

    gdb_core[core].sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (gdb_core[core].sock_fd == -1)
    {
        octeon_remote_perror(-1, "socket");
        return -1;
    }

    /* Set NODELAY so round trip times are reduced */
    int i=1;
    setsockopt(gdb_core[core].sock_fd, IPPROTO_TCP, TCP_NODELAY, &i, 4);

    int status = connect(gdb_core[core].sock_fd, (struct sockaddr *)&gdb_core[core].remote_addr, sizeof(struct sockaddr_in));
    if (status == -1)
    {
        octeon_remote_perror(-1, "connect");
        return -1;
    }
    octeon_remote_debug(2, "Socket for core %d established\n", core);

    /* Send an ACK in case the stub is waiting for one */
    send(gdb_core[core].sock_fd, "+", 1, MSG_DONTWAIT);

    /* Throw away any pending output from the stub. Some EJTAGs seem to
        keep unread responses between socket connections */
    usleep(10000);
    do
    {
        i = recv(gdb_core[core].sock_fd, response, sizeof(response), MSG_DONTWAIT);
    } while (i > 0);

    /* Verify the connection */
    i = do_command(core, "?", response, sizeof(response));
    if (i<0)
    {
        octeon_remote_debug(1, "GDB protocol verification failed\n");
        return -1;
    }
    return 0;
}

static int process_response_packet(const int core, char *response, const int response_size, int length)
{
    /*
     * Handle chatter from the targets here.  These messages can
     * provide useful feedback to the user.  They are displayed
     * without the line prefixes to avoid clutter.
     */
    if ((response[0] == 'O') && (response[1] != 'K'))
    {
        char out_response[response_size * 2];
        char *ptr = out_response;
        char *decode = response + 1; /* Skip 'O' */
        while (*decode && (ptr < response+response_size))
        {
            int v = '.';
            sscanf(decode, "%02x", &v);
            decode+=2;
            *ptr++ = v;
            if (ptr >= response+response_size)
            {
                octeon_remote_debug(4, "GDB monitor reply truncated. (Buffer filled!!)\n");
                *(ptr-1) = 0;
                break;
            }
        }
        *ptr = 0;
        octeon_remote_output(2, "%s", out_response);
    }

#if 0
    if (gdb_protocol == GDB_PROTOCOL_MACRAIGOR)
    {
        if ((response[0] == 'O') && (response[1] == 'K'))
        {
            if (length > 2)
            {
                /*
                 * When we can't anticipate the stopped signal from
                 * the Macraigor probe then we might not read or ack
                 * it before sending another command.  When that
                 * happens Macraigor resends the response combined
                 * with the 'T' signal.  We work around this by
                 * recognizing the OK response and handling any
                 * subsequent data as a separate response. */

                int i;

                /* Shift concatenated response down */
                for (i=2; i<length; ++i)
                {
                    response[i-2] = response[i];
                }
                octeon_remote_debug(-1, "Concatenated response received from probe\n");
                length -= 2;
                response[length] = 0;
            }
        }
    }
#endif

    if (response[0] == 'T')
    {
        int signal;

        sscanf(response+1, "%02x", &signal);
        octeon_remote_debug(4, "GDB Signal Received: %d\n", signal);

        gdb_core[core].state = GDB_CORE_STOPPED;

        /* The Macraigor probe can stop cores unexpectedly.
           Particularly when switching cores.  When it does it signals
           with the 'T' packet.  Accept this as a signal to resync the
           internal stopped state before issuing the next command. */
        if (gdb_protocol == GDB_PROTOCOL_MACRAIGOR)
        {
            if (length > 3)
            {
                /*
                 * When we can't anticipate the stopped signal from the
                 * Macraigor probe then we might not read or ack it before
                 * sending another command.  When that happens Macraigor
                 * resends the 'T' signal combined with the response to the
                 * subsequent command.  We work around this by recognizing the
                 * $T05 response and handling any subsequent data as a
                 * separate reponse. */

                int i;

                /* Shift concatenated response down */
                for (i=3; i<length; ++i)
                {
                    response[i-3] = response[i];
                }
                octeon_remote_debug(-1, "Concatenated response received from probe\n");
                length -= 3;
                response[length] = 0;
            }
        }
    }

    /* Some commands return ENN for error. */
    if ((response[0] == 'E') && (length == 3))
    {
        int error;
        sscanf(response+1, "%02x", &error);
        octeon_remote_debug(-1, "GDB Reply Error: %d\n", error);
        return -1;
    }
    return length;
}


/**
 * Read a GDB stub response and return it without any processing.
 * It reads a GDB packet of the form "$data#csum". If an incorrect
 * checksum is read, the response is thrown away and a NAK is sent.
 * If the checksum is correct, an ACK is sent and the response is
 * returned.
 *
 * @param core     Core to read a response for.
 * @param response Buffer for the response.
 * @param response_size
 *                 Length of the response buffer.
 * @param already_got_start
 *                 If non zero, the beginning "$" has already been read and should
 *                 not be looked for. This is useful if you already read the begin
 *                 marker and then determined that you need to read a response.
 *
 * @return Numbr of bytes in the response, or negative on failure.
 */
static int do_read_response(const int core, char *response, const int response_size, int already_got_start)
{
    char c;
    int length;
    int csum;
    int retries = 3;

    if (already_got_start)
        goto skip_start;

retry:
    /* Read the first character of the response */
    if (recv(gdb_core[core].sock_fd, &c, 1, MSG_WAITALL) != 1)
    {
        octeon_remote_perror(-1, "gdbremote read()#1");
        return -1;
    }

    /* The response should always start with a '$' */
    if (c != '$')
    {
        octeon_remote_debug(-1, "GDB didn't send a packet start\n");
        return -1;
    }

skip_start:
    /* Loop reading bytes until we get to '#', marking the checksum */
    length = 0;
    csum = 0;
    while (1)
    {
        if (length >= response_size)
        {
            octeon_remote_debug(-1, "GDB response larger than buffer. len=%d\n", length);
            octeon_remote_debug(4, "GDB response: %*s", response_size, response);
            return -1;
        }
        if (recv(gdb_core[core].sock_fd, response+length, 1, MSG_WAITALL) != 1)
        {
            octeon_remote_perror(-1, "gdbremote read()#2");
            return -1;
        }
        if (response[length] == '$')
        {
            octeon_remote_debug(-1, "GDB sent a packet start unexpectedly\n");
            return -1;
        }
        /* Sum each character until we get the '#' */
        if (response[length] == '#')
            break;
        csum += response[length];
        length++;
    }
    response[length] = 0;       /* Terminate the response */

    /* Read the checksum */
    char csum_str[3];
    if (recv(gdb_core[core].sock_fd, &csum_str, 2, MSG_WAITALL) != 2)
    {
        octeon_remote_perror(-1, "gdbremote read()#3");
        return -1;
    }
    csum_str[2] = 0;

    octeon_remote_debug(4, "GDB Reply: $%s#%s\n", response, csum_str);

    /* Verify the checksum */
    int i = 0;
    sscanf(csum_str, "%02x", &i);
    if (i != (csum & 0xff))
    {
        octeon_remote_debug(4, "GDB reply has wrong CSUM\n");
        if (!retries)
        {
            octeon_remote_debug(-1, "Reply retries exceeded\n");
            return -1;
        }
        retries--;
        /* NAK the reply to the stub and retry */
        send(gdb_core[core].sock_fd, "-", 1, MSG_DONTWAIT);
        goto retry;
    }
    /* ACK the reply to the stub */
    send(gdb_core[core].sock_fd, "+", 1, MSG_DONTWAIT);
    return length;
}


/**
 * Issue a low level command to the GDB stub.
 *
 * @param core     Core to issue command for.
 * @param command  GDB stub comamnd. This can be an empry string, which means we
 *                 read a response without issuing a command.
 * @param response Response buffer. If this is NULL, the command is assumed to not
 *                 generate a response.
 * @param response_size
 *                 Length of the response buffer in bytes.
 *
 * @return Length of the response, or negative on error.
 */
static int do_command(const int core, const char *command, char *response,
                      const int response_size)
{
    int command_len = strlen(command);
    char csum_str[4];
    int csum;
    int i;
    char c;
    int retries = 3;
    int length;

    /* Open a socket if we haven't yet */
    if (gdb_core[core].sock_fd == -1)
    {
        if (open_connection(core))
            return -1;
    }

    /* Guarantee that the response is NULL terminated on error */
    if (response_size)
        response[0] = 0;

    if (command[0] == 0)
    {
        /* Don't issue a command */
    }
    /* Control-C is special as it doesn't have the normal packet start and checksum */
    else if (command[0] == '\003')
    {
        octeon_remote_debug(4, "GDB request: ^C\n");
        if (send(gdb_core[core].sock_fd, "\003", 1, MSG_DONTWAIT) != 1)
        {
            octeon_remote_perror(-1, "gdbremote write()");
            return -1;
        }
    }
    else
    {
        /* Calculate the checksum of the command */
        csum = 0;
        for (i=0; i<command_len; i++)
            csum += command[i];
        csum &= 0xff;
        sprintf(csum_str, "#%02x", csum);

        octeon_remote_debug(4, "GDB request: $%s%s\n", command, csum_str);

        /* Retries requried by NAKs start here*/
retry_command:
        /* Send the '$', command, and checksum */
        if (send(gdb_core[core].sock_fd, "$", 1, MSG_MORE) != 1)
        {
            octeon_remote_perror(-1, "gdbremote write()");
            return -1;
        }
        if (send(gdb_core[core].sock_fd, command, command_len, MSG_MORE) != command_len)
        {
            octeon_remote_perror(-1, "gdbremote write()");
            return -1;
        }
        if (send(gdb_core[core].sock_fd, csum_str, 3, MSG_DONTWAIT) != 3)
        {
            octeon_remote_perror(-1, "gdbremote write()");
            return -1;
        }

retry_ack:
        /* Read the target's response to see if the command was accepted */
        if (recv(gdb_core[core].sock_fd, &c, 1, MSG_WAITALL) != 1)
        {
            octeon_remote_perror(-1, "gdbremote read()#1");
            return -1;
        }
        switch (c)
        {
            case '+':
                octeon_remote_debug(4, "GDB ACK\n");
                /* If the command has no reply then we're done */
                if (response_size == 0)
                    return 0;
                break;
            case '-':
                octeon_remote_debug(4, "GDB NAK\n");
                /* Received a NAK, try again */
                if (!retries)
                {
                    octeon_remote_debug(-1, "Command retries exceeded\n");
                    return -1;
                }
                retries--;
                goto retry_command;
            case '$':
                {
                    char unexpected_response[256];
                    octeon_remote_debug(4, "Processing unexpected command response.\n");
                    length = do_read_response(core, unexpected_response, sizeof(unexpected_response), 1);
                    process_response_packet(core, unexpected_response, sizeof(unexpected_response), length);
                    goto retry_ack;
                }
                break;
            default:
                octeon_remote_debug(-1, "Unexpected command reply '%c' (expected +/-)\n", c);
                return -1;
        }
    }

    /* Don't look for a response if we're not expecting one. The read
       could hang. */
    if (response_size == 0)
        return 0;

    length = do_read_response(core, response, response_size, 0);
    if (length > 0)
        return process_response_packet(core, response, response_size, length);
    else
        return length;
}


/**
 * Based on the state of ciu_pp_rst, update which cores are in
 * reset.
 */
static void do_update_reset_state(void)
{
    int core;

    /* Read which cores are in reset */
    int num_cores = cvmx_pop(octeon_remote_read_csr(CVMX_CIU_FUSE));
    uint64_t pp_rst = octeon_remote_read_csr(CVMX_CIU_PP_RST);

    for (core=0; core<64; core++)
    {
        if (core >= num_cores)
            gdb_core[core].state = GDB_CORE_DEAD;
        else if (CORE_IN_MASK(core, pp_rst))
            gdb_core[core].state = GDB_CORE_RESET;
        else if ((gdb_core[core].state == GDB_CORE_RESET) ||
                 (gdb_core[core].state == GDB_CORE_DEAD))
            gdb_core[core].state = GDB_CORE_STOPPED;
    }
}


/**
 * Called to stop a core and place it under remote control. Errors
 * will be displayed if the core is DEAD or PAUSED. If a core is
 * already stopped, then nothing is done.
 *
 * A core stopped by this function will be put in the PAUSED state.
 *
 * @param core   Core to stop
 *
 * @return The new state of the core
 */
static gdb_core_state_t do_core_stop(int core)
{
    if (gdb_core[core].state == GDB_CORE_RESET)
        do_update_reset_state();

    switch (gdb_core[core].state)
    {
        case GDB_CORE_DEAD:
            octeon_remote_debug(-1, "Dead core %d told to stop\n", core);
            return gdb_core[core].state;
        case GDB_CORE_RESET:
            octeon_remote_debug(-1, "Core %d, in reset, told to stop\n", core);
            /* All the code treats dead and reset the same, so return dead
                to simplify the if statements */
            return GDB_CORE_DEAD;
        case GDB_CORE_STOPPED:
            return gdb_core[core].state;
        case GDB_CORE_PAUSED:
            octeon_remote_debug(5, "Paused core %d told to stop\n", core);
            return gdb_core[core].state;
        case GDB_CORE_RUNNING:
            octeon_remote_debug(5, "Core %d running -> stopped\n", core);
            break;
    }

    char response[64];
    do_command(core, "\003", response, sizeof(response));
    gdb_core[core].state = GDB_CORE_PAUSED;
    return gdb_core[core].state;
}


/**
 * Called to start a core running after it has been stopped or
 * paused. Calling this in a dead or already running core will
 * print an error message.
 *
 * @param core   Core to start running
 */
static void do_core_continue(int core)
{
    switch (gdb_core[core].state)
    {
        case GDB_CORE_DEAD:
            octeon_remote_debug(-1, "Dead core %d told to continue\n", core);
            return;
        case GDB_CORE_RESET:
            octeon_remote_debug(-1, "Core %d, in reset, told to continue\n", core);
            return;
        case GDB_CORE_RUNNING:
            octeon_remote_debug(-1, "Running core %d told to continue\n", core);
            return;
        case GDB_CORE_STOPPED:
            octeon_remote_debug(5, "Core %d stopped -> running\n", core);
            break;
        case GDB_CORE_PAUSED:
            octeon_remote_debug(5, "Core %d paused -> running\n", core);
            break;
    }
    if (do_command(core, "c", NULL, 0) != 0)
        octeon_remote_debug(-1, "Continue command failed\n");
    gdb_core[core].state = GDB_CORE_RUNNING;
}


/**
 * Issue a monitor to the gdb stub to access extended features
 * supplied by the probe. Request and responses are
 * encoded/decoded from hex. Monitor command can have lots of
 * output, so be sure to give large response buffers.
 *
 * @param core     Core to issue montitor comamnd on.
 * @param command  Monitor command in a NULL terminated C string.
 * @param response Response buffer
 * @param response_size
 *                 Size of the response buffer
 *
 * @return Length of the response, or negative on failure.
 */
static int do_monitor_command(const int core, const char *command,
    char *response, const int response_size)
{
    int i;
    int command_len = strlen(command);
    char hex_command[6 + command_len*2 + 1];
    char hex_response[response_size * 2];
    char *ptr = hex_command;

    /* Build the monitor request as a hex string */
    octeon_remote_debug(4, "GDB monitor request: \"%s\"\n", command);
    ptr += sprintf(ptr, "qRcmd,");
    for (i=0;i<command_len;i++)
        ptr += sprintf(ptr, "%02x", 0xff & *command++);
    *ptr = 0;

    /* Stop the core if necessary */
    if (do_core_stop(core) == GDB_CORE_DEAD)
        return -1;

    /* Issue the monitor command */
    int hex_response_len = do_command(core, hex_command, hex_response, sizeof(hex_response));
    if (hex_response_len <= 0)
    {
        if (gdb_core[core].state == GDB_CORE_PAUSED)
            do_core_continue(core);
        *response = 0;
        return hex_response_len;
    }

    /* Start processing the monitor response. We can get multiple responses for
        a single monitor command. The final response should be "OK" or a
        hex encoded message not starting with "O" */
    ptr = response;
    while (1)
    {
        if((hex_response[0] == 'O') && (hex_response[1] == 'K'))
            break;
        char *decode = hex_response;
        if (*decode == 'O')
            decode++;
        while (*decode && (ptr < response+response_size))
        {
            int v = '.';
            sscanf(decode, "%02x", &v);
            decode+=2;
            *ptr++ = v;
            if (ptr >= response+response_size)
            {
                octeon_remote_debug(4, "GDB monitor reply truncated. (Buffer filled!!)\n");
                *(ptr-1) = 0;
                break;
            }
        }
        if (hex_response[0] != 'O')
            break;
        hex_response_len = do_command(core, "", hex_response, sizeof(hex_response));
    }
    *ptr = 0;
    octeon_remote_debug(4, "GDB monitor reply len=%d: \"%s\"\n", (int) strlen(response), response);

    /* Restart the core if we paused it */
    if (gdb_core[core].state == GDB_CORE_PAUSED)
        do_core_continue(core);

    return ptr - response;
}


/**
 * Called when octeon_remote_open() is called using a remote spec
 * for gdbremote.
 *
 * @param remote_spec
 *               String specification of the remote to connect to.
 *
 * @return Zero on success, negative on failure.
 */
static int gdbremote_open(const char *remote_spec)
{
    char local_spec[80];
    char response[32768];

    /* Make a local copy of the remote_spec os we can parse it with strtok */
    strncpy(local_spec,  remote_spec,  sizeof(local_spec));
    local_spec[sizeof(local_spec)-1] = 0;

    /* Parse the spec for the protocol, name, and port */
    /* Remote spec = "gdb:<name>,<tcp_port>" */
    char *save_ptr = NULL;
    const char *spec_type = strtok_r(local_spec, ":", &save_ptr);
    const char *name = strtok_r(NULL, ",", &save_ptr);
    const char *port_str = strtok_r(NULL, ",", &save_ptr);

    if (!spec_type || !name || !port_str)
    {
        octeon_remote_debug(-1, "Illegal protocol \"%s\"\n", remote_spec);
        return -1;
    }
    int port = atoi(port_str);
    octeon_remote_debug(1, "GDB remote %s:%d\n", name, port);

    /* Convert the name into an IP address */
    struct hostent *host_info = gethostbyname(name);
    if (!host_info)
    {
        octeon_remote_perror(-1, "hostname lookup failed\n");
        return -1;
    }

    /* Initialize all the cores as DEAD before we start probing */
    int core;
    for (core=0; core<64; core++)
    {
        memset(gdb_core + core, 0, sizeof(gdb_core[0]));
        gdb_core[core].sock_fd = -1;
        gdb_core[core].state = GDB_CORE_DEAD;
        gdb_core[core].remote_addr.sin_family = AF_INET;
        gdb_core[core].remote_addr.sin_addr.s_addr = ((struct in_addr *)(host_info->h_addr))->s_addr;
        gdb_core[core].remote_addr.sin_port = htons(port + core);
        /* Assume cores are in reset. We'll update the state later */
    }

    octeon_remote_debug(1, "IP address of GDB remote: %s\n", inet_ntoa(gdb_core[0].remote_addr.sin_addr));

    /* Core 0 is currently marked in reset. Change it to stopped */
    do_command(0, "\003", NULL, 0);
    gdb_core[0].state = GDB_CORE_STOPPED;

    /* Issue a monitor help and try to determine the gdb remote
        extensions available */
    do_monitor_command(0, "help", response, sizeof(response));
    if (strstr(response, "set memspace <virtual|physical|#>"))
    {
        gdb_protocol = GDB_PROTOCOL_MACRAIGOR;
        octeon_remote_debug(1, "Detected Macraigor extensions\n");
    }
    else if (strstr(response, "DEBUG MONITOR COMMANDS"))
    {
        gdb_protocol = GDB_PROTOCOL_MAJIC;
        octeon_remote_debug(1, "Detected Majic extensions\n");
    }
    else if (strstr(response, "MDD   [<address>] [<count>]"))
    {
        gdb_protocol = GDB_PROTOCOL_BDI3000;
        octeon_remote_debug(1, "Detected BDI3000 extensions\n");
    }
    else if (strstr(response, "TRACE32"))
    {
        gdb_protocol = GDB_PROTOCOL_TRACE32;
        octeon_remote_debug(1, "Detected TRACE32 extensions\n");
    }

    /* Force core 0 to allow 64bit addressing for CSR/memory access */
    enable_64bit_addressing(0);

    /* Now that everything is setup, read the CIU and figure out which
        cores are in reset */
    do_update_reset_state();

    /* Leave the core in the running state */
    do_core_continue(0);
    return 0;
}


/**
 * Called when octeon_remote_close() is called.
 */
static void gdbremote_close(void)
{
    int core;
    for (core=0; core<64; core++)
    {
        if (gdb_core[core].sock_fd != -1)
        {
            close(gdb_core[core].sock_fd);
            gdb_core[core].sock_fd = -1;
        }
        gdb_core[core].state = GDB_CORE_DEAD;
    }
}


/**
 * Read data from a physical memory address.
 *
 * @param buffer_ptr Buffer to put the data in
 * @param physical_address
 *                   Physical address to read from. Bits 63-38 should be zero.
 * @param length     Length to read in bytes
 */
static void gdbremote_read_mem(void *buffer, uint64_t physical_address, int length)
{
    uint8_t *data = buffer;
    char command[1 + 16 + 1 + 4 + 1];
    char response[MAX_MEMORY_TRANSFER_SIZE*2 + 1];
    int core = 0;
    int i;

    /* Access memory through XKPHYS */
    physical_address |= 1ull<<63;

    /* Stop the core if it is currently running */
    if (do_core_stop(core) == GDB_CORE_DEAD)
        return;

    while (length)
    {
        /* Limit each transfer */
        int seg_length = length;
        if (seg_length > MAX_MEMORY_TRANSFER_SIZE)
            seg_length = MAX_MEMORY_TRANSFER_SIZE;
        sprintf(command, "m%llx,%x", (ULL)physical_address, seg_length);
        int rlength = do_command(core, command, response, sizeof(response));
        if (rlength != seg_length*2)
            octeon_remote_debug(-1, "Memory read: Unexpected response length %d\n", rlength);
        for (i=0; i<seg_length; i++)
        {
            int v;
            sscanf(response + i*2, "%02x", &v);
            *data++ = v;
        }
        physical_address += seg_length;
        length -= seg_length;
    }

    /* Restart the core if it should be running */
    if (gdb_core[core].state == GDB_CORE_PAUSED)
        do_core_continue(core);
}


/**
 * Write data to a physical memory address
 *
 * @param physical_address
 *                   Physical address to write to. Bits 63-38 should be zero.
 * @param buffer_ptr Buffer containing the data to write
 * @param length     Number of bytes to write
 */
static void gdbremote_write_mem(uint64_t physical_address, const void *buffer, int length)
{
    const uint8_t *data = buffer;
    char command[1 + 16 + 1 + 4 + 1 + MAX_MEMORY_TRANSFER_SIZE*2 + 1];
    char response[64];
    int core = 0;
    int i;

    /* Access memory through XKPHYS */
    physical_address |= 1ull<<63;

    /* Stop the core if it is currently running */
    if (do_core_stop(core) == GDB_CORE_DEAD)
        return;

    while (length)
    {
        /* Limit each transfer */
        int seg_length = length;
        if (seg_length > MAX_MEMORY_TRANSFER_SIZE)
            seg_length = MAX_MEMORY_TRANSFER_SIZE;
        char *ptr = command;
        ptr += sprintf(ptr, "M%llx,%x:", (ULL)physical_address, seg_length);
        for (i=0; i<seg_length; i++)
            ptr += sprintf(ptr, "%02x", 0xff & *data++);
        int rlength = do_command(core, command, response, sizeof(response));
        /* Response is suppose to be "OK", but Macraigor returns "OKOK" */
        if ((rlength != 2) && (rlength != 4))
            octeon_remote_debug(-1, "Memory write: Unexpected response length %d\n", rlength);
        physical_address += seg_length;
        length -= seg_length;
    }

    /* Restart the core if it should be running */
    if (gdb_core[core].state == GDB_CORE_PAUSED)
        do_core_continue(core);
}


/**
 * Return the Processor ID of the Octeon. This should be identical
 * to the value found in the COP0 PRID register
 *
 * @return Processor ID
 */
static uint32_t gdbremote_get_model(void)
{
    /* Directly read the Processor ID register */
    return octeon_remote_read_register(0, 0x100 + 15*8 + 0);
}


/**
 * Take the cores in the bit vector out of the debug exception.
 *
 * @param start_mask Cores to start
 */
static void gdbremote_start_cores(uint64_t start_mask)
{
    int core;

    for (core=0;core<64; core++)
    {
        if (CORE_IN_MASK(core, start_mask) &&
            (gdb_core[core].state == GDB_CORE_STOPPED))
            do_core_continue(core);
    }
}


/**
 * Cause the cores in the stop mask to take a debug exception
 *
 * @param stop_mask Cores to stop
 */
static void gdbremote_stop_cores(uint64_t stop_mask)
{
    int core;

    for (core=0;core<64; core++)
    {
        if (CORE_IN_MASK(core, stop_mask) &&
            (gdb_core[core].state == GDB_CORE_RUNNING))
        {
            do_core_stop(core);
            /* State will be paused, change it to stopped */
            gdb_core[core].state = GDB_CORE_STOPPED;
        }
    }
}


/**
 * Get a mask where each bit represents a running core. If a core
 * is in the debug exception handler, it's bit will be zero.
 *
 * @return Bit set for every running core
 */
static uint64_t gdbremote_get_running_cores(void)
{
    uint64_t mask = 0;
    int core;

    for (core=0; core<64; core++)
        if (gdb_core[core].state == GDB_CORE_RUNNING)
            mask |= 1ull<<core;

    return mask;
}


/**
 * Read a register
 *
 * @param core   Core to read
 * @param reg    Register to read
 *
 * @return Value of the register
 */
static uint64_t gdbremote_read_register(int core, int reg)
{
    unsigned long long result;
    int reg_id;
    char response[128*16 + 1];
    int rlength;

    if (reg<0)
        return BAD_VALUE;
    else if (reg == 0)
        return 0;
    else if (reg < 32)
        reg_id = reg;
    else if (reg == 32) /* Lo */
        reg_id = 33;
    else if (reg == 33) /* Hi */
        reg_id = 34;
    else if (reg == 0x100+12*8+0) /* Status */
        reg_id = 32;
    else if (reg == 0x100+8*8+0) /* BadVaddr */
        reg_id = 35;
    else if (reg == 0x100+13*8+0) /* Cause */
        reg_id = 36;
    else if (reg == 0x100+24*8+0) /* PC */
        reg_id = 37;
    else if ((reg >= 0x100) && (reg< 0x200) && (OCTEON_REMOTE_VALID_COP0[(reg-0x100)/64] & (1ull<<(reg&63))))
    {
        char command[256];
        reg_id = (reg&0xff) >> 3;
        int select = reg & 0x7;
        result = BAD_VALUE;
        switch (gdb_protocol)
        {
            case GDB_PROTOCOL_BASIC:
                /* The normal GDB protocol doesn't support COP0 */
                break;
            case GDB_PROTOCOL_MACRAIGOR:
                sprintf(command, "reg %s", MACRAIGOR_COP0_NAMES[reg - 0x100]);
                rlength = do_monitor_command(core, command, response, sizeof(response));
                /* Macraigor probes respond with a name, some white space, and then
                    16 hex digits. We dump unwanted stuff into the command buffer. */
                if (rlength > 16)
                    sscanf(response, "%s %016llx", command, &result);
                break;
            case GDB_PROTOCOL_MAJIC:
                sprintf(command, "DD g0_%d", reg_id + select*32);
                rlength = do_monitor_command(core, command, response, sizeof(response));
                /* Majic probes respond with a name, some white space, and then
                    16 hex digits. We dump unwanted stuff into the command buffer. */
                if (rlength > 16)
                    sscanf(response, "%s %016llx", command, &result);
                break;
            case GDB_PROTOCOL_BDI3000:
                sprintf(command, "RDCP0 %d", reg_id + select*256);
                rlength = do_monitor_command(core, command, response, sizeof(response));
                /* BDI3000 probes respond with "CP0", some white space, a
                    decimal register number, some white sapce, a colon, some
                    more whitespace, and then 16 hex digits. We dump unwanted
                    stuff into the command buffer. */
                if (rlength > 16)
                    sscanf(response, "%s %s %s %llx", command, command, command, &result);
                break;
            case GDB_PROTOCOL_TRACE32:
                sprintf(command, "eval data.quad(CP0:0x%x)", reg_id + select*32);
                /* TRACE32 responds with a 1-16 character hex value of the register content */
                rlength = do_monitor_command(core, command, response, sizeof(response));
                if (rlength > 0)
                    sscanf(response, "%llx", &result);
                break;
        }
        return result;
    }
    else
        return BAD_VALUE;

    if (do_core_stop(core) == GDB_CORE_DEAD)
        return BAD_VALUE;

    rlength = do_command(core, "g", response, sizeof(response));

    if (gdb_core[core].state == GDB_CORE_PAUSED)
        do_core_continue(core);

    /* GDB should respond with at least 37 registers */
    if (rlength < 37*16)
    {
        octeon_remote_debug(-1, "Register read: Unexpected response length %d\n", rlength);
        return BAD_VALUE;
    }

    if (rlength >= (reg_id+1)*16)
        sscanf(response + reg_id*16, "%016llx", &result);
    else
        result = BAD_VALUE;

    return result;
}


/**
 * Write a register
 *
 * @param core   Core to write
 * @param reg    Register to write
 * @param value  Value to write
 */
static void gdbremote_write_register(int core, int reg, uint64_t value)
{
    int reg_id;
    char command[1 + 4 + 1 + 16 + 1];
    char response[256];

    if (reg<=0)
        return;
    else if (reg < 32)
        reg_id = reg;
    else if (reg == 32) /* Lo */
        reg_id = 33;
    else if (reg == 33) /* Hi */
        reg_id = 34;
    else if ((reg >= 0x100) && (reg< 0x200) && (OCTEON_REMOTE_VALID_COP0[(reg-0x100)/64] & (1ull<<(reg&63))))
    {
        char command[256];
        reg_id = (reg&0xff) >> 3;
        int select = reg & 0x7;
        switch (gdb_protocol)
        {
            case GDB_PROTOCOL_BASIC:
                /* The normal GDB protocol doesn't support COP0 */
                break;
            case GDB_PROTOCOL_MACRAIGOR:
                sprintf(command, "reg %s 0x%llx", MACRAIGOR_COP0_NAMES[reg - 0x100], (ULL)value);
                do_monitor_command(core, command, response, sizeof(response));
                break;
            case GDB_PROTOCOL_MAJIC:
                sprintf(command, "ED g0_%d = 0x%llx", reg_id + select*32, (ULL)value);
                do_monitor_command(core, command, response, sizeof(response));
                break;
            case GDB_PROTOCOL_BDI3000:
                sprintf(command, "RMCP0 %d 0x%llx", reg_id + select*256, (ULL)value);
                do_monitor_command(core, command, response, sizeof(response));
                break;
            case GDB_PROTOCOL_TRACE32:
                sprintf(command, "B::DATA.SET CP0:0x%x 0x%llx", reg_id + select*32, (ULL)value);
                do_monitor_command(core, command, response, sizeof(response));
                break;
        }
        return;
    }
    else
        return;

    sprintf(command, "P%x,%llx", reg_id, (ULL)value);

    if (do_core_stop(core) == GDB_CORE_DEAD)
        return;

    int rlength = do_command(core, command, response, sizeof(response));

    if (gdb_core[core].state == GDB_CORE_PAUSED)
        do_core_continue(core);

    /* Response is suppose to be "OK", but Macraigor returns "OKOK" */
    if ((rlength != 2) && (rlength != 4))
        octeon_remote_debug(-1, "Register write: Unexpected response length %d\n", rlength);
}


/**
 * Get all registers, COP0, TLB, etc for a core
 *
 * @param core      Core to get state info for
 * @param registers All of the core's internal state
 *
 * @return Zero on success, negative on failure
 */
static int gdbremote_get_core_state(int core, octeon_remote_registers_t *registers)
{
    char response[128*16 + 1];
    int r;

    /* Default all registers to bad values */
    for (r=0; r<0x200; r++)
        registers->regs[r>>8][r&0xff] = BAD_VALUE;

    if (do_core_stop(core) == GDB_CORE_DEAD)
        return -1;

    /* Get all of the general registers from GDB */
    int rlength = do_command(core, "g", response, sizeof(response));

    /* GDB should respond with at least 37 registers */
    if (rlength < 37*16)
    {
        octeon_remote_debug(-1, "Core state: Unexpected response length %d\n", rlength);
        if (gdb_core[core].state == GDB_CORE_PAUSED)
            do_core_continue(core);
        return -1;
    }

    /* Read the 32 general purpose registers */
    for (r=0; r<32; r++)
        sscanf(response+r*16, "%016llx", (ULL*)&(registers->regs[0][r]));

    /* Lo */
    sscanf(response+33*16, "%016llx", (ULL*)&(registers->regs[0][32]));
    /* Hi */
    sscanf(response+34*16, "%016llx", (ULL*)&(registers->regs[0][33]));

    /* COP0 */
    for (r=0x100; r<0x200; r++)
        registers->regs[r>>8][r&0xff] = gdbremote_read_register(core, r);

    // FIXME: TLB

    if (gdb_core[core].state == GDB_CORE_PAUSED)
        do_core_continue(core);

    return 0;
}


/**
 * Set all registers, COP0, TLB, etc for a given core
 *
 * @param core      Core to set state for
 * @param registers All date for the core
 *
 * @return Zero on success, negative on failure
 */
static int gdbremote_set_core_state(int core, const octeon_remote_registers_t *registers)
{
    int r;

    if (do_core_stop(core) == GDB_CORE_DEAD)
        return -1;

    for (r=0; r<0x200; r++)
        gdbremote_write_register(core, r, registers->regs[r>>8][r&0xff]);

    // FIXME: TLB

    if (gdb_core[core].state == GDB_CORE_PAUSED)
        do_core_continue(core);

    return 0;
}


/**
 * Enable 64bit addressing so we can get to CSRs
 *
 * @param core   Core to enable accesses for.
 */
static void enable_64bit_addressing(int core)
{
    /* Force 64 bit addressing on so we can access CSRs */
    uint64_t status = octeon_remote_read_register(core, 0x100 + 12*8 + 0);
    status |= 1<<7;
    octeon_remote_write_register(core, 0x100 + 12*8 + 0, status);
}


/**
 * Reset the core/chip
 *
 * @param stop_core Stop core 0 from booting.
 *
 * @return Zero on success, negative on failure
 */
static int gdbremote_reset(int stop_core)
{
    int i;
    int core = 0;
    char response[256];

    if (do_core_stop(core) == GDB_CORE_DEAD)
        return -1;
    gdb_core[core].state = GDB_CORE_STOPPED;

    switch (gdb_protocol)
    {
        case GDB_PROTOCOL_BASIC:
            octeon_remote_debug(-1, "GDB stub doesn't support reset\n");
            break;
        case GDB_PROTOCOL_MACRAIGOR:
            do_monitor_command(core, "reset", response, sizeof(response));
            break;
        case GDB_PROTOCOL_MAJIC:
            do_monitor_command(core, "rt", response, sizeof(response));
            break;
        case GDB_PROTOCOL_BDI3000:
            do_monitor_command(core, "reset halt", response, sizeof(response));
            break;
        case GDB_PROTOCOL_TRACE32:
            do_monitor_command(core, "B::SYSTEM.UP", response, sizeof(response));
            break;
    }

    /* Wait for possible response */
    usleep(500000);
    do
    {
        i = recv(gdb_core[core].sock_fd, response, sizeof(response), MSG_DONTWAIT);
    } while (i > 0);

    /* Issues a query command to make sure stub is working */
    do_command(core, "?", response, sizeof(response));

    enable_64bit_addressing(0);

    for (core=1; core<64; core++)
        if (gdb_core[core].state != GDB_CORE_DEAD)
            gdb_core[core].state = GDB_CORE_RESET;

    /* Leave the core in the running state */
    if (!stop_core)
        do_core_continue(0);
    return 0;
}


/**
 * Sample performance / profiling information for a set of cores
 * minimizing the performance impact of taking the sample. The
 * amount of data returned may vary depending on the remote
 * protocol used, but at least the program counter for each core
 * will normally be supported. Values that could not be sampled
 * will be zero.
 *
 * @param coremask Each set bit represent a core that should be sampled.
 * @param sample   Array of samples to populate. Note that the sample array must
 *                 contain elements for cores not set in the coremask. These may
 *                 be filled with sample data even if the coremask bit is clear.
 *                 Some transports may sample all core efficiently and yield
 *                 unwanted core samples.
 *
 * @return Zero on success, negative on failure
 */
static int gdbremote_get_sample(uint64_t coremask, octeon_remote_sample_t sample[64])
{
    int core;
    for (core=0; core<64; core++)
    {
        if ((1ull<<core) & coremask)
        {
            if (do_core_stop(core) == GDB_CORE_DEAD)
                continue;
            sample[core].pc = gdbremote_read_register(core, 0x100 + 24*8 + 0);
            sample[core].perf_count[0] = gdbremote_read_register(core, 0x100 + 25*8 + 1);
            sample[core].perf_count[1] = gdbremote_read_register(core, 0x100 + 25*8 + 3);
            if (gdb_core[core].state == GDB_CORE_PAUSED)
                do_core_continue(core);
        }
    }
    return 0;
}


/**
 * Initialize the pointers needed for octeon_remote to work over GDB remote
 * stubs. This doesn't actually create a connection, just setup internal data
 * structures.
 *
 * @param remote_funcs
 *               Function pointers to be populated
 *
 * @return Zero on success, negative on failure
 */
int octeon_remote_gdbremote(octeon_remote_funcs_t *remote_funcs)
{
    remote_funcs->open = gdbremote_open;
    remote_funcs->close = gdbremote_close;
    remote_funcs->read_mem = gdbremote_read_mem;
    remote_funcs->write_mem = gdbremote_write_mem;
    remote_funcs->get_model = gdbremote_get_model;
    remote_funcs->start_cores = gdbremote_start_cores;
    remote_funcs->stop_cores = gdbremote_stop_cores;
    remote_funcs->get_running_cores = gdbremote_get_running_cores;
    remote_funcs->get_core_state = gdbremote_get_core_state;
    remote_funcs->set_core_state = gdbremote_set_core_state;
    remote_funcs->read_register = gdbremote_read_register;
    remote_funcs->write_register = gdbremote_write_register;
    remote_funcs->reset = gdbremote_reset;
    remote_funcs->get_sample = gdbremote_get_sample;

    /* The Macraigor probes must use names instead of program friendly
        numbers. These names corespond to teh COP0 registers for CN3XXX and
        CN5XXX chips. FIXME: This table will need updates for other chips */
    MACRAIGOR_COP0_NAMES[ 0*8 + 0] = "inx";
    MACRAIGOR_COP0_NAMES[ 1*8 + 0] = "rand";
    MACRAIGOR_COP0_NAMES[ 2*8 + 0] = "elo0";
    MACRAIGOR_COP0_NAMES[ 3*8 + 0] = "elo1";
    MACRAIGOR_COP0_NAMES[ 4*8 + 0] = "ctxt";
    MACRAIGOR_COP0_NAMES[ 5*8 + 0] = "pgmask";
    MACRAIGOR_COP0_NAMES[ 5*8 + 1] = "pagegrain";
    MACRAIGOR_COP0_NAMES[ 6*8 + 0] = "wired";
    MACRAIGOR_COP0_NAMES[ 7*8 + 0] = "hwrena";
    MACRAIGOR_COP0_NAMES[ 8*8 + 0] = "badvaddr";
    MACRAIGOR_COP0_NAMES[ 9*8 + 0] = "count";
    MACRAIGOR_COP0_NAMES[10*8 + 0] = "ehi";
    MACRAIGOR_COP0_NAMES[11*8 + 0] = "compare";
    MACRAIGOR_COP0_NAMES[12*8 + 0] = "status";
    MACRAIGOR_COP0_NAMES[12*8 + 1] = "intctl";
    MACRAIGOR_COP0_NAMES[12*8 + 2] = "srsctl";
    MACRAIGOR_COP0_NAMES[13*8 + 0] = "cause";
    MACRAIGOR_COP0_NAMES[14*8 + 0] = "epc";
    MACRAIGOR_COP0_NAMES[15*8 + 0] = "prid";
    MACRAIGOR_COP0_NAMES[15*8 + 1] = "ebase";
    MACRAIGOR_COP0_NAMES[16*8 + 0] = "config";
    MACRAIGOR_COP0_NAMES[16*8 + 1] = "config1";
    MACRAIGOR_COP0_NAMES[16*8 + 2] = "config2";
    MACRAIGOR_COP0_NAMES[16*8 + 3] = "config3";
    MACRAIGOR_COP0_NAMES[18*8 + 0] = "iwatchlo";
    MACRAIGOR_COP0_NAMES[18*8 + 1] = "dwatchlo";
    MACRAIGOR_COP0_NAMES[19*8 + 0] = "iwatchhi";
    MACRAIGOR_COP0_NAMES[19*8 + 1] = "dwatchhi";
    MACRAIGOR_COP0_NAMES[20*8 + 0] = "xcontext";
    MACRAIGOR_COP0_NAMES[23*8 + 0] = "debug";
    MACRAIGOR_COP0_NAMES[25*8 + 0] = "perfcntctl";
    MACRAIGOR_COP0_NAMES[25*8 + 2] = "perfcntctl1";
    MACRAIGOR_COP0_NAMES[24*8 + 0] = "depc";
    MACRAIGOR_COP0_NAMES[25*8 + 1] = "perfcntcnt";
    MACRAIGOR_COP0_NAMES[25*8 + 3] = "perfcntcnt1";
    MACRAIGOR_COP0_NAMES[27*8 + 0] = "icacheerr";
    MACRAIGOR_COP0_NAMES[27*8 + 1] = "dcacheerr";
    MACRAIGOR_COP0_NAMES[28*8 + 0] = "icachetaglo";
    MACRAIGOR_COP0_NAMES[28*8 + 1] = "icachedatalo";
    MACRAIGOR_COP0_NAMES[28*8 + 2] = "dcachetaglo";
    MACRAIGOR_COP0_NAMES[28*8 + 3] = "dcachedatalo";
    MACRAIGOR_COP0_NAMES[29*8 + 1] = "icachedatahi";
    MACRAIGOR_COP0_NAMES[29*8 + 2] = "dcachetaghi";
    MACRAIGOR_COP0_NAMES[29*8 + 3] = "dcachedatahi";
    MACRAIGOR_COP0_NAMES[30*8 + 0] = "errorepc";
    MACRAIGOR_COP0_NAMES[31*8 + 0] = "dsave";
    MACRAIGOR_COP0_NAMES[ 9*8 + 7] = "cvmctl";
    MACRAIGOR_COP0_NAMES[11*8 + 7] = "memctl";
    MACRAIGOR_COP0_NAMES[ 9*8 + 6] = "cvmcount";
    MACRAIGOR_COP0_NAMES[22*8 + 0] = "multicoredebug";

    /* Octeon 2 COP0 registers */
    MACRAIGOR_COP0_NAMES[ 4*8 + 2] = "userlocal";
    MACRAIGOR_COP0_NAMES[16*8 + 4] = "config4";
    MACRAIGOR_COP0_NAMES[11*8 + 6] = "powthrottle";
    MACRAIGOR_COP0_NAMES[23*8 + 6] = "debug2";
    MACRAIGOR_COP0_NAMES[31*8 + 2] = "kscratch1";
    MACRAIGOR_COP0_NAMES[31*8 + 3] = "kscratch2";
    MACRAIGOR_COP0_NAMES[31*8 + 4] = "kscratch3";

    return 0;
}

