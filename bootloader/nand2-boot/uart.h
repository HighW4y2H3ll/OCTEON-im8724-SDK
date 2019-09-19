#ifndef __UART_H__
#define __UART_H__

void uart_write_char(char);
void uart_write_string(const char *);
void uart_write_hex(uint64_t);

#endif /* __UART_H__ */