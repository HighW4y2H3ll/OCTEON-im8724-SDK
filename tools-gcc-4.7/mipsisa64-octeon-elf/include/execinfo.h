/* Copyright (c) 2007 Cavium Networks.
 *
 * The authors hereby grant permission to use, copy, modify, distribute,
 * and license this software and its documentation for any purpose, provided
 * that existing copyright notices are retained in all copies and that this
 * notice is included verbatim in any distributions. No written agreement,
 * license, or royalty fee is required for any of the authorized uses.
 * Modifications to this software may be copyrighted by their authors
 * and need not follow the licensing terms described here, provided that
 * the new terms are clearly indicated on the first page of each file where
 * they apply.
 */

#ifndef _EXECINFO_H
#define _EXECINFO_H 1

typedef int (*__octeon_backtrace_printf_t) (const char *, ...);

/* Similar to glibc's backtrace.  Need to compile code with
   -fexceptions.  */
int backtrace (void **, int);

/* Octeon extensions.  Prints backtrace to standard output or printing
   through a custom printf function.  Need to compile code with
   -fexceptions.  */
void __octeon_print_backtrace (void);
void __octeon_print_backtrace_func (__octeon_backtrace_printf_t);

#endif  /* _EXECINFO_H */
