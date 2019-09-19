#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <errno.h>

#include "agent.h"

static speed_t speed_to_code(int speed)
{
    switch(speed)
    {
	default:return B0;
	case 50:return B50;
	case 75:return B75;
	case 110:return B110;
	case 134:return B134;
	case 150:return B150;
	case 200:return B200;
	case 300:return B300;
	case 600:return B600;
	case 1200:return B1200;
	case 1800:return B1800;
	case 2400:return B2400;
	case 4800:return B4800;
	case 9600:return B9600;
	case 19200:return B19200;
	case 38400:return B38400;
	case 57600:return B57600;
	case 115200:return B115200;
	case 230400:return B230400;    
    }
}

int proxy_serial_open(char *comm)
{
    int baudrate = 115200;
    char *baudstr;
    struct termios options;
    speed_t speed;

    baudstr = strchr(comm, ' ');
    if (baudstr)
    {
	long n = strtol(baudstr + 1, NULL, 10);
	if (n != LONG_MIN && n != LONG_MAX)
	    baudrate = n;

	baudstr[0] = '\0';
    }

    int fd = open(comm, O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd == -1)
	error_and_exit("Failed to open serial port %s\n", comm);

    tcgetattr(fd, &options);

    speed = speed_to_code(baudrate);
    if (speed == B0)
    {
	fprintf(stderr, "Incorrect baud rate in %s\n", comm);
	return -1;
    }
    cfsetispeed(&options, speed);
    cfsetospeed(&options, speed);

    options.c_cflag |= (CLOCAL | CREAD);
    /* Set 8N1.  */
    options.c_cflag &= ~(PARENB | CSTOPB | CSIZE);
    options.c_cflag |=  CS8;
    /* No HW flow control.  */
    options.c_cflag &= ~CRTSCTS;
 
    /* Enable data to be processed as raw input */
    options.c_lflag = 0;

    tcsetattr(fd, TCSANOW, &options);

    return fd;
}

int proxy_tcp_open(char *targetstr)
{
    int fd;
    struct sockaddr_in sockaddr;
    char *hostname, *portstr;
    long port;
    struct hostent *hostent;

    hostname = &targetstr[4];
    portstr = strchr(hostname, ':');
    if (!portstr)
	error_and_exit("Incorrect tcp connection string: \"%s\"\n", targetstr);
    *portstr = '\0';
    if (hostname[0] == '\0')
	hostname = "127.0.0.1";
    port = strtol(portstr + 1, NULL, 10);
    if (port == LONG_MIN || port == LONG_MAX)
	error_and_exit("Incorrect tcp connection string: \"%s\"\n", targetstr);

    hostent = gethostbyname(hostname);
    if (hostent == NULL)
	error_and_exit("Failed to lookup host %s\n", hostname);

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1)
	perror_and_exit("socket");

    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons(port);
    sockaddr.sin_addr = *( struct in_addr *) hostent->h_addr_list[0];

    if (connect(fd, (struct sockaddr *) &sockaddr, sizeof(struct sockaddr_in)) == -1)
	perror_and_exit("connect");

    return fd;
}

