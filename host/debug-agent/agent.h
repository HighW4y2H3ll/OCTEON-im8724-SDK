#ifndef __AGENT_H__
#define __AGENT_H__

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

extern int debug;

extern int gdb_fd;

bool create_packet(char *, ssize_t *, char *, va_list);
void remote_open(char *, char *, char *, int, char **);
void remote_handle_packet(char *, size_t, char *, ssize_t *);
void remote_close(void);

int proxy_serial_open (char *);
int proxy_tcp_open (char *);

void perror_and_exit(char *s);
void error_and_exit(char *, ...) __attribute__((noreturn, format(printf, 1, 2)));
void send_verbose_error_response(char *, ...) __attribute__((format(printf, 1, 2)));

static inline  __attribute__ ((format(printf, 1, 2))) void debug_printf(char *format, ...)
{
    va_list ap;

    if (!debug)
	return;

    va_start(ap, format);
    vfprintf(stderr, format, ap);
    va_end(ap);
}

#endif /* __AGENT_H__ */
