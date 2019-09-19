/* Copyright (c) 2004, 2005, 2006 Cavium Networks.
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

#ifndef	_ENDIAN_H
#define	_ENDIAN_H   1

/* Definitions for byte order, according to significance of bytes,
   from low addresses to high addresses.  The value is what you get by
   putting '4' in the most significant byte, '3' in the second most
   significant byte, '2' in the second least significant byte, and '1'
   in the least significant byte, and then writing down one digit for
   each byte, starting with the byte at the lowest address at the left,
   and proceeding to the byte with the highest address at the right.  */

#define __LITTLE_ENDIAN 1234
#define __BIG_ENDIAN    4321

/* This file defines `__BYTE_ORDER' for the particular machine.  */
#include <bits/endian.h>

/* Provide standard hton* macros */
#if __BYTE_ORDER == __LITTLE_ENDIAN
#error Little endian mode currently not supported.
#elif __BYTE_ORDER == __BIG_ENDIAN
#define htons(x)    (x)
#define htonl(x)    (x)
#define htonll(x)    (x)
#define ntohs(x)    (x)
#define ntohl(x)    (x)
#define ntohll(x)    (x)
#endif

#endif	/* endian.h */
