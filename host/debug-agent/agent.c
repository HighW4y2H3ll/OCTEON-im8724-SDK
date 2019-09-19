#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

#include "agent.h"

#include "cvmx.h"
#include "cvmx-debug.h"

#define VERSION "v2.0"

int debug = 0;
int quiet = 0;
static long gdb_port;
static char *targetstr = NULL;
static char *exec_file;
static char *octeon_pci_bootcmd = NULL;

int gdb_fd = -1;

static int packetchar(int fd, char *c)
{
    ssize_t r = read(fd, c, 1);
    if (r == 1)
        return 1;
    if (r == 0)
    {
        debug_printf("EOF reached\n");
        return 0;
    }
    perror_and_exit("read");
    return 0;
}

static void writechar(int fd, char c)
{
    debug_printf("Character outside a packet: %c (%d)\n", c, c);
    if (fd == -1)
        return;
    if (write(fd, &c, 1) == 1)
        return;
    perror_and_exit("write");
}

/* Receive a packet over STREAM.  STREAM may not be reliable.  Everything that
   is not part of a packet should be sent to OUT_STREAM.  Return 0 if EOF was
   reached, return 1 otherwise.  */

int getpacket(int fd, char *packet, ssize_t *size, int fd_out)
{
    ssize_t i;
    unsigned expected;
    unsigned char csum;

    while (1)
    {
    retry:
        while (1)
        {
            if (!packetchar(fd, &packet[0]))
                return 0;
            if (packet[0] == '$')
                break;
            writechar(fd, packet[0]);
        }
        csum = 0;
        for (i = 1; i < *size; i++)
        {
            if (!packetchar(fd, &packet[i]))
                return 0;
            if (packet[i] == '#')
                break;
            if (packet[i] == '$')
                goto retry;
            csum += packet[i];
        }
        if (packet[i] != '#' || i + 3 > *size)
        {
            debug_printf("Max packet size (%zd) exceeded: %.*s\n", *size, (int) i + 1, packet);
            return 0;
        }
        if (!packetchar(fd, &packet[i + 1]))
            return 0;
        if (!packetchar(fd, &packet[i + 2]))
            return 0;
        sscanf(&packet[i + 1], "%2x", &expected);
        if (csum == expected)
	  {
	    *size = i + 3;
            return 1;
	  }
        debug_printf("Checksum mismatch: \"%.*s\" expected %d received %d\n", (int) i + 2, packet, expected, csum);
    }
}

/* Return 1 if using the remote library.  Otherwise return 0 and set *to.  */

static int target_open(char *targetstr, int *to, int argc, char **argv)
{
    if (targetstr)
    {
        if (strncmp(targetstr, "/dev/", 5) == 0)
        {
            *to = proxy_serial_open(targetstr);
            return 0;
        }
        else if (strncmp(targetstr, "tcp:", 4) == 0)
        {
            *to = proxy_tcp_open(targetstr);
            return 0;
        }
        else if (strcmp(targetstr, "LINUX") == 0 ||
                 strncmp(targetstr, "PCI", 3) == 0 ||
                 strncmp(targetstr, "MACRAIGOR:", 10) == 0)
        {
            remote_open(targetstr, octeon_pci_bootcmd, exec_file, argc, argv);
            return 1;
        }
    }
    error_and_exit("Incorrect targetstr line \"%s\"\n", targetstr);
}

static int gdb_open(int port)
{
    int sfd, cfd;
    struct sockaddr_in sockaddr, peer_addr;
    socklen_t peer_addr_size = sizeof(struct sockaddr_in);

    sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sfd == -1)
        perror_and_exit("socket");
    if (port != 0)
    {
        int on = 1;
        if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(on)) == -1)
            perror_and_exit("setsockopt");

        sockaddr.sin_family = AF_INET;
        sockaddr.sin_port = htons(port);
        sockaddr.sin_addr.s_addr = INADDR_ANY;
        if (bind(sfd, (struct sockaddr *) &sockaddr, sizeof(struct sockaddr_in)) == -1)
            perror_and_exit("bind");
    }

    if (listen(sfd, 50) == -1)
        perror_and_exit("listen");

    if (port == 0)
    {
        socklen_t len = sizeof(struct sockaddr_in);
        if (getsockname(sfd, (struct sockaddr *) &sockaddr, &len) == -1)
            perror_and_exit("getsockname");
    }
    if (!quiet)
        fprintf(stderr, "Listening on port: %d\n", ntohs(sockaddr.sin_port));

    cfd = accept(sfd, (struct sockaddr *) &peer_addr, &peer_addr_size);
    if (cfd == -1)
        perror_and_exit("accept");
    if (!quiet)
        fprintf(stderr, "Connected to peer: %s\n", inet_ntoa(peer_addr.sin_addr));

    return cfd;
}

static void print_usage(char *prog)
{
    fprintf (stderr,
             "Usage: %s <exec_file> <option(s)> [<boot commands>]\n\n"
             " Listen on <port> for incoming connection fro GDB and perform the\n"
             " corresponding operation over the <comm> debug interface.\n\n"
             " Possible values for <option(s)> are:\n"
             "  -l, --listen <num>  Port number to listen for GDB connection\n"
             "  -t, --target <targetstr> (Required) \n"
             "  -d, --debug         Print debug messages during execution\n"
             "  -b, --pci-bootcmd <exec>  Set boot command needed for PCI booting"
             "  -q, --quiet         Quiet during execution\n"
             "  -h, --help          Display help message\n", prog);
    exit (1);
}


static void parse_options(int argc, char **argv)
{
    int opt;
    char *end;

    const struct option long_options[] = 
        {
            { "help", no_argument, NULL, 'h' },
            { "quiet", no_argument, NULL, 'q' },
            { "debug", optional_argument, NULL, 'd' },
            { "listen", required_argument, NULL, 'l' },
            { "target", required_argument, NULL, 't' },
            { "pci-bootcmd", required_argument, NULL, 'b' },
            { 0, 0, 0, 0 }
        };

    while ((opt = getopt_long (argc, argv, "hd::l:t:qb:", long_options, NULL)) != -1)
    {
        switch (opt)
        {
            case 'h':
                print_usage(argv[0]);
                break;
            case 'd':
                if (optarg)
                {
                    long d = strtol(optarg, &end, 10);
                    if (d == LONG_MIN || d == LONG_MAX || *optarg == 0 || *end != 0)
                        print_usage(argv[0]);
                    debug = d;
                }
                else
                    debug = 1;
                break;
            case 'l':
                gdb_port = strtol(optarg, &end, 10);
                if (gdb_port == LONG_MIN || gdb_port == LONG_MAX || *optarg == 0 || *end != 0)
                    print_usage(argv[0]);
                break;
	    case 'q':
		quiet = 0;
		break;
            case 't':
                targetstr = optarg;
                break;
            case 'b':
                octeon_pci_bootcmd = optarg;
                break;
            default:
                abort();
        }
    }
    if (optind >= argc)
        print_usage (argv[0]);

    exec_file = argv[optind++];
}

void perror_and_exit(char *s)
{
    perror(s);
    exit(1);
}

void error_and_exit(char *format, ...)
{
    va_list ap;
    va_start(ap, format);
    vfprintf(stderr, format, ap);
    va_end(ap);
    exit(1);
}

static unsigned char compute_csum(char *packet, size_t packetlen)
{
    unsigned char csum = 0;
    while (packetlen--)
        csum += (unsigned char) packet[packetlen];
    return csum;
}

static size_t assemble_packet(char *packet, size_t packetlen)
{
    unsigned char csum = compute_csum(&packet[1], packetlen);
    packet[0] = '$';
    packet[1 + packetlen] = '#';
    sprintf(&packet[1 + packetlen + 1], "%02x", csum);
    return packetlen + 4;
}

bool create_packet(char *packet, ssize_t *packetlen, char *format, va_list ap)
{
    ssize_t n;

    n = vsnprintf(&packet[1], *packetlen - 4, format, ap);
    if (n >= *packetlen - 4)
        return false;
    *packetlen = assemble_packet(packet, n);
    return true;
}

void send_verbose_error_response(char *format, ...)
{
    char response[CVMX_DEBUG_MAX_RESPONSE_SIZE];
    ssize_t n;
    va_list ap;
    va_start(ap, format);

    if (gdb_fd == -1)
    {
        vprintf(format, ap);
        va_end(ap);
        return;
    }

    response[0] = 'E';
    response[1] = '.';
    n = vsnprintf(&response[2], sizeof(response) - 2, format, ap);
    va_end(ap);
    if (n > (ssize_t)(sizeof(response) - 2))
        debug_printf("Verbose error response truncated: \"%s\"\n", &response[2]);
    n = assemble_packet(response, n + 2);
    if (write(gdb_fd, response, n) < n)
        perror_and_exit("send_verbose_error_response");
}

int main(int argc, char **argv)
{
    int target_fd = -1;
    int use_octeon_remote = 0;
    char request[CVMX_DEBUG_MAX_REQUEST_SIZE];
    char response[CVMX_DEBUG_MAX_RESPONSE_SIZE];

    parse_options(argc, argv);
    debug_printf("Debug is on at level %d\n", debug);

    if (!targetstr)
        print_usage(argv[0]);

    gdb_fd = gdb_open(gdb_port);

    use_octeon_remote = target_open(targetstr, &target_fd, argc - optind, argv + optind);

    while (1)
    {
        ssize_t requestlen, responselen;

        requestlen = sizeof(request);
        if (!getpacket(gdb_fd, request, &requestlen, -1))
            break;
        debug_printf("Request: %.*s\n", (int) requestlen, request);

        responselen = sizeof(response);
        if (use_octeon_remote)
            remote_handle_packet(request, requestlen, response, &responselen);
        else
        {
            if (write(target_fd, request, requestlen) != requestlen)
                perror_and_exit("writing request to target");
            if (!getpacket(target_fd, response, &responselen, gdb_fd))
                break;
        }

        if (write(gdb_fd, response, responselen) != responselen)
            perror_and_exit("writing response to gdb");
        debug_printf("Response: %.*s\n", (int) responselen, response);
    }

    if (use_octeon_remote)
        remote_close();
    else
        close(target_fd);
            
    close(gdb_fd);

    return 0;
}
