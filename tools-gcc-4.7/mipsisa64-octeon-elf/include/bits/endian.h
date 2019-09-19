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

/* File version info: $Id: endian.h 19371 2006-09-19 21:40:59Z cchavva $  */

/* The MIPS architecture has selectable endianness.
   Linux/MIPS exists in two both little and big endian flavours and we
   want to be able to share the installed headerfiles between both,
   so we define __BYTE_ORDER based on GCC's predefines.  */

#ifndef _ENDIAN_H
# error "Never use <bits/endian.h> directly; include <endian.h> instead."
#endif

#ifdef __MIPSEB__
# define __BYTE_ORDER __BIG_ENDIAN
#else
# ifdef __MIPSEL__
#  define __BYTE_ORDER __LITTLE_ENDIAN
# endif
#endif

/* Needed internal by hash.h in newlib.  */
#define LITTLE_ENDIAN  __LITTLE_ENDIAN
#define BIG_ENDIAN     __BIG_ENDIAN
#define BYTE_ORDER     __BYTE_ORDER
