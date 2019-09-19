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

/* asm.h: Define some inline assembly macros.  */

#ifndef _TEXT_SECTION
#define _TEXT_SECTION .text
#endif

#define LEAF(name) \
        _TEXT_SECTION; \
        .globl  name; \
        .ent    name; \
name:

#define END(name) \
        .size name,.-name; \
        .end    name

