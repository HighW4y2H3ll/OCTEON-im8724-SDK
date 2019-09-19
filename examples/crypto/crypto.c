/***********************license start***************
 * Copyright (c) 2003-2010  Cavium Inc. (support@cavium.com). All rights 
 * reserved.
 *
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.

 *   * Neither the name of Cavium Inc. nor the names of
 *     its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written
 *     permission.  

 * This Software, including technical data, may be subject to U.S. export  control
 * laws, including the U.S. Export Administration Act and its  associated
 * regulations, and may be subject to export or import  regulations in other
 * countries. 

 * TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS" 
 * AND WITH ALL FAULTS AND CAVIUM INC. MAKES NO PROMISES, REPRESENTATIONS OR
 * WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH RESPECT TO
 * THE SOFTWARE, INCLUDING ITS CONDITION, ITS CONFORMITY TO ANY REPRESENTATION OR
 * DESCRIPTION, OR THE EXISTENCE OF ANY LATENT OR PATENT DEFECTS, AND CAVIUM
 * SPECIFICALLY DISCLAIMS ALL IMPLIED (IF ANY) WARRANTIES OF TITLE,
 * MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, LACK OF
 * VIRUSES, ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR
 * CORRESPONDENCE TO DESCRIPTION. THE ENTIRE  RISK ARISING OUT OF USE OR
 * PERFORMANCE OF THE SOFTWARE LIES WITH YOU.
 ***********************license end**************************************/


/**
 *
 *
 * @file crypto.c
 *
 * Crypto example performing:
 *
 *     - MD5
 *     - SHA1
 *     - 3DES
 *     - 3DES CBC
 *     - AES
 *     - AES CBC
 *     - KASUMI
 *     - KASUMI CBC
 *     - SNOW 3G
 *     - SMS4
 *     - SMS4 CBC
 *
 * File version info: $Id: crypto.c 82375 2013-03-28 02:12:56Z cchavva $
 *
 */

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "cvmx.h"
#include "cvmx-key.h"
#include "cvmx-swap.h"


/**
 * Calculate the MD5 hash of a block of data
 *
 * @param md5        Filled with the 16 byte MD5 hash
 * @param buffer     Input data
 * @param buffer_len Inout data length
 */
static void hash_md5(uint8_t *md5, const uint8_t *buffer, int buffer_len)
{
    const uint64_t bits = cvmx_cpu_to_le64(buffer_len * 8); /* MD5 expects little endian */
    const uint64_t *ptr = (const uint64_t *)buffer;
    uint8_t chunk[64];

    /* Set the IV to the MD5 magic start value */
    CVMX_MT_HSH_IV(0x0123456789abcdefull, 0);
    CVMX_MT_HSH_IV(0xfedcba9876543210ull, 1);

    /* MD5 input is in the following form:
        1) User data
        2) Byte 0x80
        3) Optional zero padding
        4) Original Data length in bits as an 8 byte unsigned integer
        Zero padding is added to make the 1-4 an even multiple of 64 bytes */

    /* Iterate through 64 bytes at a time */
    while (buffer_len >= 64)
    {
        CVMX_MT_HSH_DAT(*ptr++, 0);
        CVMX_MT_HSH_DAT(*ptr++, 1);
        CVMX_MT_HSH_DAT(*ptr++, 2);
        CVMX_MT_HSH_DAT(*ptr++, 3);
        CVMX_MT_HSH_DAT(*ptr++, 4);
        CVMX_MT_HSH_DAT(*ptr++, 5);
        CVMX_MT_HSH_DAT(*ptr++, 6);
        CVMX_MT_HSH_STARTMD5(*ptr++);
        buffer_len-=64;
    }

    /* The rest of the data will need to be copied into a chunk */
    if (buffer_len > 0)
        memcpy(chunk, ptr, buffer_len);
    chunk[buffer_len] = 0x80;
    memset(chunk + buffer_len + 1, 0, 64 - buffer_len - 1);

    ptr = (const uint64_t *)chunk;
    CVMX_MT_HSH_DAT(*ptr++, 0);
    CVMX_MT_HSH_DAT(*ptr++, 1);
    CVMX_MT_HSH_DAT(*ptr++, 2);
    CVMX_MT_HSH_DAT(*ptr++, 3);
    CVMX_MT_HSH_DAT(*ptr++, 4);
    CVMX_MT_HSH_DAT(*ptr++, 5);
    CVMX_MT_HSH_DAT(*ptr++, 6);

    /* Check to see if there is room for the bit count */
    if (buffer_len < 56)
        CVMX_MT_HSH_STARTMD5(bits);
    else
    {
        CVMX_MT_HSH_STARTMD5(*ptr);
        /* Another block was needed */
        CVMX_MT_HSH_DATZ(0);
        CVMX_MT_HSH_DATZ(1);
        CVMX_MT_HSH_DATZ(2);
        CVMX_MT_HSH_DATZ(3);
        CVMX_MT_HSH_DATZ(4);
        CVMX_MT_HSH_DATZ(5);
        CVMX_MT_HSH_DATZ(6);
        CVMX_MT_HSH_STARTMD5(bits);
    }

    /* Get the final MD5 */
    CVMX_MF_HSH_IV(((uint64_t*)md5)[0], 0);
    CVMX_MF_HSH_IV(((uint64_t*)md5)[1], 1);
}


/**
 * Calculate the SHA1 hash of a block of data
 *
 * @param sha1       Filled with the 20 byte SHA1 hash
 * @param buffer     Input data
 * @param buffer_len Inout data length
 */
static void hash_sha1(uint8_t *sha1, const uint8_t *buffer, int buffer_len)
{
    const uint64_t bits = (buffer_len * 8); /* SHA1 expects big endian */
    const uint64_t *ptr = (const uint64_t *)buffer;
    uint8_t chunk[64];
    uint64_t tmp;

    /* Set the IV to the MD5 magic start value */
    CVMX_MT_HSH_IV(0x67452301EFCDAB89ull, 0);
    CVMX_MT_HSH_IV(0x98BADCFE10325476ull, 1);
    CVMX_MT_HSH_IV(0xC3D2E1F000000000ull, 2);

    /* SHA1 input is in the following form:
        1) User data
        2) Byte 0x80
        3) Optional zero padding
        4) Original Data length in bits as an 8 byte unsigned integer
        Zero padding is added to make the 1-4 an even multiple of 64 bytes */

    /* Iterate through 64 bytes at a time */
    while (buffer_len >= 64)
    {
        CVMX_MT_HSH_DAT(*ptr++, 0);
        CVMX_MT_HSH_DAT(*ptr++, 1);
        CVMX_MT_HSH_DAT(*ptr++, 2);
        CVMX_MT_HSH_DAT(*ptr++, 3);
        CVMX_MT_HSH_DAT(*ptr++, 4);
        CVMX_MT_HSH_DAT(*ptr++, 5);
        CVMX_MT_HSH_DAT(*ptr++, 6);
        CVMX_MT_HSH_STARTSHA(*ptr++);
        buffer_len-=64;
    }

    /* The rest of the data will need to be copied into a chunk */
    if (buffer_len > 0)
        memcpy(chunk, ptr, buffer_len);
    chunk[buffer_len] = 0x80;
    memset(chunk + buffer_len + 1, 0, 64 - buffer_len - 1);

    ptr = (const uint64_t *)chunk;
    CVMX_MT_HSH_DAT(*ptr++, 0);
    CVMX_MT_HSH_DAT(*ptr++, 1);
    CVMX_MT_HSH_DAT(*ptr++, 2);
    CVMX_MT_HSH_DAT(*ptr++, 3);
    CVMX_MT_HSH_DAT(*ptr++, 4);
    CVMX_MT_HSH_DAT(*ptr++, 5);
    CVMX_MT_HSH_DAT(*ptr++, 6);

    /* Check to see if there is room for the bit count */
    if (buffer_len < 56)
        CVMX_MT_HSH_STARTSHA(bits);
    else
    {
        CVMX_MT_HSH_STARTSHA(*ptr);
        /* Another block was needed */
        CVMX_MT_HSH_DATZ(0);
        CVMX_MT_HSH_DATZ(1);
        CVMX_MT_HSH_DATZ(2);
        CVMX_MT_HSH_DATZ(3);
        CVMX_MT_HSH_DATZ(4);
        CVMX_MT_HSH_DATZ(5);
        CVMX_MT_HSH_DATZ(6);
        CVMX_MT_HSH_STARTSHA(bits);
    }

    /* Get the final SHA1 */
    CVMX_MF_HSH_IV(((uint64_t*)sha1)[0], 0);
    CVMX_MF_HSH_IV(((uint64_t*)sha1)[1], 1);
    CVMX_MF_HSH_IV(tmp, 2);
    ((uint32_t*)sha1)[4] = tmp >> 32;
}


/**
 * Initialize 3des for use
 *
 * @param key    3des keys
 */
static void crypto_3des_initialize(const uint64_t *key)
{
    CVMX_MT_3DES_KEY(key[0],0);
    CVMX_MT_3DES_KEY(key[1],1);
    CVMX_MT_3DES_KEY(key[2],2);
}


/**
 * 3des encrypt without any block chaining
 *
 * @param data     Data to encrypt
 * @param data_len Length of the data. Must be a multiple of 8
 */
static void crypto_3des_encrypt(uint64_t *data, int data_len)
{
    assert((data_len & 0x7) == 0);

    while (data_len)
    {
        CVMX_MT_3DES_ENC(*data);
        CVMX_MF_3DES_RESULT(*data);
        data++;
        data_len-=8;
    }
}


/**
 * 3des decrypt without any block chaining
 *
 * @param data     Data to decrypt
 * @param data_len Length of the data. Must be a multiple of 8
 */
static void crypto_3des_decrypt(uint64_t *data, int data_len)
{
    assert((data_len & 0x7) == 0);

    while (data_len)
    {
        CVMX_MT_3DES_DEC(*data);
        CVMX_MF_3DES_RESULT(*data);
        data++;
        data_len-=8;
    }
}


/**
 * 3des encrypt with CBC block chaining
 *
 * @param iv     Initialization Vector
 * @param data     Data to encrypt
 * @param data_len Length of the data. Must be a multiple of 8
 */
static void crypto_3des_encrypt_cbc(uint64_t iv, uint64_t *data, int data_len)
{
    assert((data_len & 0x7) == 0);

    CVMX_MT_3DES_IV(iv);

    while (data_len)
    {
        CVMX_MT_3DES_ENC_CBC(*data);
        CVMX_MF_3DES_RESULT(*data);
        data++;
        data_len-=8;
    }
}


/**
 * 3des decrypt with CBC block chaining
 *
 * @param iv     Initialization Vector
 * @param data     Data to decrypt
 * @param data_len Length of the data. Must be a multiple of 8
 */
static void crypto_3des_decrypt_cbc(uint64_t iv, uint64_t *data, int data_len)
{
    assert((data_len & 0x7) == 0);

    CVMX_MT_3DES_IV(iv);

    while (data_len)
    {
        CVMX_MT_3DES_DEC_CBC(*data);
        CVMX_MF_3DES_RESULT(*data);
        data++;
        data_len-=8;
    }
}


/**
 * Initialize AES for use
 *
 * @param key     AES keys
 * @param key_len Length of key in bits
 */
static void crypto_aes_initialize(const uint64_t *key, int key_len)
{
    CVMX_MT_AES_KEY(key[0],0);
    CVMX_MT_AES_KEY(key[1],1);
    CVMX_MT_AES_KEY(key[2],2);
    CVMX_MT_AES_KEY(key[3],3);
    CVMX_MT_AES_KEYLENGTH(key_len/64 - 1);
}


/**
 * AES encrypt without any block chaining
 *
 * @param data     Data to encrypt
 * @param data_len Length of the data. Must be a multiple of 16
 */
static void crypto_aes_encrypt(uint64_t *data, int data_len)
{
    assert((data_len & 0xf) == 0);

    while (data_len)
    {
        CVMX_MT_AES_ENC0(*data);
        CVMX_MT_AES_ENC1(*(data+1));
        CVMX_MF_AES_RESULT(*data++, 0);
        CVMX_MF_AES_RESULT(*data++, 1);
        data_len-=16;
    }
}


/**
 * AES decrypt without any block chaining
 *
 * @param data     Data to decrypt
 * @param data_len Length of the data. Must be a multiple of 16
 */
static void crypto_aes_decrypt(uint64_t *data, int data_len)
{
    assert((data_len & 0xf) == 0);

    while (data_len)
    {
        CVMX_MT_AES_DEC0(*data);
        CVMX_MT_AES_DEC1(*(data+1));
        CVMX_MF_AES_RESULT(*data++, 0);
        CVMX_MF_AES_RESULT(*data++, 1);
        data_len-=16;
    }
}


/**
 * AES encrypt with CBC block chaining
 *
 * @param iv       Initialization Vector
 * @param data     Data to encrypt
 * @param data_len Length of the data. Must be a multiple of 16
 */
static void crypto_aes_encrypt_cbc(const uint64_t *iv, uint64_t *data, int data_len)
{
    assert((data_len & 0xf) == 0);

    CVMX_MT_AES_IV(iv[0],0);
    CVMX_MT_AES_IV(iv[1],1);

    while (data_len)
    {
        CVMX_MT_AES_ENC_CBC0(*data);
        CVMX_MT_AES_ENC_CBC1(*(data+1));
        CVMX_MF_AES_RESULT(*data++, 0);
        CVMX_MF_AES_RESULT(*data++, 1);
        data_len-=16;
    }
}


/**
 * AES decrypt with CBC block chaining
 *
 * @param iv       Initialization Vector
 * @param data     Data to decrypt
 * @param data_len Length of the data. Must be a multiple of 16
 */
static void crypto_aes_decrypt_cbc(const uint64_t *iv, uint64_t *data, int data_len)
{
    assert((data_len & 0xf) == 0);

    CVMX_MT_AES_IV(iv[0],0);
    CVMX_MT_AES_IV(iv[1],1);

    while (data_len)
    {
        CVMX_MT_AES_DEC_CBC0(*data);
        CVMX_MT_AES_DEC_CBC1(*(data+1));
        CVMX_MF_AES_RESULT(*data++, 0);
        CVMX_MF_AES_RESULT(*data++, 1);
        data_len-=16;
    }
}


/**
 * Initialize KASUMI for use
 *
 * @param key    KASUMI keys
 */
static void crypto_kasumi_initialize(const uint64_t *key)
{
    CVMX_MT_KAS_KEY(key[0],0);
    CVMX_MT_KAS_KEY(key[1],1);
}

/**
 * KASUMI encrypt without any block chaining
 *
 * @param data     Data to encrypt
 * @param data_len Length of the data. Must be a multiple of 8
 */
static void crypto_kasumi_encrypt(uint64_t *data, int data_len)
{
    assert((data_len & 0x7) == 0);

    while (data_len)
    {
        CVMX_MT_KAS_ENC(*data);
        CVMX_MF_KAS_RESULT(*data);
        data++;
        data_len-=8;
    }
}


/**
 * KASUMI encrypt with CBC block chaining
 *
 * @param iv       Initialization Vector
 * @param data     Data to encrypt
 * @param data_len Length of the data. Must be a multiple of 8
 */
static void crypto_kasumi_encrypt_cbc(uint64_t iv, uint64_t *data, int data_len)
{
    assert((data_len & 0x7) == 0);

    CVMX_MT_KAS_RESULT(iv);

    while (data_len)
    {
        CVMX_MT_KAS_ENC_CBC(*data);
        CVMX_MF_KAS_RESULT(*data);
        data++;
        data_len-=8;
    }
}

/**
 * Initialize SMS4 for use
 *
 * @param key     SMS4 keys
 */
static void crypto_sms4_initialize(const uint64_t *key)
{
    CVMX_MT_SMS4_KEY(key[0],0);
    CVMX_MT_SMS4_KEY(key[1],1);
}


/**
 * SMS4 encrypt without any block chaining
 *
 * @param data     Data to encrypt
 * @param data_len Length of the data. Must be a multiple of 16
 */
static void crypto_sms4_encrypt(uint64_t *data, int data_len)
{
    assert((data_len & 0xf) == 0);

    while (data_len)
    {
        CVMX_MT_SMS4_ENC0(*data);
        CVMX_MT_SMS4_ENC1(*(data+1));
        CVMX_MF_SMS4_RESINP(*data++, 0);
        CVMX_MF_SMS4_RESINP(*data++, 1);
        data_len-=16;
    }
}


/**
 * SMS4 decrypt without any block chaining
 *
 * @param data     Data to decrypt
 * @param data_len Length of the data. Must be a multiple of 16
 */
static void crypto_sms4_decrypt(uint64_t *data, int data_len)
{
    assert((data_len & 0xf) == 0);

    while (data_len)
    {
        CVMX_MT_SMS4_DEC0(*data);
        CVMX_MT_SMS4_DEC1(*(data+1));
        CVMX_MF_SMS4_RESINP(*data++, 0);
        CVMX_MF_SMS4_RESINP(*data++, 1);
        data_len-=16;
    }
}


/**
 * SMS4 encrypt with CBC block chaining
 *
 * @param iv       Initialization Vector
 * @param data     Data to encrypt
 * @param data_len Length of the data. Must be a multiple of 16
 */
static void crypto_sms4_encrypt_cbc(const uint64_t *iv, uint64_t *data, int data_len)
{
    assert((data_len & 0xf) == 0);

    CVMX_MT_SMS4_IV(iv[0],0);
    CVMX_MT_SMS4_IV(iv[1],1);

    while (data_len)
    {
        CVMX_MT_SMS4_ENC_CBC0(*data);
        CVMX_MT_SMS4_ENC_CBC1(*(data+1));
        CVMX_MF_SMS4_RESINP(*data++, 0);
        CVMX_MF_SMS4_RESINP(*data++, 1);
        data_len-=16;
    }
}


/**
 * SMS4 decrypt with CBC block chaining
 *
 * @param iv       Initialization Vector
 * @param data     Data to decrypt
 * @param data_len Length of the data. Must be a multiple of 16
 */
static void crypto_sms4_decrypt_cbc(const uint64_t *iv, uint64_t *data, int data_len)
{
    assert((data_len & 0xf) == 0);

    CVMX_MT_SMS4_IV(iv[0],0);
    CVMX_MT_SMS4_IV(iv[1],1);

    while (data_len)
    {
        CVMX_MT_SMS4_DEC_CBC0(*data);
        CVMX_MT_SMS4_DEC_CBC1(*(data+1));
        CVMX_MF_SMS4_RESINP(*data++, 0);
        CVMX_MF_SMS4_RESINP(*data++, 1);
        data_len-=16;
    }
}

/**
 * Encrypts/decrypts blocks of data between 1 and 20000 bits in length.
 *
 * @param key        128 bit Confidentiality Key.
 * @param count      32-bit Count, Frame dependent input.
 * @param bearer     5-bit Bearer identity (in the LSB side).
 * @param dir        1 bit, direction of transmission.
 * @param data       length number of bits, input bit stream.
 * @param length     16 bit Length, i.e., the number of bits to be encrypted or
 *                   decrypted.
 */
static void hash_snow3g(const uint8_t *key, int count, int bearer, int dir, uint8_t *data, int length)
{
    uint32_t K[4], IV[4];
    int n = (length + 31) / 32;
    uint32_t *KS;
    int i;

    /* Load the confidentiality key for SNOW 3G initialization as in section 3.4. */
    memcpy(K+3,key+0,4);  /* K[3] = key[0]; we assume K[3]=key[0]||key[1]||...||key[31],
		             with key[0] the most important bit of key */
    memcpy(K+2,key+4,4);  /* K[2] = key[1]; */
    memcpy(K+1,key+8,4);  /* K[1] = key[2]; */
    memcpy(K+0,key+12,4); /* K[0] = key[3]; we assume
                             K[0]=key[96]||key[97]||...||key[127] , with key[127] the
                             least important bit of key */
                                                                               
  /* Prepare the initialization vector (IV) for SNOW 3G initialization as in 
     section 3.4. */
    IV[3] = count;
    IV[2] = (bearer << 27) | ((dir & 0x1) << 26);
    IV[1] = IV[3];
    IV[0] = IV[2];
    
    KS = (uint32_t *) malloc (4*n);

    /* Run SNOW 3G algorithm to generate sequence of key stream bits KS */
    CVMX_MT_SNOW3G_RESULT(IV[0]);
    CVMX_MT_SNOW3G_FSM(IV[1], 0);
    CVMX_MT_SNOW3G_FSM(IV[2], 1);
    CVMX_MT_SNOW3G_FSM(IV[3], 2);


    CVMX_MT_SNOW3G_LFSR((((uint64_t)(K[0] ^ 0xffffffff) << 32) | (K[1] ^ 0xffffffff)), 0);
    CVMX_MT_SNOW3G_LFSR((((uint64_t)(K[2] ^ 0xffffffff) << 32) | (K[3] ^ 0xffffffff)), 1);
    CVMX_MT_SNOW3G_LFSR((((uint64_t)K[0] << 32) | K[1]), 2);
    CVMX_MT_SNOW3G_LFSR((((uint64_t)K[2] << 32) | K[3]), 3);
    CVMX_MT_SNOW3G_LFSR((((uint64_t)(K[0] ^ 0xffffffff) << 32) | (K[1] ^ 0xffffffff ^ IV[3])), 4);
    CVMX_MT_SNOW3G_LFSR((((uint64_t)(K[2] ^ 0xffffffff ^ IV[2]) << 32) | (K[3] ^ 0xffffffff)), 5);
    CVMX_MT_SNOW3G_LFSR((((uint64_t)(K[0] ^ IV[1]) << 32) | (K[1])), 6);
    CVMX_MT_SNOW3G_START(((uint64_t)K[2] << 32) | (K[3] ^ IV[0]));

    CVMX_MF_SNOW3G_RESULT(((uint64_t*)KS)[0]);

    for (i=1;i<n/2;i++)
    {
        CVMX_MT_SNOW3G_MORE(0);
        CVMX_MF_SNOW3G_RESULT(((uint64_t*)KS)[i]);
    }

    /* Exclusive-OR the input data with keystream to generate the output bit 
       stream */
    for (i=0;i<n*4;i++)
        data[i] ^= *(((uint8_t*)KS)+i);

    free(KS);
}


/**
 * Display a buffer in hex
 *
 * @param buffer     Data buffer to display
 * @param buffer_len Length of the buffer
 */
static void display_hex(const void *buffer, int buffer_len)
{
    const uint8_t *ptr = (const uint8_t*)buffer;
    while (buffer_len--)
    {
        printf("%02x", (unsigned int)*ptr);
        ptr++;
    }
}


/**
 * Main entry point
 *
 * @return
 */
int main()
{
    const char    *plaintext            = "Original plaintext message";
    const uint64_t plaintext_md5[]      = {0x1517fa5c261777f9ull, 0x67f2c7c034a13fd4ull};
    const uint64_t plaintext_sha1[]     = {0x4964868fb28c4fdbull, 0x08afa18c462e4c2cull, 0x53b9c0ca00000000ull};
    const uint64_t key_3des[]           = {0x0123456789abcdefull,
                                           0x0123456789abcdefull,
                                           0x0123456789abcdefull};
    const uint64_t iv_3des[]            = {0x1234567890abcdefull};
    const uint64_t plaintext_3des[]     = {0x4e6f772069732074ull, 0x4e6f772069732074ull};
    const uint64_t key_aes[]           	= {0x0123456789abcdefull,
                                           0x0123456789abcdefull,
                                           0x0123456789abcdefull,
                                           0x0123456789abcdefull};
    const uint64_t iv_aes[]             = {0x1234567890abcdefull,
                                           0x1234567890abcdefull};
    const uint64_t plaintext_aes[]      = {0x4e6f772069732074ull, 0x4e6f772069732074ull,
                                           0x4e6f772069732074ull, 0x4e6f772069732074ull};
    const uint64_t key_kasumi[]         = {0x2bd6459f82c5b300ull, 0x952c49104881ff48ull}; /* 3GPP Test set 1 */
    const uint64_t input_kasumi[]       = {0xea024714ad5c4d84ull};
    const uint64_t output_kasumi[]      = {0xdf1f9b251c0bf45full};
    const uint64_t plaintext_kasumi[]   = {0x4e6f772069732074ull, 0x4e6f772069732074ull,
                                           0x4e6f772069732074ull, 0x4e6f772069732074ull};
    const uint64_t output_kasumi_cbc[]  = {0x86e5030dee2524f9ull, 0x6d88a257350ad649ull,
                                           0x3fb6337a9d7c329bull, 0x5b1bb9785cd1808full};
    const uint64_t key_sms4[]           = {0x0123456789abcdefull, 0xfedcba9876543210ull};
    const uint64_t iv_sms4[]            = {0x1234567890abcdefull, 0xfedcba9876543210ull};
    const uint64_t plaintext_sms4[]     = {0x0123456789abcdefull, 
				           0xfedcba9876543210ull,
					   0x0123456789abcdefull,
				           0xfedcba9876543210ull};
    const uint8_t key_snow3g[]          = {0xD3, 0xC5, 0xD5, 0x92, 0x32, 0x7F, 0xB1,
					   0x1C, 0x40, 0x35, 0xC6, 0x68, 0x0A, 0xF8,
					   0xC6, 0xD1};
    uint8_t plaintext_snow3g[]          = {0x98, 0x1B, 0xA6, 0x82, 0x4C, 0x1B, 0xFB,
					   0x1A, 0xB4, 0x85, 0x47, 0x20, 0x29, 0xB7,
					   0x1D, 0x80, 0x8C, 0xE3, 0x3E, 0x2C, 0xC3,
					   0xC0, 0xB5, 0xFC, 0x1F, 0x3D, 0xE8, 0xA6,
					   0xDC, 0x66, 0xB1, 0xF0};
    const uint8_t cipher_snow3g[]       = {0x5D, 0x5B, 0xFE, 0x75, 0xEB, 0x04, 0xF6,
					   0x8C, 0xE0, 0xA1, 0x23, 0x77, 0xEA, 0x00,
					   0xB3, 0x7D, 0x47, 0xC6, 0xA0, 0xBA, 0x06,
					   0x30, 0x91, 0x55, 0x08, 0x6A, 0x85, 0x9C,
					   0x43, 0x41, 0xB3, 0x7C};

    /* Additional parameters used by SNOW3g. */
    int count_snow3g = 0x398A59B4; // frame dependent input.
    int bearer_snow3g=0x15;        // Bearer identity (in the LSB side).
    int dir_snow3g=1;              // Direction of transmission.
    int length_snow3g=253;         // number of bits to be encrpted/decrypted.

    uint8_t  result_md5[16];
    uint8_t  result_sha1[20];
    uint64_t result_3des[(sizeof(plaintext_3des) + 7) / 8];
    uint64_t result_3des_cbc[(sizeof(plaintext_3des) + 7) / 8];
    uint64_t result_aes[(sizeof(plaintext_aes) + 15) / 16 * 2];
    uint64_t result_aes_cbc[(sizeof(plaintext_aes) + 15) / 16 * 2];
    uint64_t result_kasumi[(sizeof(input_kasumi) + 7) / 8];
    uint64_t result_kasumi_cbc[(sizeof(plaintext_kasumi) + 7) / 8];
    uint64_t result_sms4[(sizeof(plaintext_sms4) + 7) / 8];
    uint64_t result_sms4_cbc[(sizeof(plaintext_sms4) + 7) / 8];

    if (octeon_has_feature(OCTEON_FEATURE_KEY_MEMORY))
    {
        uint64_t counter;
        /* Fill the on chip key memory with incrementing numbers */
        for (counter=0; counter<CVMX_KEY_MEM_SIZE; counter+=8)
            cvmx_key_write(counter, counter);

        /* Read back the on chip key memory making sure it contains the correct stuff */
        for (counter=0; counter<CVMX_KEY_MEM_SIZE; counter+=8)
            if (counter != cvmx_key_read(counter))
                printf("Key memory at address 0x%llx - ***** TEST FAILED *****\n", (unsigned long long)counter);

        /* In this example we only check if key memory is there. We don't
            actually store the keys in it. */
    }

    printf("Plaintext: \"%s\"\n", plaintext);

    /* Test MD5 */

    hash_md5(result_md5, (uint8_t*)plaintext, strlen(plaintext));
    printf("MD5:      ");
    display_hex(result_md5, sizeof(result_md5));
    if (memcmp(result_md5, plaintext_md5, sizeof(result_md5)) == 0)
        printf("\n");
    else
        printf(" ***** TEST FAILED *****\n");

    /* Test SHA1 */

    hash_sha1(result_sha1, (uint8_t*)plaintext, strlen(plaintext));
    printf("SHA1:     ");
    display_hex(result_sha1, sizeof(result_sha1));
    if (memcmp(result_sha1, plaintext_sha1, sizeof(result_sha1)) == 0)
        printf("\n");
    else
        printf(" ***** TEST FAILED *****\n");

    /* Test 3des */

    crypto_3des_initialize(key_3des);
    memset(result_3des, 0, sizeof(result_3des));
    memcpy(result_3des, plaintext_3des, sizeof(plaintext_3des));
    crypto_3des_encrypt(result_3des, sizeof(result_3des));
    printf("3des:     ");
    display_hex(result_3des, sizeof(result_3des));
    crypto_3des_initialize(key_3des);
    crypto_3des_decrypt(result_3des, sizeof(result_3des));
    if (memcmp((char*)result_3des, plaintext_3des, sizeof(result_3des_cbc)) == 0)
        printf("\n");
    else
        printf(" ***** TEST FAILED *****\n");

    memset(result_3des_cbc, 0, sizeof(result_3des_cbc));
    memcpy(result_3des_cbc, plaintext_3des, sizeof(plaintext_3des));
    crypto_3des_initialize(key_3des);
    crypto_3des_encrypt_cbc(iv_3des[0], result_3des_cbc, sizeof(result_3des_cbc));
    printf("3des cbc: ");
    display_hex(result_3des_cbc, sizeof(result_3des_cbc));
    crypto_3des_initialize(key_3des);
    crypto_3des_decrypt_cbc(iv_3des[0], result_3des_cbc, sizeof(result_3des_cbc));
    if (memcmp((char*)result_3des_cbc, plaintext_3des, sizeof(result_3des_cbc)) == 0)
        printf("\n");
    else
        printf(" ***** TEST FAILED *****\n");

    /* Test AES */

    crypto_aes_initialize(key_aes, 256);
    memset(result_aes, 0, sizeof(result_aes));
    memcpy(result_aes, plaintext_aes, sizeof(plaintext_aes));
    crypto_aes_encrypt(result_aes, sizeof(result_aes));
    printf("aes:      ");
    display_hex(result_aes, sizeof(result_aes));
    crypto_aes_initialize(key_aes, 256);
    crypto_aes_decrypt(result_aes, sizeof(result_aes));
    if (memcmp(result_aes, plaintext_aes, sizeof(plaintext_aes)) == 0)
        printf("\n");
    else
        printf(" ***** TEST FAILED *****\n");

    memset(result_aes_cbc, 0, sizeof(result_aes_cbc));
    memcpy(result_aes_cbc, plaintext_aes, sizeof(plaintext_aes));
    crypto_aes_initialize(key_aes, 256);
    crypto_aes_encrypt_cbc(iv_aes, result_aes_cbc, sizeof(result_aes_cbc));
    printf("aes cbc:  ");
    display_hex(result_aes_cbc, sizeof(result_aes_cbc));
    crypto_aes_initialize(key_aes, 256);
    crypto_aes_decrypt_cbc(iv_aes, result_aes_cbc, sizeof(result_aes_cbc));
    if (memcmp(result_aes_cbc, plaintext_aes, sizeof(plaintext_aes)) == 0)
        printf("\n");
    else
        printf(" ***** TEST FAILED *****\n");

    /* Test KASUMI */

    if (!OCTEON_IS_MODEL(OCTEON_CN3XXX))
    {
        crypto_kasumi_initialize(key_kasumi);
        memset(result_kasumi, 0, sizeof(result_kasumi));
        memcpy(result_kasumi, input_kasumi, sizeof(input_kasumi));
        crypto_kasumi_encrypt(result_kasumi, sizeof(result_kasumi));
        printf("kasumi:   ");
        display_hex(result_kasumi, sizeof(result_kasumi));

        if (memcmp((char*)result_kasumi, output_kasumi, sizeof(result_kasumi)) == 0)
            printf("\n");
        else
            printf(" ***** TEST FAILED *****\n");

        memset(result_kasumi_cbc, 0, sizeof(result_kasumi_cbc));
        memcpy(result_kasumi_cbc, plaintext_kasumi, sizeof(plaintext_kasumi));
        crypto_kasumi_initialize(key_kasumi);
        crypto_kasumi_encrypt_cbc(0ull, result_kasumi_cbc, sizeof(result_kasumi_cbc));
        printf("kasumi cbc: ");
        display_hex(result_kasumi_cbc, sizeof(result_kasumi_cbc));
        if (memcmp((char*)result_kasumi_cbc, output_kasumi_cbc, sizeof(result_kasumi_cbc)) == 0)
            printf("\n");
        else
            printf(" ***** TEST FAILED *****\n");
    }

    if (!OCTEON_IS_MODEL(OCTEON_CN3XXX) && !OCTEON_IS_MODEL(OCTEON_CN5XXX))
    {
        /* Test SMS4 */

        crypto_sms4_initialize(key_sms4);
        memset(result_sms4, 0, sizeof(result_sms4));
        memcpy(result_sms4, plaintext_sms4, sizeof(plaintext_sms4));
        crypto_sms4_encrypt(result_sms4, sizeof(result_sms4));
        printf("sms4:     ");
        display_hex(result_sms4, sizeof(result_sms4));
        crypto_sms4_initialize(key_sms4);
        crypto_sms4_decrypt(result_sms4, sizeof(result_sms4));
        if (memcmp(result_sms4, plaintext_sms4, sizeof(plaintext_sms4)) == 0)
            printf("\n");
        else
            printf(" ***** TEST FAILED *****\n");

        memset(result_sms4_cbc, 0, sizeof(result_sms4_cbc));
        memcpy(result_sms4_cbc, plaintext_sms4, sizeof(plaintext_sms4));
        crypto_sms4_initialize(key_sms4);
        crypto_sms4_encrypt_cbc(iv_sms4, result_sms4_cbc, sizeof(result_sms4_cbc));
        printf("sms4 cbc: ");
        display_hex(result_sms4_cbc, sizeof(result_sms4_cbc));
        crypto_sms4_initialize(key_sms4);
        crypto_sms4_decrypt_cbc(iv_sms4, result_sms4_cbc, sizeof(result_sms4_cbc));
        if (memcmp(result_sms4_cbc, plaintext_sms4, sizeof(plaintext_sms4)) == 0)
            printf("\n");
        else
            printf(" ***** TEST FAILED *****\n");

        /* Test SNOW3G */
        hash_snow3g(key_snow3g, count_snow3g, bearer_snow3g, dir_snow3g, plaintext_snow3g, length_snow3g);
        printf("snow3g:   ");
        display_hex(plaintext_snow3g, sizeof(plaintext_snow3g));
        if (memcmp(plaintext_snow3g, cipher_snow3g, sizeof(plaintext_snow3g)) == 0)
            printf("\n");
        else
            printf(" ***** TEST FAILED *****\n");
    }

    return 0;
}

