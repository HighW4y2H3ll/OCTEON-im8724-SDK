/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License, GPLv2 or later.  See the file "COPYING" in the main directory of
 * this archive for more details.
 *
 * Copyright (C) 2010 Cavium Inc. 
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdarg.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>

#define INIT_BUFFS_SIZE (512)
#define CONSOLE_CMD "console="

#ifndef DEBUG_ENABLE
#define messageD(fmt, ...)
#else   /* #ifndef DEBUG_ENABLE */
#define messageD message

static ssize_t safe_write(int fd, const void *buf, size_t count)
{
	ssize_t n;

	do {
		n = write(fd, buf, count);
	} while (n < 0 && errno == EINTR);

	return n;
}


/*
 * Write all of the supplied buffer out to a file.
 * This does multiple writes as necessary.
 * Returns the amount written, or -1 on an error.
 */
static ssize_t bb_full_write(int fd, const void *buf, size_t len)
{
	ssize_t cc;
	ssize_t total;

	total = 0;

	while (len > 0) {
		cc = safe_write(fd, buf, len);

		if (cc < 0)
			return cc;		/* write() returns -1 on failure. */

		total += cc;
		buf = ((const char *)buf) + cc;
		len -= cc;
	}

	return total;
}

static void message(const char *fmt, ...)
	__attribute__ ((format(__printf__, 1, 2)));
static void message(const char *fmt, ...)
{
	va_list arguments;
	int l;
	char msg[1024];
	int fd;

	msg[0] = '\r';
	va_start(arguments, fmt);
	l = vsnprintf(msg + 1, 1024 - 2, fmt, arguments) + 1;
	va_end(arguments);

	msg[l++] = '\n';
	msg[l] = 0;

	fd = open("/dev/console",
			     O_WRONLY | O_NOCTTY | O_NONBLOCK);
	/* Always send console messages to /dev/console so people will see them. */
	if (fd >= 0) {
		bb_full_write(fd, msg, l);
		close(fd);
	}
}
#endif   /* #ifndef DEBUG_ENABLE */

/* Set terminal settings to reasonable defaults */
static void set_term(void)
{
	struct termios tty;

	tcgetattr(STDIN_FILENO, &tty);

	/* set control chars */
	tty.c_cc[VINTR] = 3;	/* C-c */
	tty.c_cc[VQUIT] = 28;	/* C-\ */
	tty.c_cc[VERASE] = 127;	/* C-? */
	tty.c_cc[VKILL] = 21;	/* C-u */
	tty.c_cc[VEOF] = 4;	/* C-d */
	tty.c_cc[VSTART] = 17;	/* C-q */
	tty.c_cc[VSTOP] = 19;	/* C-s */
	tty.c_cc[VSUSP] = 26;	/* C-z */

	/* use line dicipline 0 */
	tty.c_line = 0;

	/* Make it be sane */
	tty.c_cflag &= CBAUD | CBAUDEX | CSIZE | CSTOPB | PARENB | PARODD;
	tty.c_cflag |= CREAD | HUPCL | CLOCAL;


	/* input modes */
	tty.c_iflag = ICRNL | IXON | IXOFF;

	/* output modes */
	tty.c_oflag = OPOST | ONLCR;

	/* local modes */
	tty.c_lflag =
		ISIG | ICANON | ECHO | ECHOE | ECHOK | ECHOCTL | ECHOKE | IEXTEN;

	tcsetattr(STDIN_FILENO, TCSANOW, &tty);
}

struct tty_device_type {
	const char *lookup_str;
	const char *format_str;
};

static struct tty_device_type device_table[] =
{
/*	{"ttyS", "ttyS"}, this is already done by busybox init.c */
/*	{"tty", "tty"},   this is already done by busybox init.c */
	{"hvc", "hvc"},
	{"pci", "ttyPCI"}
};

static int device_table_size = sizeof(device_table) / sizeof(struct tty_device_type);

int main(int argc, char **argv)
{
	FILE *file;
	int found_cons_type = 0;
	char buf[INIT_BUFFS_SIZE];
	char console[32];
	char *str_ptr;
	char *num_str;
	int fd;
	int i;
	int cons_i;

	/* open and read /proc/cmdline, if console is there use that device */
	file = fopen("/proc/cmdline", "r");

	if (file != NULL) {
		if ((fgets(buf, INIT_BUFFS_SIZE, file) != NULL) &&
		    (str_ptr = strstr(buf, CONSOLE_CMD))) {
			messageD("cmdline: %s", buf);

			str_ptr += strlen(CONSOLE_CMD);

			messageD("console string: %s", str_ptr);

			for (i = 0; i < device_table_size; i++) {
				if (strstr(str_ptr,
					   device_table[i].lookup_str) == str_ptr) {
					cons_i = i;
					found_cons_type = 1;
					break;
				}
			}

			if (found_cons_type) {
				str_ptr += strlen(device_table[cons_i].lookup_str);
				messageD("after device: XX%sXX", str_ptr);

				num_str = strtok(str_ptr, ", \t\r\n");

				messageD("number: XX%sXX", num_str);

				snprintf(console, sizeof(console) - 1,"/dev/%s%s",
					 device_table[cons_i].format_str, num_str);

				messageD("console: %s (%s:%s)", console,
					 device_table[cons_i].format_str, num_str);

				if ((fd = open(console, O_RDWR)) >= 0) {
					close(0);
					close(1);
					close(2);

					dup2(fd, 0);
					dup2(fd, 1);
					dup2(fd, 2);

					/* Make the terminal act normally */
					set_term();
					close(fd);
				}
			}
		}

		fclose(file);
	}

	/* Now the "real" program - probably /bin/sh. */
	messageD("real program: %s", argv[1]);
	execv(argv[1], (argv + 1));

	return 0;
}
