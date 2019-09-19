/***********************license start************************************
 * OCTEON SDK
 * 
 * Copyright (c) 2003-2005 Cavium Inc.. All rights reserved.
 * 
 * This file, which is part of the OCTEON SDK from Cavium Inc.,
 * contains proprietary and confidential information of Cavium Inc. and
 * its suppliers.
 * 
 * Any licensed reproduction, distribution, modification, or other use of
 * this file or the confidential information or patented inventions
 * embodied in this file is subject to your license agreement with Cavium
 * Networks. Unless you and Cavium Inc. have agreed otherwise in
 * writing, the applicable license terms can be found at:
 * licenses/cavium-license-type2.txt
 * 
 * All other use and disclosure is prohibited.
 * 
 * Contact Cavium Inc. at info@cavium.com for more information.
 **********************license end**************************************/

/**
 * Cavium Octeon Packet I/O Utility
 *
 * This program provides an interface to the packet input and
 * output of the Cavium Octeon Simulator. Packets transfer
 * into the simulator just like they came from the actual
 * Octeon ethernet hardware. Packets may be stored in
 * tcpdump/pcab capture files or in Octeon specific text
 * files. Packets can also be bridged from an existing host
 * ethernet device into the simulator.
 *
 * $Id: oct-packet-io.c 153709 2017-02-06 00:06:50Z cchavva $
 *
 */
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <pthread.h>
#include <signal.h>
#include <setjmp.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <linux/if.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <linux/sockios.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/if_tun.h>
#include "../../../executive/cvmx-version.h"

const uint32_t  TCPDUMP_MAGIC_NUMBER    = 0xa1b2c3d4;
const uint32_t  OCTEON_CYCLES           = 1;            /* Cycles to use for tcpdump file send */
#define         MAX_PACKET_SIZE         65535
#define         MAX_SIMULATOR_PORTS     0xe00		/* The ipd_ports are numbers differents in 68xx */

const char TUN_DEVICE[] = "/dev/net/tun";

/**
 * Tcpdump file header format
 */
typedef struct
{
    uint32_t    magic;              /* TCPDUMP magic number */
    uint16_t    major_version;      /* Should be 2 */
    uint16_t    minor_version;      /* Should be 4 */
    uint32_t    time_zone_offset;   /* Not used - zero */
    uint32_t    time_stamp_accuracy;/* Not used - zero */
    uint32_t    max_capture_length; /* Max size of packet capture - use for buffer size */
    uint32_t    link_layer_type;    /* Link type captured from */
} tcpdump_header_t;

/**
 * Tcpdump packet header format
 */
typedef struct
{
    uint32_t    time_seconds;       /* Timestamp for a tcpdump packet */
    uint32_t    time_microseconds;  /* Timestamp for a tcpdump packet */
    uint32_t    capture_length;     /* Number of bytes in the file for the packet */
    uint32_t    packet_length;      /* May be larger than capture length */
} tcpdump_packet_header_t;

/**
 * Used to track the input files we need to send
 */
typedef struct
{
    char name[1024];                /* Input file to send to the simulator */
    int port;                       /* For tcpdump files, the simulator ethernet port to send on */
} file_info_t;

/**
 * Our current state with respect to the simulator
 */
typedef enum
{
    STATE_INITALIZING,              /**< We're just starting up */
    STATE_NO_CONNECTION,            /**< We're attempting to connect to the simulator */
    STATE_RUNNING,                  /**< Actively sending and receiving from the simulator */
    STATE_DONE                      /**< Nothing more to send. We're waiting for the simulator to die */
} state_t;


/****************************************************************/
/****************************************************************/
/* Global Variables                                             */
/****************************************************************/
/****************************************************************/


/**
 * Name used for the Octeon format output files. This filename
 * may contain a %d to split packets from the simulator by port
 * number.
 */
char octeon_output_name[1024];

/**
 * Name used for the Tcpdump format output files. This filename
 * may contain a %d to split packets from the simulator by port
 * number.
 */
char tcpdump_output_name[1024];

/**
 * If non zero the cycle count written to output files will
 * be forced to 1.
 */
int strip_cycles = 0;

/**
 * Socket to use to communicate with the simulator
 */
int simulator_socket = 0;

/**
 * Socket used to access the bridged device
 */
int bridge_socket = 0;

/**
 * Octeon ethernet port to bridge to
 */
int bridge_port = 0;

/**
 * Max wait time for connection to the simulator
 */
int wait_timeout = 0;

/**
 * Our current state with respect to the simulator
 */
static volatile state_t simulator_state = STATE_INITALIZING;

static int simulator_write_packet(int port, int cycles, char *buffer, int bufferSize);
static uint64_t ntoh64(uint64_t value);

/****************************************************************/
/****************************************************************/
/* tcpdump support routines                                     */
/****************************************************************/
/****************************************************************/


/**
 * Open a tcpdump file for input. Returns a file handle if
 * the file can be opened and is a valid tcpdump file.
 *
 * @param filename Name of the file to open
 * @return File handle or NULL on failure
 */
static FILE *tcpdump_open_file(const char *filename)
{
    FILE *in;
    int count;
    tcpdump_header_t header;

    in = fopen(filename, "r");
    if (in == NULL)
    {
        fprintf(stderr, "Failed to open tcpdump file \"%s\"\n", filename);
        return NULL;
    }

    count = fread(&header, sizeof(header), 1, in);
    if (count != 1)
    {
        fprintf(stderr, "Failed to read tcpdump file header\n");
        fclose(in);
        return NULL;
    }

    if ((header.magic != TCPDUMP_MAGIC_NUMBER) ||
        (header.major_version != 2) ||
        (header.minor_version != 4))
    {
        fprintf(stderr, "Bad tcpdump header\n");
        fclose(in);
        return NULL;
    }

    if (header.max_capture_length > MAX_PACKET_SIZE)
    {
        fprintf(stderr, "tcpdump file contains too big packets\n");
        fclose(in);
        return NULL;
    }

    return in;
}


/**
 * Read a packet from a tcpdump file
 *
 * @param in         File handle to read from
 * @param buffer     Buffer to put the packet in
 * @param bufferSize The size of the buffer
 * @return Size of the packet. Zero on error
 */
static int tcpdump_read_packet(FILE *in, char *buffer, int bufferSize)
{
    tcpdump_packet_header_t header;
    int count;

    memset(buffer, 0, bufferSize);
    count = fread(&header, sizeof(header), 1, in);
    if (count != 1)
        return 0;

    count = fread(buffer, header.capture_length, 1, in);
    if (count != 1)
    {
        fprintf(stderr, "Failed to read tcpdump packet\n");
        return 0;
    }

    return header.capture_length + 4;  /* incl. L2 FCS */
}


/**
 * Append a packet to a tcpdump file
 *
 * @param out        File to append to
 * @param port       Port the packet came in on
 * @param cycles     Cycles the simulator spent sending the packet
 * @param buffer     Packet data
 * @param bufferSize Packet size
 * @return Zero on success
 */
static int tcpdump_write_packet(FILE *out, int port, int cycles, char *buffer, int bufferSize)
{
    int result = 0;
    tcpdump_packet_header_t packet_header;

    /* Check if we need to add the tcpdump header */
    if (ftell(out) == 0)
    {
        tcpdump_header_t file_header;
        file_header.magic = TCPDUMP_MAGIC_NUMBER;
        file_header.major_version = 2;
        file_header.minor_version = 4;
        file_header.time_zone_offset = 0;
        file_header.time_stamp_accuracy = 0;
        file_header.max_capture_length = MAX_PACKET_SIZE;
        file_header.link_layer_type = 1;
        if (fwrite(&file_header, sizeof(file_header), 1, out) != 1)
            result = -1;
    }

    /* strip the L2 FCS */
    bufferSize -= 4;

    /* Write the packet header */
    packet_header.time_seconds = 0;
    packet_header.time_microseconds = 0;
    packet_header.capture_length = bufferSize;
    packet_header.packet_length = bufferSize;
    if (fwrite(&packet_header, sizeof(packet_header), 1, out) != 1)
        result = -1;

    /* Write the packet */
    if (fwrite(buffer, bufferSize, 1, out) != 1)
        result = -1;

    return result;
}


/**
 * Send a tcpdump file to the simulator
 *
 * @param filename File to send
 * @param port     Port to send to
 * @return Zero on success
 */
static int tcpdump_send_file(const char *filename, int port)
{
    FILE *in;
    char buffer[MAX_PACKET_SIZE];
    extern uint32_t crc32(uint32_t seed, const char *p, unsigned size);
    uint32_t fcs;

    in = tcpdump_open_file(filename);
    if (in)
    {
        while (!feof(in))
        {
            int size = tcpdump_read_packet(in, buffer, sizeof(buffer));
            if (size == 0)
		continue;
	    if (size < 0)
		break;

	/* Synthesize FCS at the end */
	fcs = crc32(~0, buffer, size);

	/* NOTE: Works on LE hosts only */
	memcpy(buffer+size, &fcs, sizeof(fcs));
	size += sizeof(fcs);

	simulator_write_packet(port, OCTEON_CYCLES, buffer, size);
        }

        fclose(in);
        return 0;
    }
    else
        return -1;
}


/****************************************************************/
/****************************************************************/
/* Octeon text file support routines                            */
/****************************************************************/
/****************************************************************/


/**
 * Append a packet to an Octeon file
 *
 * @param out        File to append to
 * @param port       Port the packet came in on
 * @param cycles     Cycles the simulator spent sending the packet
 * @param buffer     Packet data
 * @param bufferSize Packet size
 * @return Zero on success
 */
static int octeon_write_packet(FILE *out, int port, int cycles, char *buffer, int bufferSize)
{
    int count;

    fprintf(out, "%d %d %d\n", port, bufferSize, cycles);
    count = 0;
    while (count < bufferSize)
    {
        fprintf(out, "%d 0x%02x%02x%02x%02x%02x%02x%02x%02x\n", port,
               (uint32_t)(uint8_t)*(buffer + count + 0),
               (uint32_t)(uint8_t)*(buffer + count + 1),
               (uint32_t)(uint8_t)*(buffer + count + 2),
               (uint32_t)(uint8_t)*(buffer + count + 3),
               (uint32_t)(uint8_t)*(buffer + count + 4),
               (uint32_t)(uint8_t)*(buffer + count + 5),
               (uint32_t)(uint8_t)*(buffer + count + 6),
               (uint32_t)(uint8_t)*(buffer + count + 7));
        count += 8;
    }
    return 0;
}


/**
 * Send an Octeon file to the simulator
 *
 * @param filename File to send
 * @return Zero on success
 */
static void octeon_send_files(unsigned num_files, file_info_t list[])
{
    FILE *input[256];
    char buffer[256];
    long long packet[64*1024/8];
    unsigned index, fix, fcnt;
    size_t count;
    const int debug = 0;

    for (fcnt = fix = 0; fix < num_files && fix < 256; fix ++) {
	input[fix] = NULL;
	if (list[fix].port != -1)
		continue;
	input[fix] = fopen(list[fix].name, "r");
	if (input[fix] == NULL)
	    perror(list[fix].name);
	else
	    fcnt ++;
	}

    if (debug) fprintf(stderr, "Got %d files\n", fcnt);

    fix = 0;

    /* Write data to simulator one packet at a time */
    while(fcnt)
    {
	int port, cycle, c;
	unsigned len;
	long long value;

        if (fgets(buffer, sizeof(buffer), input[fix]) == NULL) {
		fclose(input[fix]);
		input[fix] = NULL;
		fcnt --;
		if (debug) fprintf(stderr, "File %d closed\n", fix);
		if (fcnt == 0)
		    break;
		/* Move to the next active file */
		fix ++;
		if(fix >= num_files) fix = 0;
		while( input[fix] == NULL) {
		    if(fix >= num_files) fix = 0;
		    else fix ++;
		}
		if (debug) fprintf(stderr, "Fix=%d\n", fix);
	} else {

	    count = sscanf(buffer, "%d %llu %d", &port, &value, &c);

	    if (count == 3) {
		    index = 0;
		    cycle = c;
		    len = value;
	    } else if(count == 2) {
		    sscanf(buffer, "%d %llx", &port, &value);
		    packet[index++] = ntoh64(value);
	    }

	    if ((index*sizeof(packet[0])) >= len) {
		if (debug) fprintf(stderr, "fix%d port %d len %u\n", fix, port, len);
		if (debug) octeon_write_packet(stderr, port, cycle, (void*)packet, len);
		/* FIXME: Add FCS generation here */
                simulator_write_packet(port, cycle, (void *)packet, len);
		/* Move to the next active file */
		fix ++;
		if(fix >= num_files) fix = 0;
		while( input[fix] == NULL) {
		    if(fix >= num_files) fix = 0;
		    else fix ++;
		}
		if (debug) fprintf(stderr, "Fix=%d\n", fix);
	    }
	}
    }
}


/****************************************************************/
/****************************************************************/
/* Bridge support routines                                      */
/****************************************************************/
/****************************************************************/

static int bridge_ifindex;
static int bridge_promisc;

/**
 * Open a linux device for bridging
 *
 * @param devicename Device name to bridge
 * @return Socket handle for the device. Zero on failure.
 */
static int bridge_open_device(const char *devicename)
{
    struct sockaddr_ll addr;
    struct ifreq       req;

    int sock = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (sock <= 0)
    {
        perror(__FUNCTION__);
        return 0;
    }

    strncpy(req.ifr_ifrn.ifrn_name, devicename, sizeof(req.ifr_ifrn.ifrn_name));
    if (ioctl(sock, SIOCGIFINDEX, &req) != 0)
    {
        perror(__FUNCTION__);
        close(sock);
        return 0;
    }
    bridge_ifindex = req.ifr_ifru.ifru_ivalue;
    memset(&addr, 0, sizeof(addr));
    addr.sll_family = AF_PACKET;
    addr.sll_ifindex = bridge_ifindex;
    addr.sll_protocol = htons(ETH_P_ALL);

    if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) != 0)
    {
        perror(__FUNCTION__);
        close(sock);
        sock = 0;
    }

    return sock;
}

/**
 * Open a linux TUN/TAP device
 *
 * @return Socket handle for the device. Zero on failure.
 */
static int tap_open_device(void)
{
	struct ifreq ifr; 
	int fd;

	if((fd = open(TUN_DEVICE, O_RDWR)) < 0) 
		return 0; 
 
	memset(&ifr, 0, sizeof(ifr)); 
 
	/* Flags: IFF_TUN   - TUN device (no Ethernet headers)  
	 *        IFF_TAP   - TAP device   
	 * 
	 *        IFF_NO_PI - Do not provide packet information   
	 */  
	ifr.ifr_flags = IFF_TAP | IFF_NO_PI;
	if(ioctl(fd, TUNSETIFF, (void *) &ifr) < 0)
	{ 
		close(fd); 
		return 0; 
	} 
	fprintf (stderr, "TAP interface is %s\n", ifr.ifr_name);
 
	return fd;
}

/**
 * Loop reading packets from the bridged interface and
 * writing them to the simulator.
 *
 * @return Zero on success
 */
static int bridge_packets(void)
{
    extern uint32_t crc32(uint32_t seed, const char *p, unsigned size);
    char buffer[MAX_PACKET_SIZE];
    int size;
    uint32_t fcs;

    while (bridge_socket)
    {
	struct iovec iovec;
	struct msghdr msghdr;
	struct cmsghdr *cm;
	struct tpacket_auxdata *ad;
	unsigned char cmsg_buf[1024];

        if (simulator_state == STATE_DONE)
            break;
//        size = recv(bridge_socket, buffer, sizeof(buffer), MSG_TRUNC);

	iovec.iov_base = buffer;
	iovec.iov_len = sizeof(buffer);

	memset(&msghdr, 0, sizeof(msghdr));

	msghdr.msg_iov = &iovec;
	msghdr.msg_iovlen = 1;
	msghdr.msg_control = cmsg_buf;
	msghdr.msg_controllen = sizeof(cmsg_buf);

	size = recvmsg(bridge_socket, &msghdr, 0);

	cm = CMSG_FIRSTHDR(&msghdr);
	ad = (void *) CMSG_DATA(cm);

	/* The aux_data can be used to extract VLAN tag data, etc */
#if 0
if(size > 1530) {
fprintf(stderr,"WARN: Rx %d %u\n", size, (unsigned)cm->cmsg_len); 
if(ad != NULL) fprintf(stderr, "dize %d %d\n", size, ad->tp_len);
}
#endif

        if (size < 0) {
		perror(__func__);
		return size;
	}
	if (size == 0)
		continue;

	/* PAD to Ether_Min = 60 before FCS */
	if (size < 60) {
		memset(buffer + size, 0, 60-size);
		size = 60;
	}

	/* Synthesize FCS at the end */
	fcs = crc32(~0, buffer, size);

	/* NOTE: Works on LE hosts only */
	memcpy(buffer+size, &fcs, sizeof(fcs));
	size += sizeof(fcs);

	simulator_write_packet(bridge_port, OCTEON_CYCLES, buffer, size);
    }
    return 0;
}


/**
 * Write a packet out the bridged interface
 *
 * @param port       Port the packet came in on
 * @param cycles     Cycles the simulator spent sending the packet
 * @param buffer     Packet data
 * @param bufferSize Packet size
 * @return Zero on success
 */
static int bridge_write_packet(int port, int cycles, char *buffer, int bufferSize)
{
    int count;
    if (port != bridge_port)
        return -1;

    /* strip the L2 FCS */
    bufferSize -= 4;

    count = write(bridge_socket, buffer, bufferSize);
    if (count == bufferSize)
        return 0;
    else
    {
fprintf(stderr,"len=%d ret=%d errno=%d\n",bufferSize,count,errno);
        perror(__FUNCTION__);
        return -1;
    }
}


/****************************************************************/
/****************************************************************/
/* Generic routines                                             */
/****************************************************************/
/****************************************************************/


/**
 * 64 bit version of the standard ntoh functions
 *
 * @param value  Network order
 * @return Host order
 */
static inline uint64_t ntoh64(uint64_t value)
{
    return ((((value >> 56) & 0xff) <<  0) |
            (((value >> 48) & 0xff) <<  8) |
            (((value >> 40) & 0xff) << 16) |
            (((value >> 32) & 0xff) << 24) |
            (((value >> 24) & 0xff) << 32) |
            (((value >> 16) & 0xff) << 40) |
            (((value >>  8) & 0xff) << 48) |
            (((value >>  0) & 0xff) << 56));
}


/**
 * Called every time a packet is received from the simulator.
 *
 * @param port       The ethernet port the packet was sent out in the simulator
 * @param cycles     Number of cycles spent sending the packet by PKO in the
 *                   simulator.
 * @param buffer     The packet
 * @param bufferSize The size of the packet including the ethernet CRC
 */
static void log_packet(int port, int cycles, char *buffer, int bufferSize)
{
    char name[1024];
    FILE *out;

    if (strip_cycles)
        cycles = 1;

    if (octeon_output_name[0])
    {
        sprintf(name, octeon_output_name, port);
        if (strcmp(name, "/dev/null") != 0)
        {
            out = fopen(name, "a");
            if (out)
            {
                octeon_write_packet(out, port, cycles, buffer, bufferSize);
                fclose(out);
            }
            else
                fprintf(stderr, "Failed to write to \"%s\"\n", name);
        }
    }

    if (tcpdump_output_name[0])
    {
        sprintf(name, tcpdump_output_name, port);
        if (strcmp(name, "/dev/null") != 0)
        {
            out = fopen(name, "a");
            if (out)
            {
                tcpdump_write_packet(out, port, cycles, buffer, bufferSize);
                fclose(out);
            }
            else
                fprintf(stderr, "Failed to write to \"%s\"\n", name);
        }
    }

    if (bridge_socket)
    {
	/* Send packet down to bridge, tick on screen */
        if (bridge_write_packet(port, cycles, buffer, bufferSize) == 0)
		putchar('.');
	else
		putchar('!');
	fflush(stdout);
    }
    else if ((octeon_output_name[0] == 0) && (tcpdump_output_name[0] == 0))
    {
	/* Default log data to stdout */
        octeon_write_packet(stdout, port, cycles, buffer, bufferSize);
    }

}


/**
 * Write a packet to the simulator
 *
 * @param port       Simulator ethernet port to write to
 * @param cycles     Number of cycles to delay in simulator
 * @param buffer     Packet data
 * @param bufferSize Packet size
 * @return Zero on success
 */
static int simulator_write_packet(int port, int cycles, char *buffer, int bufferSize)
{
    char line[256];
    int count = 0;
    int len = snprintf(line, sizeof(line), "%d %d %d\n", port, bufferSize, OCTEON_CYCLES);

    if (simulator_state == STATE_NO_CONNECTION)
        return -1;

    write(simulator_socket, line, len);
    while (count < bufferSize)
    {
        len = snprintf(line, sizeof(line), "%d 0x%02x%02x%02x%02x%02x%02x%02x%02x\n", port,
               (uint32_t)(uint8_t)*(buffer + count + 0),
               (uint32_t)(uint8_t)*(buffer + count + 1),
               (uint32_t)(uint8_t)*(buffer + count + 2),
               (uint32_t)(uint8_t)*(buffer + count + 3),
               (uint32_t)(uint8_t)*(buffer + count + 4),
               (uint32_t)(uint8_t)*(buffer + count + 5),
               (uint32_t)(uint8_t)*(buffer + count + 6),
               (uint32_t)(uint8_t)*(buffer + count + 7));
        count += 8;
        write(simulator_socket, line, len);
    }
    return 0;
}


/**
 * Utility function to read a single line form the simulator.
 *
 * @param sock       Socket to read from
 * @param buffer     Buffer to put the line in
 * @param bufferSize The size of the buffer
 * @return Zero on success, Non zero on failure
 */
static int simulator_readline(int sock, char *buffer, int bufferSize)
{
    int count;
    int bufferLoc = 0;

    do
    {
        if (bufferLoc >= bufferSize)
            return -1;

        count = read(sock, buffer + bufferLoc, 1);
        if (count == 1)
        {
            bufferLoc++;
            if (buffer[bufferLoc-1] == '\n')
                return 0;
        }

    } while (count > 0);

    return -1;
}


/**
 * Thread reading from the simulator and processing its
 * output. It calls log_packet every time a packet is
 * received.
 *
 * @param arg    The port number to connect to the simulator on
 * @return Zero on successful execution
 */
static void *simulator_reader(void *arg)
{
    const unsigned long tcp_port = (unsigned long)arg;
    int                 port;
    int                 wait_loops;
    char                buffer[256];
    struct sockaddr_in  addr;
    static struct
    {
        char buffer[MAX_PACKET_SIZE];
        int size;
        int cycles;
        int index;
        uint64_t packets;
    } packet[MAX_SIMULATOR_PORTS];

    /* Fill in the socket address structure */
    memset(&addr, 0, sizeof(addr));
    addr.sin_addr.s_addr = htonl(0x7f000001);  /* 127.0.0.1 */
    addr.sin_family = AF_INET;
    addr.sin_port = htons(tcp_port);

    /* Zero out the packet state */
    memset(packet, 0, sizeof(packet));

    simulator_state = STATE_NO_CONNECTION;
    fprintf(stderr, "Trying to connect to simulator...\n");
    wait_loops = 0;
    while (simulator_state != STATE_DONE)
    {
        int status = connect(simulator_socket, (struct sockaddr *)&addr, sizeof(addr));

        if (status == 0)
        {
            const char *model = getenv("OCTEON_MODEL");
            fprintf(stderr, "Connected to simulator on port %lu\n", tcp_port);

#if 0
	    if(model == NULL) {
		model = "OCTEON_CN78XX";
		fprintf(stderr,"WARNING: MODEL unknown, using '%s'\n", model);
	    }
#endif

            if (bridge_socket)
            {
		    if (model && (
			(strncasecmp(model, "OCTEON_CN68XX", 13) == 0) ||
		        (strncasecmp(model, "OCTEON_CN78XX", 13) == 0) ||
		        (strncasecmp(model, "OCTEON_CN73XX", 13) == 0) ||
		        (strncasecmp(model, "OCTEON_CN23XX", 13) == 0) ||
		        (strncasecmp(model, "OCTEON_CNF75XX", 13) == 0) ||
		        (strncasecmp(model, "OCTEON_CN76XX", 13) == 0)))
                    snprintf(buffer, sizeof(buffer), "0x%llx\n", (unsigned long long)bridge_port);
                else
                    snprintf(buffer, sizeof(buffer), "0x%llx\n", 1ull << bridge_port);
            }
            else
            {
                const char *interested_ports = "0xfffff";
                if (model && (strncasecmp(model, "OCTEON_CN31XX", 13) == 0))
                    interested_ports = "0x7";
                else if (model && ((strncasecmp(model, "OCTEON_CN30XX", 13) == 0) 
			 || (strncasecmp(model, "OCTEON_CN3010", 13) == 0) 
			 || (strncasecmp(model, "OCTEON_CN50XX", 13) == 0)))
                    interested_ports = "0x7";
                else if (model && (strncasecmp(model, "OCTEON_CN3005", 13) == 0))
                    interested_ports = "0x3";
                else if (model && ((strncasecmp(model, "OCTEON_CN56XX", 13) == 0) 
			 || (strncasecmp(model, "OCTEON_CN57XX", 13) == 0) 
			 || (strncasecmp(model, "OCTEON_CN55XX", 13) == 0)
			 || (strncasecmp(model, "OCTEON_CN54XX", 13) == 0)))
                    interested_ports = "0x1f000f000f";
                else if (model && (strncasecmp(model, "OCTEON_CN52XX", 13) == 0))
                    interested_ports = "0x3f0000000f";
                else if (model && (strncasecmp(model, "OCTEON_CN61XX", 13) == 0))
                    interested_ports = "0xff3f000f000f";
                else if (model && (strncasecmp(model, "OCTEON_CN63XX", 13) == 0))
                    interested_ports = "0xf3f0000000f";
                else if (model && (strncasecmp(model, "OCTEON_CN66XX", 13) == 0))
                    interested_ports = "0xff3f000f000f";
                else if (model && (strncasecmp(model, "OCTEON_CN68XX", 13) == 0))
                    interested_ports = "0x10" /* MGMT */
                                       " 0x100 0x101 0x102 0x103" /* DPI */
                                       " 0x400 0x500" /* ILK */
                                       " 0x800 0x810 0x820 0x830" /* GMX0 - SGMII */
                                       " 0x840" /* GMX0 - XAUI/RXAUI */
                                       " 0x940" /* GMX1 - RXAUI */
                                       " 0xa00 0xa10 0xa20 0xa30" /* GMX2 - SGMII */
                                       " 0xa40" /* GMX2 - XAUI */
                                       " 0xb00 0xb10 0xb20 0xb30" /* GMX3 - SGMII */
                                       " 0xb40" /* GMX3 - XAUI */
                                       " 0xc00 0xc10 0xc20 0xc30" /* GMX4 - SGMII */
                                       " 0xc40" /* GMX4 - XAUI */
                                       ;
                else if (model && (strncasecmp(model, "OCTEON_CN78XX", 13) == 0))
		    //FIXME: PFC channels are not covered, only 4 DPI/ILK chans
                    interested_ports = 
			       " 0x100 0x101 0x102 0x103" /* DPI 0..3 */
			       " 0x400 0x401 0x402 0x403" /* ILK0 CH0..3*/
			       " 0x404 0x405 0x406 0x407" /* ILK0 CH4..7*/
			       " 0x500 0x501 0x502 0x503" /* ILK1 CH0..3*/
//			       " 0x504 0x505 0x506 0x507" /* ILK1 CH4..7*/
			       " 0x800 0x810 0x820 0x830" /* BGX0  */
			       " 0x900 0x910 0x920 0x930" /* BGX1  */
			       " 0xa00 0xa10 0xa20 0xa30" /* BGX2  */
			       " 0xb00 0xb10 0xb20 0xb30" /* BGX3  */
			       " 0xc00 0xc10 0xc20 0xc30" /* BGX4  */
			       " 0xd00 0xd10 0xd20 0xd30" /* BGX5  */
				;
                else if (model && (strncasecmp(model, "OCTEON_CN76XX", 13) == 0))
		    //FIXME: PFC channels are not covered, only 4 DPI/ILK chans
                    interested_ports = 
			       " 0x100 0x101 0x102 0x103" /* DPI 0..3 */
			       " 0x800 0x810 0x820 0x830" /* BGX0  */
			       " 0x900 0x910 0x920 0x930" /* BGX1  */
			       " 0xa00 0xa10 0xa20 0xa30" /* BGX2  */
				;
                else if (model && (strncasecmp(model, "OCTEON_CNF75XX", 14) == 0))
                    interested_ports = 
			       " 0x100 0x101 0x102 0x103" /* DPI 0..3 */
			       " 0x800 0x810 0x820 0x830" /* BGX0  */
			       " 0x240 0x241 0x242 0x243" /* SRIO 0..1  */
				;
                else if (model && (strncasecmp(model, "OCTEON_CN73XX", 13) == 0))
                    interested_ports = 
			       " 0x100 0x101 0x102 0x103" /* DPI 0..3 */
			       " 0x800 0x810 0x820 0x830" /* BGX0  */
			       " 0x900 0x910 0x920 0x930" /* BGX1  */
			       " 0xa00 0xa10 0xa20 0xa30" /* BGX2  */
				;
                else if (model && (strncasecmp(model, "OCTEON_CN23XX", 13) == 0))
                    interested_ports = 
			       " 0x100 0x101 0x102 0x103" /* DPI 0..3 */
			       " 0x800 0x810 0x820 0x830" /* BGX0  */
			       " 0x900 0x910 0x920 0x930" /* BGX1  */
			       " 0xa00 0xa10 0xa20 0xa30" /* BGX2  */
				;
                else if (model && (strncasecmp(model, "OCTEON_CNF71XX", 14) == 0))
                    interested_ports = "0xf00000003";
                else if ((model && (strncasecmp(model, "OCTEON_CN70XX", 13) == 0))
                         || (model && (strncasecmp(model, "OCTEON_CN71XX", 13) == 0)))
                    interested_ports = "0xff010f000f";
                snprintf(buffer, sizeof(buffer), "%s\n", interested_ports);
            }

	    fprintf(stderr,"interest ports '%s'\n", buffer);

            status = write(simulator_socket, buffer, strlen(buffer));
            if (status != (int)strlen(buffer))
                fprintf(stderr, "Failed to register with simulator\n");
            else
            {
                status = 0;
                simulator_state = STATE_RUNNING;
            }
        }
        else
        {
            sleep(1);
            wait_loops++;
            if (wait_timeout && (wait_loops >= wait_timeout))
            {
                simulator_state = STATE_DONE;
                fprintf(stderr, "Timeout waiting for simulator\n");
                return NULL;
            }
            continue;
        }

        while (status == 0)
        {
	    memset(buffer, 0, sizeof(buffer));
            status = simulator_readline(simulator_socket, buffer, sizeof(buffer));
            if (status == 0)
            {
                int port;
                unsigned long long value;
                int cycle;
                int count;

                count = sscanf(buffer, "%d %llu %d", &port, &value, &cycle);
		if (port >= MAX_SIMULATOR_PORTS) {
			fprintf(stderr, "Port %u out of range, aborting.\n",
				port);
			simulator_state = STATE_DONE;
			return NULL;
		}
                switch (count)
                {
                    case 3: /* Begin a new packet */
                        if (packet[port].size != 0)
                            fprintf(stderr, "Starting new packet on port %d without finishing previous", port);
                        packet[port].size = value;
                        packet[port].cycles = cycle;
                        packet[port].index = 0;
                        break;
                    case 2: /* Packet data */
                        /* The above scanf doesn't get the hex data right. Redo it. */
                        sscanf(buffer, "%d %llx", &port, &value);

                        value = ntoh64(value);
                        memcpy(packet[port].buffer + packet[port].index, &value, 8);
                        packet[port].index+=8;
                        if (packet[port].index >= packet[port].size)
                        {
                            packet[port].packets++;
                            log_packet(port, packet[port].cycles, packet[port].buffer, packet[port].size);
                            packet[port].size = 0;
                            packet[port].index = 0;
                        }
                        break;
                    default:
                        fprintf(stderr, "Illegal line from simulator: %s", buffer);
                        break;
                }
            }
        }

        if (simulator_state != STATE_DONE)
            simulator_state = STATE_NO_CONNECTION;
        close(simulator_socket);
        simulator_socket = socket(PF_INET, SOCK_STREAM, 0);
        fprintf(stderr, "Connection closed\n");
        wait_loops = 0;
    }

    fprintf(stderr, "\n");
    for (port=0; port<MAX_SIMULATOR_PORTS; port++)
    {
        if (packet[port].packets)
            fprintf(stderr, "Port %d - Packets received: %llu\n", port, (unsigned long long)packet[port].packets);
    }

    return NULL;
}


/**
 * This function writes all packet to the simulator. First
 * all of the supplied input files are sent. Once these are
 * sent packets are bridged if enable on the command line.
 *
 * @param num_files Number of files to send
 * @param list      List of files to send to the simulator
 */
static void simulator_writer(int num_files, file_info_t list[])
{
    int i;

    while (simulator_state != STATE_RUNNING)
    {
        sleep(1);
        if (simulator_state == STATE_DONE)
            return;
    }

    /* Send all text files to simulator in parallel */
    octeon_send_files(num_files, list);

    for (i=0; i<num_files; i++)
    {
        if (list[i].port != -1)
            tcpdump_send_file(list[i].name, list[i].port);
    }

    if (bridge_socket)
        bridge_packets();

    simulator_state = STATE_DONE;
}

static sigjmp_buf exit_jmp;

static void terminate_handler(int sig)
{
    simulator_state = STATE_DONE;
    siglongjmp(exit_jmp, 1);
}

/**
 * Main entry point
 *
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *const *argv)
{
    const struct option long_options[] =
    {
        {"octeon_log", 1, 0, 'o'},
        {"tcpdump_log", 1, 0, 't'},
        {"input", 1, 0, 'i'},
        {"tcpdump_input", 1, 0, 'I'},
        {"tcp_port", 1, 0, 'p'},
        {"bridge", 1, 0, 'b'},
        {"strip_cycles", 2, 0, 'c'},
        {"tap", 1, 0, 'a'},
        {"wait", 1, 0, 'w'},
	{"promiscuous", 0, &bridge_promisc, 1},
        {NULL, 0, 0, 0}
    };

    int         result          = 0;
    int         option_index    = 0;
    int         show_usage      = (argc < 2);
    int         num_files       = 0;
    unsigned long tcp_port      = 2000;
    pthread_t   thread_id       = 0;
    char *      sep;
    char *devicename = NULL;
    file_info_t list[256];
    struct sigaction sa;

    /* Default the log names to blank. This will cause packet to be sent to
        stdout by default */
    memset(octeon_output_name, 0, sizeof(octeon_output_name));
    memset(tcpdump_output_name, 0, sizeof(tcpdump_output_name));

    while (1)
    {
        char option = getopt_long(argc, argv, "o:t:i:I:p:b:c::a:w:", long_options, &option_index);
        if (option < 0)
            break;

        switch (option)
        {
	    case 0: /* --promisc */
		break;
            case 'o':   /* octeon_log */
                strncpy(octeon_output_name, optarg, sizeof(octeon_output_name));
                break;

            case 't':   /* tcpdump_log */
                strncpy(tcpdump_output_name, optarg, sizeof(tcpdump_output_name));
                break;

            case 'i':   /* input */
                list[num_files].port = -1;
                strncpy(list[num_files].name, optarg, sizeof(list[num_files].name));
                num_files++;
                break;

            case 'I':   /* tcpdump_input */
                sep = strchr(optarg, ':');
                if (sep)
                {
                    list[num_files].port = atoi(optarg);
                    strncpy(list[num_files].name, sep+1, sizeof(list[num_files].name));
                    num_files++;
                }
                else
                {
                    fprintf(stderr, "Tcpdump input missing port number.\n");
                    show_usage = 1;
                }
                break;

            case 'p':   /* tcp_port */
                tcp_port = atoi(optarg);
                break;

            case 'b':   /* bridge */
                sep = strchr(optarg, ':');
                if (sep)
                {
                    bridge_port = atoi(optarg);
		    devicename = sep + 1;
                    bridge_socket = bridge_open_device(devicename);
                    if (bridge_socket == 0)
                    {
                        fprintf(stderr, "Unable to open bridge interface.\n");
                        show_usage = 1;
                    }
                }
                else
                {
                    fprintf(stderr, "Bridge missing port number.\n");
                    show_usage = 1;
                }
                break;

	    case 'a':		/* Use tap. */
		bridge_port = atoi(optarg);
		bridge_socket = tap_open_device();
		if (bridge_socket == 0)
		{
			fprintf(stderr, "Unable to open TUN/TAP device %s.\n",
				TUN_DEVICE);
			show_usage = 1;
		}
		break;

            case 'c':   /* strip_cycles */
                if (optarg)
                    strip_cycles = atoi(optarg);
                else
                    strip_cycles = 1;
                break;

            case 'w':   /* wait */
                wait_timeout = atoi(optarg);
                break;

            default:
                show_usage = 1;
                break;
        }
    }

    if (show_usage)
    {
        fprintf(stderr,
                "\n"
                "Cavium Octeon Packet I/O Utility\n"
                "Version: %s\n"
                "\n"
                "Usage:\n"
                "    %s [options]\n"
                "\n"
                "Supported Options:\n"
                " -o --octeon_log=file          Log all packets out of the simulator in the\n"
                "                               Octeon text format. \"%%d\" will be replaced with\n"
                "                               the output port number\n"
                " -t --tcpdump_log=file         Log all packets out of the simulator in tcpdump\n"
                "                               format. \"%%d\" will be replaced with the output\n"
                "                               port number\n"
                " -i --input=file               Read input from the specified Octeon text file\n"
                "                               and send it to the simulator. This may be specified\n"
                "                               multiple times.\n"
                " -I --tcpdump_input=port:file  Read input from the specified tcpdump file and\n"
                "                               send it to the simulator. This may be specified\n"
                "                               multiple times.\n"
                " -p --tcp_port=port            TCP port to use to connect to the simulator.\n"
                "                               Defaults to port 2000.\n"
                " -b --bridge=port:interface    Bridge traffic received on a Linux interface\n"
                "                               to the supplied simulator port.\n"
                " -a --tap=port                 Bridge traffic received on the Linux TUN/TAP\n"
                "                               device %s to the simulator port.\n"
                " -c --strip_cycles[=cycles]    Strip the cycle count from Octeon logs, forcing it\n"
                "                               to be cycles. This can be useful to allow easy diffs\n"
                "                               with the original input.\n"
                " -w --wait=seconds             Specifiy a max time for waiting for the simulator.\n"
		" --promiscuous                 Put the bridge interface in promiscuous mode.\n",
                OCTEON_SDK_VERSION_STRING, argv[0], TUN_DEVICE);
        return -1;
    }

    if (bridge_promisc && bridge_socket) {
	struct packet_mreq mreq;
	int r;
	memset(&mreq, 0, sizeof(mreq));
	mreq.mr_ifindex = bridge_ifindex;
	mreq.mr_type = PACKET_MR_PROMISC;

	r = setsockopt(bridge_socket, SOL_PACKET, PACKET_ADD_MEMBERSHIP, &mreq, sizeof(mreq));
	if (r) {
	    perror("bridge_open_device setsockopt");
	} else {
	    fprintf(stderr, "set promiscuous mode on %s\n", devicename ? devicename : "*?*");
	}

    }

    if (bridge_socket) {
	int r = 1;
	r = setsockopt(bridge_socket, SOL_PACKET, PACKET_AUXDATA, &r, sizeof(r));
	if(r)
		perror("PACKET_AUX_DATA");
    }

    /* Erase any old Octeon output files. This is necessary since we
        append to files */
    if (octeon_output_name[0])
    {
        int port;
        char name[sizeof(octeon_output_name)];
        for (port=0; port<MAX_SIMULATOR_PORTS; port++)
        {
            sprintf(name, octeon_output_name, port);
            unlink(name);
        }
    }

    /* Erase any old tcpdump output files. This is necessary since we
        append to files */
    if (tcpdump_output_name[0])
    {
        int port;
        char name[sizeof(tcpdump_output_name)];
        for (port=0; port<MAX_SIMULATOR_PORTS; port++)
        {
            sprintf(name, tcpdump_output_name, port);
            unlink(name);
        }
    }

    if (sigsetjmp(exit_jmp, 0))
	goto quit;

    signal(SIGPIPE, SIG_IGN);
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = terminate_handler;
    sigemptyset(&sa.sa_mask);

    sigaction(SIGHUP, &sa, NULL);
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);

    /* Create a socket for the simulator communications */
    simulator_socket = socket(PF_INET, SOCK_STREAM, 0);

    /* Create the reader thread */
    pthread_create(&thread_id, NULL, simulator_reader, (void*)tcp_port);

    /* Do all the needed sending */
    simulator_writer(num_files, list);

    /* Wait for the reader thread to finish */
    pthread_join(thread_id, (void*)&result);


quit:
    /* Don't forget to close down the sockets */
    close(simulator_socket);
    if (bridge_socket) {
	if (bridge_promisc) {
	    struct packet_mreq mreq;

	    memset(&mreq, 0, sizeof(mreq));
	    mreq.mr_ifindex = bridge_ifindex;
	    mreq.mr_type = PACKET_MR_PROMISC;

	    setsockopt(bridge_socket, SOL_PACKET, PACKET_DROP_MEMBERSHIP, &mreq, sizeof(mreq));
	}
        close(bridge_socket);
    }

    exit(result);
}
