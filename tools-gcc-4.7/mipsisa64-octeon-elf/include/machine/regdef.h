/* regdef.h: Conventional names of registers with usage mnemonics. 
 *
 * Copyright (c) 2004, 2005, 2006 Cavium Networks.
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

#define zero    $0	/* always returns 0 */

#define AT      $1	/* reserved for use by assembler */

#define v0      $2	/* value returned by subroutine */
#define v1      $3	

#define a0      $4	/* parameters for a subroutine */
#define a1      $5	
#define a2      $6
#define a3      $7

#define t0      $8	/* subroutines can use without saving */
#define t1      $9
#define t2      $10
#define t3      $11
#define t4      $12
#define t5      $13
#define t6      $14
#define t7      $15

#define s0      $16	/* subroutine register variables */
#define s1      $17
#define s2      $18
#define s3      $19
#define s4      $20
#define s5      $21
#define s6      $22
#define s7      $23

#define s8      $30	/* frame pointer */

#define t8      $24
#define t9      $25

#define k0      $26	/* reserved for use by interrupt/trap handler */
#define k1      $27

#define gp      $28	/* global pointer */

#define sp      $29	/* stack pointer */
#define fp      $30	/* frame pointer */
#define ra      $31	/* return address for subroutine */
