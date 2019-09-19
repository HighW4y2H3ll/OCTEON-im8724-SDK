/***********************license start************************************
 * Copyright (c) 2003-2008 Cavium Inc. (support@cavium.com). All rights
 * reserved.
 *
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials provided
 *       with the distribution.
 *
 *     * Neither the name of Cavium Inc. nor the names of
 *       its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written
 *       permission.
 *
 * TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
 * AND WITH ALL FAULTS AND CAVIUM NETWORKS MAKES NO PROMISES, REPRESENTATIONS
 * OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH
 * RESPECT TO THE SOFTWARE, INCLUDING ITS CONDITION, ITS CONFORMITY TO ANY
 * REPRESENTATION OR DESCRIPTION, OR THE EXISTENCE OF ANY LATENT OR PATENT
 * DEFECTS, AND CAVIUM SPECIFICALLY DISCLAIMS ALL IMPLIED (IF ANY) WARRANTIES
 * OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR
 * PURPOSE, LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET
 * POSSESSION OR CORRESPONDENCE TO DESCRIPTION.  THE ENTIRE RISK ARISING OUT
 * OF USE OR PERFORMANCE OF THE SOFTWARE LIES WITH YOU.
 *
 *
 * For any questions regarding licensing please contact marketing@cavium.com
 *
 **********************license end**************************************/
#ifndef OCTEON_REMOTE_H
#define OCTEON_REMOTE_H

/**
 * @file octeon-remote.h
 *
 * Interface to Octeon remotely using various transports
 *
 * $Id: octeon-remote.h 156174 2017-03-20 22:14:34Z cchavva $
 */

#ifdef	__cplusplus
extern "C" {
#endif

#ifndef OCTEON_REMOTE_DEFAULT
#define OCTEON_REMOTE_DEFAULT NULL
#endif

/**
 * - regs[0][0-31] = General purpose registers
 * - regs[0][32] = Lo
 * - regs[0][33] = Hi
 * - regs[0][64-97] = Floating point registers
 * - regs[0][128-255] = EJTAG special registers
 * - regs[1][0-255] = COP0 registers indexed by (reg*8)+select
 * - tlb[*][0] = EntryHi
 * - tlb[*][1] = Pagemask
 * - tlb[*][2] = EntryLo0
 * - tlb[*][3] = EntryLo1
 */
typedef struct
{
    uint64_t regs[2][256];
    uint64_t tlb[256][4];
    uint64_t pad[512];
} octeon_remote_registers_t;

/**
 * This typedef represents the data per core returned by an
 * octeon_remote_get_sample() function call. The get_sample will
 * populate as many of these fields it can with a minimum
 * performance impact to the core. Not all transports can fill
 * in all fields, but "pc" is generally supplied at a minimum.
 * Fields that cannot be filled will be zero.
 */
typedef struct
{
    uint64_t pc;
    uint64_t perf_count[4];
    uint64_t var;
} octeon_remote_sample_t;

/**
 * This structure represents the API required to implement a
 * remote transport. If all these functions can be implemented
 * over some remote protocol (PCI, EJTAG, etc), then the Octeon
 * command line utilities should work. Which protocol that is
 * used can be controlled either by and argument to
 * octeon_remote_open() or the environment variable
 * OCTEON_REMOTE_PROTOCOL.
 */
typedef struct
{
    int debug; /* The current debug level as set by OCTEON_REMOTE_DEBUG */
    int debug_indent; /* The current debug indent level */
    uint32_t model; /* The remote's Octeon model, cached for fast access */

    int is_le; /* The remote byte order mode, cached */
    /* If the remote memory is accessed in LE way if the core is in LE mode.
     * It is not used if the remote core is in BE mode */
    int memory_le;

    /* The following function pointers must be populated by the underlying
        remote transport */
    int (*open)(const char *remote_spec);
    void (*close)(void);
    void (*read_mem)(void *buffer, uint64_t physical_address, int length);
    void (*write_mem)(uint64_t physical_address, const void *buffer, int length);
    uint32_t (*get_model)(void);
    void (*start_cores)(uint64_t start_mask);
    void (*stop_cores)(uint64_t stop_mask);
    uint64_t (*get_running_cores)(void);
    uint32_t (*get_num_cores)(void);
    int (*get_core_state)(int core, octeon_remote_registers_t *registers);
    int (*set_core_state)(int core, const octeon_remote_registers_t *registers);
    int (*get_sample)(uint64_t coremask, octeon_remote_sample_t sample[64]);

    /* The following functions are optional. If they are not set, a default
        implementation is used. In general the default implementation should
        be used */
    void (*lock)(void);
    void (*unlock)(void);
    int (*reset)(int stop_core);
    uint64_t (*read_register)(int core, int reg);
    void (*write_register)(int core, int reg, uint64_t value);
    uint64_t (*read_csr)(uint64_t physical_address);
    void (*write_csr)(uint64_t physical_address, uint64_t value);
    uint32_t (*read_csr32)(uint64_t physical_address);
    void (*write_csr32)(uint64_t physical_address, uint32_t value);
} octeon_remote_funcs_t;

/**
 * Utility function to display debug messages. Only messages at
 * or below the current debug level are displayed.
 *
 * @param level  Level for this message
 * @param format prinf format
 */
void octeon_remote_debug(int level, const char *format, ...) __attribute__ ((format(printf, 2, 3)));

/**
 * Utility function to display library error messages similar to
 * perror(). As an extension, this functions takes full printf
 * style arguments instead of the simple string used by perror().
 * Only messages at or below the current debug level are
 * displayed.
 *
 * @param level  Level for this message
 * @param format prinf format
 */
void octeon_remote_perror(int level, const char *format, ...) __attribute__ ((format(printf, 2, 3)));

/**
 * Utility function to display output messages without line
 * prefixes. Only messages at or below the current debug level are
 * displayed.
 *
 * @param level  Level for this message
 * @param format prinf format
 */
void octeon_remote_output(int level, const char *format, ...) __attribute__ ((format(printf, 2, 3)));

void octeon_remote_debug_call(int is_return, const char *format, ...) __attribute__ ((format(printf, 2, 3)));
#define OCTEON_REMOTE_DEBUG_CALLED(format, ...) \
    octeon_remote_debug_call(0, "Called %s(" format ")\n", __FUNCTION__, ##__VA_ARGS__);
#define OCTEON_REMOTE_DEBUG_RETURNED(format, ...) \
    octeon_remote_debug_call(1, "Return %s(" format ")\n", __FUNCTION__, ##__VA_ARGS__);

/**
 * Open a connection to a remote Octeon.
 *
 * @param remote_spec
 *               Remote protocol spec to use. If NULL, it will be taken from the
 *               OCTEON_REMOTE_PROTOCOL environment variable.
 *
 * @return Zero on success, negative on failure.
 */
int octeon_remote_open(const char *remote_spec, int debug);

/**
 * Close a remote connection created by octeon_remote_open()
 */
void octeon_remote_close(void);

/**
 * Read a CSR from a remote Octeon
 *
 * @param physical_address
 *               Physical address of the CSR. Bits 63-49 should be zero.
 *
 * @return 64bit value of the CSR
 */
uint64_t octeon_remote_read_csr(uint64_t physical_address);

/**
 * Write a CSR on a remote Octeon
 *
 * @param physical_address
 *               Physical address of the CSR. Bits 63-49 should be zero.
 * @param value  Value to write
 */
void octeon_remote_write_csr(uint64_t physical_address, uint64_t value);

/**
 * Read a 32bit CSR from a remote Octeon
 *
 * @param physical_address
 *               Physical address of the CSR. Bits 63-49 should be zero.
 *
 * @return 32bit value of the CSR
 */
uint32_t octeon_remote_read_csr32(uint64_t physical_address);

/**
 * Write a 32bit CSR on a remote Octeon
 *
 * @param physical_address
 *               Physical address of the CSR. Bits 63-49 should be zero.
 * @param value  Value to write
 */
void octeon_remote_write_csr32(uint64_t physical_address, uint32_t value);

/**
 * Read data from a physical memory address.
 *
 * @param buffer_ptr Buffer to put the data in
 * @param physical_address
 *                   Physical address to read from. Bits 63-38 should be zero.
 * @param length     Length to read in bytes
 */
void octeon_remote_read_mem(void *buffer, uint64_t physical_address, int length);

/**
 * Write data to a physical memory address
 *
 * @param physical_address
 *                   Physical address to write to. Bits 63-38 should be zero.
 * @param buffer_ptr Buffer containing the data to write
 * @param length     Number of bytes to write
 */
void octeon_remote_write_mem(uint64_t physical_address, const void *buffer, int length);

/**
 * Return the Processor ID of the Octeon. This should be identical
 * to the value found in the COP0 PRID register
 *
 * @return Processor ID
 */
uint32_t octeon_remote_get_model(void) __attribute__ ((pure));

/**
 * Take the cores in the bit vector out of the debug exception.
 *
 * @param start_mask Cores to start
 */
void octeon_remote_start_cores(uint64_t start_mask);

/**
 * Cause the cores in the stop mask to take a debug exception
 *
 * @param stop_mask Cores to stop
 */
void octeon_remote_stop_cores(uint64_t stop_mask);

/**
 * Get a mask where each bit represents a running core. If a core
 * is in the debug exception handler, it's bit will be zero.
 *
 * @return Bit set for every running core
 */
uint64_t octeon_remote_get_running_cores(void);

/**
 * Return the number of cores available in the chip
 *
 * @return available cores on the chip
 */
uint32_t octeon_remote_get_num_cores(void);

/**
 * Get all registers, COP0, TLB, etc for a core
 *
 * @param core      Core to get state info for
 * @param registers All of the core's internal state
 *
 * @return Zero on success, negative on failure
 */
int octeon_remote_get_core_state(int core, octeon_remote_registers_t *registers);

/**
 * Set all registers, COP0, TLB, etc for a given core
 *
 * @param core      Core to set state for
 * @param registers All date for the core
 *
 * @return Zero on success, negative on failure
 */
int octeon_remote_set_core_state(int core, const octeon_remote_registers_t *registers);

/**
 * Read a single Octeon register
 *
 * @param core   Core to read the register from
 * @param reg    Register to read
 *
 * @return Register value
 */
uint64_t octeon_remote_read_register(int core, int reg);

/**
 * Write a single Octeon register.
 *
 * @param core   Core to write the register on
 * @param reg    Register to write
 * @param value  Value to write
 */
void octeon_remote_write_register(int core, int reg, uint64_t value);

/**
 * Lock the remote protocol for exclusive access. This is needed
 * if multiple programs might be accessing the same Octeon at the
 * same time.
 */
void octeon_remote_lock(void);

/**
 * Unlock the remote protocol for exclusive access. This is needed if multiple
 * programs might be accessing the same Octeon at the same time.
 */
void octeon_remote_unlock(void);

/**
 * Reset a remote Octeon. This performs a full chip soft reset.
 *
 * @param stop_core Attempt to stop core 0 from booting, if possible. If the remote
 *                  protocol supports it, core 0 is stopped before executing
 *                  instructions from flash.
 *
 * @return Zero on success, negative on failure.
 */
int octeon_remote_reset(int stop_core);

/**
 * Sample performance / profiling information for a set of cores
 * minimizing the performance impact of taking the sample. The
 * amount of data returned may vary depending on the remote
 * protocol used, but at least the program counter for each core
 * will normally be supported. Values that could not be sampled
 * will be zero.
 *
 * @param coremask Each set bit represent a core that should be sampled.
 * @param sample   Array of samples to populate. Note that the sample array must
 *                 contain elements for cores not set in the coremask. These may
 *                 be filled with sample data even if the coremask bit is clear.
 *                 Some transports may sample all core efficiently and yield
 *                 unwanted core samples.
 *
 * @return Zero on success, negative on failure
 */
int octeon_remote_get_sample(uint64_t coremask, octeon_remote_sample_t sample[64]);

/**
 * Write a 16 bit value into Octeon's memory. This code automatically correct
 * for differences between Octeon's endianness and where we are running.
 *
 * @param physical_address
 *               Physical address to write to. Bits 63-38 should be zero.
 * @param data   Data to write
 */
void octeon_remote_write_mem16(uint64_t physical_address, uint16_t data);

/**
 * Write a 32 bit value into Octeon's memory. This code automatically correct
 * for differences between Octeon's endianness and where we are running.
 *
 * @param physical_address
 *               Physical address to write to. Bits 63-38 should be zero.
 * @param data   Data to write
 */
void octeon_remote_write_mem32(uint64_t physical_address, uint32_t data);

/**
 * Write a 64 bit value into Octeon's memory. This code automatically correct
 * for differences between Octeon's endianness and where we are running.
 *
 * @param physical_address
 *               Physical address to write to. Bits 63-38 should be zero.
 * @param data   Data to write
 */
void octeon_remote_write_mem64(uint64_t physical_address, uint64_t data);

/**
 * Read a 16 bit value from Octeon's memory. This code automatically correct for
 * differences between Octeon's endianness and where we are running.
 *
 * @param physical_address
 *               Physical address to read from. Bits 63-38 should be zero.
 *
 * @return Data read
 */
uint16_t octeon_remote_read_mem16(uint64_t physical_address);

/**
 * Read a 32 bit value from Octeon's memory. This code automatically correct for
 * differences between Octeon's endianness and where we are running.
 *
 * @param physical_address
 *               Physical address to read from. Bits 63-38 should be zero.
 *
 * @return Data read
 */
uint32_t octeon_remote_read_mem32(uint64_t physical_address);

/**
 * Read a 64 bit value from Octeon's memory. This code automatically correct for
 * differences between Octeon's endianness and where we are running.
 *
 * @param physical_address
 *               Physical address to read from. Bits 63-38 should be zero.
 *
 * @return Data read
 */
uint64_t octeon_remote_read_mem64(uint64_t physical_address);

/**
 * Send a string to u-boot as a command.
 *
 * @param cmd_str String to send
 * @param wait_hundredths
 *                Time to wait for u-boot to accept the command.
 *
 * @return Zero on success, negative on failure.
 */
int octeon_remote_send_bootcmd(const char *cmd_str, int wait_hundredths);

/**
 * Wait for u-boot to boot and be waiting for a command.
 *
 * @param wait_time_hundredths
 *               Maximum time to wait
 *
 * @return Zero on success, negative on failure.
 */
int octeon_remote_wait_for_bootloader(int wait_time_hundredths);

/**
 * Find a named block on the remote Octeon
 *
 * @param name      Name of block to find
 * @param base_addr Address the block is at (OUTPUT)
 * @param size      The size of the block (OUTPUT)
 *
 * @return One on success, zero on failure.
 */
int octeon_remote_named_block_find(const char *name, uint64_t *base_addr, uint64_t *size);

/**
 * Allocate a named block on the remote Octeon
 *
 * @param size      Size of the block in bytes
 * @param min_addr  Minimum address the block can be located at. Zero is a good default.
 * @param max_addr  Maximum upper address of the block. Use 0 if you don't care.
 * @param alignment Byte alignment for the block. Must be 0 or a power of two.
 * @param name      Name to give the new block
 *
 * @return Address of the named block or 0xffffffffffffffff on failure.
 */
int64_t octeon_remote_named_block_alloc(uint64_t size, uint64_t min_addr, uint64_t max_addr, uint64_t alignment, const char *name);

/**
 * Return non-zero of Octeon's memory can be accessed.
 *
 * @return True if memory is configured.
 */
int octeon_remote_mem_access_ok(void);

/**
 * Write to a remote Octeon's "PCI console" data structures.
 *
 * @param console_desc_addr
 *               Location of the console
 * @param console_num
 *               Which console
 * @param buffer Buffer to write
 * @param write_reqest_size
 *               Number of bytes to write
 * @param flags  Optional flags
 *
 * @return Number of bytes written
 */
int octeon_remote_console_host_write(uint64_t console_desc_addr, unsigned int console_num, const char * buffer, int write_reqest_size, uint32_t flags);

/**
 * Read from a remote Octeon's "PCI console" data structures.
 *
 * @param console_desc_addr
 *                 Location of the console
 * @param console_num
 *                 Which console
 * @param buffer   Buffer to read into
 * @param buf_size Number of bytes to read
 * @param flags    Optional flags
 *
 * @return Number of bytes read
 */
int octeon_remote_console_host_read(uint64_t console_desc_addr, unsigned int console_num, char * buffer, int buf_size, uint32_t flags);

/**
 * Determine the byte order mode of a remote Oceton.
 *
 * @return One if Octeon is in LE mode.
 */
int octeon_remote_is_le(void);

/**
 * Determine the byte order mode of a remote Oceton memory model.
 *
 * @return One if Octeon memory should be accessed is in LE mode.
 */
int octeon_remote_is_mem_le(void);

/**
 * Determine the OCTEON_REMOTE_PROTOCOL used for doing byte swapping
 *
 * @return return value of memory_le
 */
int octeon_remote_protocol(void);

/**
 * Write a 32 bit value into Octeon's memory. This code automatically correct
 * for differences between Octeon's endianness and where we are running.
 *
 * @param physical_address
 *               Physical address to write to. Bits 63-38 should be zero.
 */
void octeon_remote_spinlock_lock(uint64_t physical_address);

/**
 * Write a 32 bit value into Octeon's memory. This code automatically correct
 * for differences between Octeon's endianness and where we are running.
 *
 * @param physical_address
 *               Physical address to write to. Bits 63-38 should be zero.
 */
void octeon_remote_spinlock_unlock(uint64_t physical_address);

/**
 * Atomic:'spinlock_lock':Write 1 to a physical memory address and return the previous value
 *
 * @param physical_address
 *                   Physical address to write to. Bits 63-38 should be zero.
 */
void pci_spinlock_lock(uint64_t physical_address);

/**
 * Atomic:'spinlock_unlock':Write 0 to a physical memory address
 *
 * @param physical_address
 *                   Physical address to write to. Bits 63-38 should be zero.
 */
void pci_spinlock_unlock(uint64_t physical_address);


#ifdef	__cplusplus
}
#endif

#endif
