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


/*
 * Example zip (deflate) block usage.  This file shows a simple example of using
 * the zip block to compress/decompress the gzip format.
 *
 * Input is read from the files 'input' and 'input.gz', and output files
 * 'output' and 'output.gz' are written
 *
 *
 * File version info: $Id: zip.c 104198 2014-09-10 22:23:01Z ajasty $
 */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#include <stdio.h>
#include <string.h>
#include <unistd.h>
#ifdef __linux__
#include <malloc.h>
#endif
#include "cvmx-config.h"
#include "cvmx.h"
#include "cvmx-fpa.h"
#include "cvmx-zip.h"
#include "cvmx-bootmem.h"
#include "cvmx-wqe.h"
#include "cvmx-helper.h"
#include "cvmx-sysinfo.h"


#ifdef COMPILED_INPUT
extern char input_array[], input_array_end[];
extern char input_gz_array[], input_gz_array_end[];
#endif

/* Structure of what the fixed portion of a gzip file header looks like.  See
** RFC 1952 for further information
*/
typedef struct
{
    uint8_t id1;
    uint8_t id2;
    uint8_t cm;
#if __BYTE_ORDER == __BIG_ENDIAN
    uint8_t reserved    :3;    /* Flags field */
    uint8_t fcomment    :1;
    uint8_t fname       :1;
    uint8_t fextra      :1;
    uint8_t fhcrc       :1;
    uint8_t ftext       :1;
#else
    uint8_t ftext       :1;
    uint8_t fhcrc       :1;
    uint8_t fextra      :1;
    uint8_t fname       :1;
    uint8_t fcomment    :1;
    uint8_t reserved    :3;
#endif
    uint32_t mtime;  /* little endian */
    uint8_t xfl;
    uint8_t os;
} __attribute__ ((packed)) gzip_header_t;

#ifndef MAX
#define MAX(a,b)    ((a)>(b) ? (a):(b))
#define MIN(a,b)    ((a)<(b) ? (a):(b))
#endif


// NOTE: must use pool 5
#define SG_FPA_POOL         (5)
#define SG_FPA_POOL_SIZE    (2048)

#define SG_NUM_PTRS         (MIN(SG_FPA_POOL_SIZE/8, 16*1024 -1))
#define SG_PTRS_SIZE        (SG_NUM_PTRS * 8)

#define MAX_INPUT_SIZE      (SG_NUM_PTRS * CVMX_ZIP_PTR_MAX_LEN)
#define MAX_OUTPUT_SIZE     (SG_NUM_PTRS * CVMX_ZIP_PTR_MAX_LEN)

#define CPU_FREQ_MHZ        (600)

/**
 * Creates list of (up to number that fit in configured free pool) scatter/gather pointers
 * returns number of sg pointers created
 * Memory for the sg lists are allocated from the SG_FPA_POOL FPA pool
 *
 * @param buf_ptr   pointer to buffer to create scatter/gather list for
 * @param data_size size of buffer to create sg list for
 * @param sg_list_addr
 *                  pointer to address to return address of sg list in
 *
 * @return number of pointer in the sg list that is returned in sg_list_addr
 */
int create_sg_list(uint8_t *buf_ptr, uint64_t data_size, uint64_t *sg_list_addr)
{
    cvmx_zip_ptr_t *zptr = cvmx_fpa_alloc(SG_FPA_POOL);
    cvmx_zip_ptr_t *zptr_base = zptr;

    if ((data_size > SG_NUM_PTRS * CVMX_ZIP_PTR_MAX_LEN) || data_size < 2)
        return(0);
    if (!zptr)
        return(0);

    /* Set sg list address for caller */
    *sg_list_addr = cvmx_ptr_to_phys(zptr);

    /* We must to have at least 2 pointers, so a small amount of data is a special case.
    ** Split up the data so that we have 2 sg pointers */
    if (data_size <= CVMX_ZIP_PTR_MAX_LEN)
    {
        zptr->u64 = 0;
        cvmx_zptr_set_ptr(zptr, cvmx_ptr_to_phys(buf_ptr));
        cvmx_zptr_set_len(zptr, 1);
        data_size -= cvmx_zptr_get_len(zptr);
        buf_ptr += cvmx_zptr_get_len(zptr);
        zptr++;
    }

    /* We know we won't run out of pointers because we checked the size */
    while (data_size > 0)
    {
        zptr->u64 = 0;
        cvmx_zptr_set_ptr(zptr, cvmx_ptr_to_phys(buf_ptr));
	cvmx_zptr_set_len(zptr, data_size > CVMX_ZIP_PTR_MAX_LEN ? 
			  CVMX_ZIP_PTR_MAX_LEN : data_size);
        data_size -= cvmx_zptr_get_len(zptr);
        buf_ptr += cvmx_zptr_get_len(zptr);
        zptr++;
    }


    return(zptr - zptr_base);
}


/**
 * This function setups up a simple compress or decompress operation.
 *
 * (It does not handle zip instruction queue management.)
 *
 * @param compress_flag
 *                   1: compress
 *                   0: decompress
 * @param input_ptr  pointer to input data
 * @param input_size size of input data
 * @param output_ptr pointer to output buffer
 * @param output_size
 *                   size of output buffer
 * @param queue      ZIP queue
 *
 * @return pointer to cvmx_zip_result_t, the result structure filed in by the hardware
 */
cvmx_zip_result_t *process_deflate_file(int compress_flag, void *input_ptr, uint64_t input_size, void *output_ptr, uint64_t output_size, int queue)
{
    cvmx_zip_command_t zip_cmd;
    uint64_t input_sg_addr, output_sg_addr;
    unsigned num_output_sg_pointers, num_input_sg_pointers;
    cvmx_zip_result_t *result_ptr;

    memset(&zip_cmd, 0, sizeof(zip_cmd));

    zip_cmd.s.compress = compress_flag; /* decompress */

    /* Create gather list for input data since it may be larger than a single pointer can
    ** store */
    num_input_sg_pointers = create_sg_list(input_ptr, input_size, &input_sg_addr);
    zip_cmd.s.dgather = 1;  /* input is scatter list */
    cvmx_zptr_set_ptr(&zip_cmd.s.in_ptr, input_sg_addr);
    cvmx_zptr_set_len(&zip_cmd.s.in_ptr, num_input_sg_pointers);

    zip_cmd.s.adler32 = 0x0;  /* CRC is initilized to zero for gzip */

    /* We are doing entire 'file', this chunk includes beginning and end */
    zip_cmd.s.eof = 1;
    zip_cmd.s.bof = 1;

    /* Setup output buffer with scatter list */
    zip_cmd.s.totaloutputlength = output_size;
    num_output_sg_pointers = create_sg_list(output_ptr, output_size, &output_sg_addr);
    zip_cmd.s.dscatter = 1;  /* output is scatter list */
    cvmx_zptr_set_fbw(&zip_cmd.s.out_ptr, 0);  /* must be 0 when scatter used */
    cvmx_zptr_set_len(&zip_cmd.s.out_ptr, num_output_sg_pointers);
    cvmx_zptr_set_ptr(&zip_cmd.s.out_ptr, output_sg_addr);

printf("totaloutputlength = %llu, input_sg_ptrs = %d, output_sg_ptrs = %d\n", (unsigned long long)output_size, num_input_sg_pointers, num_output_sg_pointers);
printf("input_sg_addr = 0x%llx, output_sg_addr = 0x%llx\n", CAST_ULL(input_sg_addr), CAST_ULL(output_sg_addr));

    /* Setup result pointer */
    result_ptr = cvmx_bootmem_alloc(sizeof(cvmx_zip_result_t), 128);
    if (!result_ptr)
    {
        printf("Error: bootmem allocation failed.\n");
        return(NULL);
    }
    memset((void *)result_ptr, 0, sizeof(*result_ptr));
    cvmx_zptr_set_ptr(&zip_cmd.s.result_ptr, 
		      cvmx_ptr_to_phys((void*)result_ptr));
    /* Size not set for result ptr - size is fixed */

    if (OCTEON_IS_MODEL(OCTEON_CN68XX) || octeon_has_feature(OCTEON_FEATURE_ZIP3))
        cvmx_zip_queue_submit(&zip_cmd, queue);
    else
        cvmx_zip_submit(&zip_cmd);

    /* poll results for completion of operation */
    while (!(((result_ptr->u64[2]) & 0xffull)))
	CVMX_SYNC;

    /* Free scatter/gather list structures */
    cvmx_fpa_free(cvmx_phys_to_ptr(output_sg_addr) , SG_FPA_POOL, 0);
    cvmx_fpa_free(cvmx_phys_to_ptr(input_sg_addr), SG_FPA_POOL, 0);

    return((cvmx_zip_result_t *)result_ptr);
}

int main()
{
    unsigned align = CVMX_CACHE_LINE_SIZE;
    gzip_header_t *ghdr_ptr;
    cvmx_zip_result_t *zip_result_ptr;
    uint64_t start_cycles;
    uint64_t total_cycles;
    int queue = 0;
    int bytes_read;
    uint64_t output_size = MAX_OUTPUT_SIZE;
    unsigned long long deflate_bytes;
    uint8_t *output_data_ptr, *input_data_ptr;
    uint8_t *cptr;
    uint32_t uncomp_crc, uncomp_size;
#ifndef COMPILED_INPUT
    static const uint8_t generic_gzip_header[] =
	{0x1f, 0x8b, 0x8, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xff};
    int bytes_written;
    uint32_t tmp;
    int fd;
#endif

    if (!octeon_has_feature(OCTEON_FEATURE_ZIP))
    {
        printf("ZIP is not present, skipping.\n");
        return 0;
    }

    cvmx_user_app_init();

    /* Make sure we are only run on one core. */
    if (!cvmx_is_init_core())
        return 0;

    /* Assumes pool 5, buffers must be 2k! */
    cvmx_fpa_enable();

    while (align < CVMX_FPA_ZIP_POOL_SIZE)
        align = align << 1;
    
    /* Allocate pools for ZIP command queues */
    if( __cvmx_helper_initialize_fpa_pool(CVMX_FPA_ZIP_POOL,
				CVMX_FPA_ZIP_POOL_SIZE,
				100, "ZIP Cmd-bufs") < 0)
    {
	cvmx_dprintf("%s: ERROR: in ZIP buffer pool\n", __func__);
	return -1;
    }

#ifdef __linux__

    /* Need to use valid user memory for read, then copy to phys mapped */
    void *linux_buf = malloc(MAX_INPUT_SIZE + 1);
    if (!linux_buf)
    {
        printf("Error allocating tmp linux buffer\n");
        return(-1);
    }
#endif

    printf("Max input size: %d, max output size: %d\n", MAX_INPUT_SIZE, MAX_OUTPUT_SIZE);

    /* initialize the ZIP block */
    if (octeon_has_feature(OCTEON_FEATURE_ZIP3))
        cvmx_zip3_init(queue, 0x1);
    else if (OCTEON_IS_MODEL(OCTEON_CN68XX))
        cvmx_zip_queue_initialize(queue, 0x1); // Use ZIP core0
    else
        cvmx_zip_initialize();

    /* Read input compressed file*/
    input_data_ptr = cvmx_bootmem_alloc(MAX_INPUT_SIZE + 1, 0);
    if (!input_data_ptr)
    {
        printf("Error: bootmem allocation failed.\n");
        return(-1);
    }

#ifdef COMPILED_INPUT
    bytes_read = input_gz_array_end - input_gz_array;
    memcpy(input_data_ptr, input_gz_array, bytes_read);
#else

    fd = open("input.gz", O_RDONLY);
    if (fd < 0)
    {
        perror("Error opening compressed input file\n");
        return(-1);
    }


#ifdef __linux__
    bytes_read = read(fd, linux_buf, MAX_INPUT_SIZE + 1);
    if (bytes_read > 0)
        memcpy(input_data_ptr, linux_buf, bytes_read);
#else
    bytes_read = read(fd, input_data_ptr, MAX_INPUT_SIZE + 1);
#endif

    if (bytes_read < 0 || bytes_read > MAX_INPUT_SIZE)
    {
        perror("Error reading compressed input file\n");
        return(-1);
    }
    close(fd);

#endif

    /* Parse the gzip header, find start of compressed data */
    ghdr_ptr = (gzip_header_t *)input_data_ptr;
    cptr = input_data_ptr + sizeof(gzip_header_t);
    if (ghdr_ptr->id1 != 0x1f || ghdr_ptr->id2 != 0x8b)
    {
        printf("ERROR, the input compressed file does not appear to be in gzip format\n");
        return(-1);
    }
    if (ghdr_ptr->fextra)
    {
        uint16_t xlen;
        xlen = *cptr | (*(cptr + 1) << 8);
        cptr += xlen + 2;
    }
    if (ghdr_ptr->fname)
    {
        printf("Original filename of compressed source: %s\n", cptr);
        while (*cptr++)
            ;
    }
    if (ghdr_ptr->fcomment)
        while (*cptr++)
            ;

    if (ghdr_ptr->fhcrc)
        cptr += 2;

    /* cptr now points to the start of the deflate compressed data */

    /* Read CRC and uncompressed size from end of file */
    memcpy(&uncomp_crc, input_data_ptr + bytes_read - 8, 4);
    memcpy(&uncomp_size, input_data_ptr + bytes_read - 4, 4);


#if __BYTE_ORDER == __BIG_ENDIAN
    CVMX_ES32(uncomp_crc, uncomp_crc);
    CVMX_ES32(uncomp_size, uncomp_size);
#endif

    /* Compute number of compressed bytes */
    deflate_bytes = bytes_read - (cptr - input_data_ptr) - 8;
    printf("CRC of original file: 0x%lu, uncompressed size: %lu, bytes of deflate data: %llu\n", (unsigned long)uncomp_crc, (unsigned long)uncomp_size, deflate_bytes);

    /* We have now parsed the gzip header, and identfied the the start of the 'deflate' bytes, and how many there are.
    ** Now we feed those to the deflate block, and verify the uncompressed CRC and length of the output.
    */

    output_data_ptr = cvmx_bootmem_alloc(output_size, 128);
    if (!output_data_ptr)
    {
        printf("Error: bootmem allocation failed.\n");
        return(-1);
    }

    start_cycles = cvmx_get_cycle();
    zip_result_ptr = process_deflate_file(0, cptr, deflate_bytes, output_data_ptr, output_size, queue);
    total_cycles = cvmx_get_cycle() - start_cycles;

    if (zip_result_ptr->s.completioncode == 1)
    {
        if (uncomp_crc != zip_result_ptr->s.crc32)
        {
            printf("ERROR, CRC compare FAILED!\n");
        }
        if (uncomp_size != zip_result_ptr->s.totalbyteswritten)
        {
            printf("ERROR, uncompressed length compare FAILED!\n");
        }
        printf("Decompressed %llu bytes to %d bytes in %llu cycles, \n%d compressed Mbits/sec, %d uncompressed Mbits/sec (at %d Mhz)\n",
               (unsigned long long)deflate_bytes, zip_result_ptr->s.totalbyteswritten, (unsigned long long)total_cycles,
               (int)(((float)deflate_bytes)/total_cycles * CPU_FREQ_MHZ * 8), (int)(((float)zip_result_ptr->s.totalbyteswritten)/total_cycles * CPU_FREQ_MHZ)*8, CPU_FREQ_MHZ);


        printf("Decompression done, completion code: %d, bytes: %d, crc: 0x%x\n",
               zip_result_ptr->s.completioncode, zip_result_ptr->s.totalbyteswritten, zip_result_ptr->s.crc32);
    }
    else
    {
        printf("Decompression FAILED, returned completion code: %d\n", zip_result_ptr->s.completioncode);
    }


#ifndef COMPILED_INPUT
    /* Write decompressed file out */
    fd = open("output", O_WRONLY | O_CREAT, 0644);
    if (fd < 0)
    {
        perror("Error opening decompressed ouput file\n");
        return(-1);
    }
#ifdef __linux__
    memcpy(linux_buf, output_data_ptr, zip_result_ptr->s.totalbyteswritten);
    bytes_written = write(fd, linux_buf, zip_result_ptr->s.totalbyteswritten);
#else
    bytes_written = write(fd, output_data_ptr, zip_result_ptr->s.totalbyteswritten);
#endif

    if (bytes_written < 0 || (unsigned int)bytes_written != zip_result_ptr->s.totalbyteswritten)
    {
        perror("Error writing decompressed output file\n");
        return(-1);
    }
    close(fd);
#endif

    printf("done with decomp, now doing compression\n");
    /**********************************************************************/
    /* Now do compression */
    /**********************************************************************/
    printf("\n\n");

#ifdef COMPILED_INPUT

    bytes_read = input_array_end - input_array;
    memcpy(input_data_ptr, input_array, bytes_read);
#else

    fd = open("input", O_RDONLY);
    if (fd < 0)
    {
        printf("Error opening uncompressed input file\n");
        return(-1);
    }
#ifdef __linux__
    bytes_read = read(fd, linux_buf, MAX_INPUT_SIZE + 1);
    if (bytes_read > 0)
        memcpy(input_data_ptr, linux_buf, MAX_INPUT_SIZE);
#else
    bytes_read = read(fd, input_data_ptr, MAX_INPUT_SIZE + 1);
#endif

    if (bytes_read < 0 || bytes_read > MAX_INPUT_SIZE)
    {
        printf("Error reading uncompressed input file\n");
        return(-1);
    }
    close(fd);
#endif

    start_cycles = cvmx_get_cycle();
    zip_result_ptr = process_deflate_file(1, input_data_ptr, bytes_read, output_data_ptr, output_size, queue);
    total_cycles = cvmx_get_cycle() - start_cycles;



    if (zip_result_ptr->s.completioncode == 1)
    {
        printf("Compressed %d bytes to %d bytes in %llu cycles, \n%d uncompressed Mbits/sec, %d compressed Mbits/sec (at %d Mhz)\n",
               bytes_read, zip_result_ptr->s.totalbyteswritten, (unsigned long long)total_cycles,
               (int)(((float)bytes_read)/total_cycles * CPU_FREQ_MHZ*8), (int)(((float)zip_result_ptr->s.totalbyteswritten)/total_cycles * CPU_FREQ_MHZ*8), CPU_FREQ_MHZ);


        printf("Compression done, completion code: %d, bytes: %d, crc: 0x%x\n",
               zip_result_ptr->s.completioncode, zip_result_ptr->s.totalbyteswritten, zip_result_ptr->s.crc32);
    }
    else
    {
        printf("Compression FAILED, returned completion code: %d\n", zip_result_ptr->s.completioncode);
    }

    /* Now print out size comparison */
    printf("Input size: %d, Octeon comp size: %d (%llu gzip), comp ratio: %2.1f%% (%2.1f%% gzip) # %s\n",
           bytes_read, zip_result_ptr->s.totalbyteswritten, (unsigned long long)deflate_bytes,
           100 - (zip_result_ptr->s.totalbyteswritten)/(float)bytes_read * 100,
           100 - (deflate_bytes)/(float)bytes_read * 100,
           zip_result_ptr->s.totalbyteswritten <= deflate_bytes ? "octeon smaller" : "gzip smaller");

#ifndef COMPILED_INPUT
    /* Write compressed file out */
    fd = open("output.gz", O_WRONLY | O_CREAT, 0644);
    if (fd < 0)
    {
        printf("Error opening compressed ouput file\n");
        return(-1);
    }

    /* Prepare and write out gzip header */

    /* Generic zip header - magic nums, no flags, no mtime, unknown os */
    bytes_written = write(fd, generic_gzip_header, sizeof(generic_gzip_header));

#ifdef __linux__
    memcpy(linux_buf, output_data_ptr, zip_result_ptr->s.totalbyteswritten);
    bytes_written = write(fd, linux_buf, zip_result_ptr->s.totalbyteswritten);
#else
    bytes_written = write(fd, output_data_ptr, zip_result_ptr->s.totalbyteswritten);
#endif
    if (bytes_written < 0 || (unsigned int)bytes_written != zip_result_ptr->s.totalbyteswritten)
    {
        printf("Error writing compressed output file\n");
        return(-1);
    }
    tmp = zip_result_ptr->s.crc32;
#if __BYTE_ORDER == __BIG_ENDIAN
    CVMX_ES32(tmp, tmp);
#endif
    bytes_written = write(fd, &tmp, 4);

    tmp = bytes_read;
#if __BYTE_ORDER == __BIG_ENDIAN
    CVMX_ES32(tmp, tmp);
#endif
    bytes_written = write(fd, &tmp, 4);
    close(fd);
#endif

    __cvmx_bootmem_phy_free(cvmx_ptr_to_phys(zip_result_ptr), sizeof(cvmx_zip_result_t), 0);
    __cvmx_bootmem_phy_free(cvmx_ptr_to_phys(input_data_ptr), MAX_INPUT_SIZE, 0);
    __cvmx_bootmem_phy_free(cvmx_ptr_to_phys(output_data_ptr), output_size, 0);

    if (octeon_has_feature(OCTEON_FEATURE_ZIP3) || OCTEON_IS_MODEL(OCTEON_CN68XX)) {
	    cvmx_zip_queue_shutdown(queue);
    } else
	    cvmx_zip_shutdown();

    cvmx_fpa_shutdown_pool(CVMX_FPA_ZIP_POOL);

    printf("Compress/decompress TEST PASSED\n");


    return(0);
}
