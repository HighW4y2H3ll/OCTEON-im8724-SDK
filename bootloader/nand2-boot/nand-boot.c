/*
 * Copyright 2013 Cavium, Inc. <support@cavium.com>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <nand-config.h>
#include <tinylibc.h>
#include <spi.h>
#include <string.h>
#include "octeon.h"
#include "cvmx-bootloader.h"
#include "cvmx-l2c-defs.h"

#include "tiny_stdio.c"
#include "tinylibc.c"
#include "octeon-nand.c"
#include "octeon-queue.c"
#include "octeon-fpa.c"
#include "octeon-bch.c"
#include "tiny_mem.c"

extern int cvmx_node;
extern int cvmx_uart;
extern unsigned long timestamp;
extern uint64_t cpu_speed;
extern unsigned long __start, __end;
struct octeon_nand_chip nand_chip;

typedef void (*move_and_start_t)(uint64_t dest, uint64_t src_start,
				 uint64_t src_end);

extern void move_and_start(uint64_t dest, uint64_t src_start, uint64_t src_end);
extern char move_and_start_end;

uint64_t octeon_load_stage2(struct octeon_nand_chip *chip, uint64_t load_addr)
{
	uint8_t *buffer;
	bootloader_header_t *header;
	int rc;
	int size, page_size, page, last_page;
	unsigned int start = 0, block_size;

	page_size = octeon_nand_get_page_size(chip);
	block_size = octeon_nand_get_pages_per_block(chip);
again:
	if (start * page_size > CONFIG_NAND_HEADER_SEARCH_END) {
		puts("Stage 2 bootloader not found in NAND\n");
		return 0;
	}

	buffer = (uint8_t *)load_addr;
	header = (bootloader_header_t *)buffer;

	rc = octeon_nand_hw_bch_read_page(chip, buffer, 0, start);
	start++;

	if (rc)
		goto again;

	size = octeon_get_image_size(header);
	if (!size)
		goto again;

	l2c_lock_mem_region(load_addr, size);

	if (header->image_type != BL_HEADER_IMAGE_STAGE2) {
		printf("Non-stage 2 (%d) bootloader image type starting at offset %lu\n",
		       header->image_type,
		       (uint64_t)start * (uint64_t)page_size);
		goto again;
	}
	printf("Stage 2 image size: %d bytes\n", size);
	last_page = start + (size + page_size - 1) / page_size;
	buffer += page_size;

	for (page = start; page < last_page; page++) {
		printf("\rReading NAND page %d.", page);
		rc = octeon_nand_hw_bch_read_page(chip, buffer, 0, page);
		if (rc == -2) {
			/* If we come across a bad block, just skip it and retry
			 * with the first page of the next block.
			 */
			printf("\nPage %d marked bad, skipping %d pages to next block\n",
			       page, block_size);
			last_page += block_size;
			page += block_size - 1;
			continue;
		} else if (rc == -1) {
			printf("\nPage %d ECC failed!\n", page);
			printf("Buffer: %p\n", buffer);
			print_buffer((u64)buffer, buffer, 1, page_size, 0);
			printf("NDF_CMD:         %lx\n"
			       "NDF_MISC:        %lx\n"
			       "NDF_BT_PG_INFO:  %lx\n"
			       "NDF_INT:         %lx\n"
			       "NDF_DRBELL:      %lx\n"
			       "NDF_ST_REG:      %lx\n"
			       "MIO_NDF_DMA_CFG: %lx\n",
			       cvmx_read_csr(CVMX_NDF_CMD),
			       cvmx_read_csr(CVMX_NDF_MISC),
			       cvmx_read_csr(CVMX_NDF_BT_PG_INFO),
			       cvmx_read_csr(CVMX_NDF_INT),
			       cvmx_read_csr(CVMX_NDF_DRBELL),
			       cvmx_read_csr(CVMX_NDF_ST_REG),
			       cvmx_read_csr(CVMX_MIO_NDF_DMA_CFG));
			break;
			/*return -1;*/
		}

		buffer += page_size;
	}
	puts("\n");
	if (octeon_validate_data(header)) {
		puts("Corrupted stage 2 bootloader found, searching for next bootloader\n");
		goto again;
	}
	printf("Stage 2 bootloader loaded at 0x%p\n", header);
	return header->hlen + header->dlen;
}

void __octeon_init(void)
{
	cvmx_node = cvmx_get_node_num();

	cpu_speed = octeon_get_cpu_speed();

	octeon_set_uart(cvmx_node, UART_PORT);
	octeon_set_baudrate(UART_BAUD_RATE, 0);

	main();

	octeon_do_reset();
}

int main(void)
{
	cvmx_nand_status_t status;
	int chips, chip, page_size;
	int image_size;
	int fn_size;
	move_and_start_t fn_ptr;
	cvmx_l2c_wpar_ppx_t l2c_wpar_ppx;

	cvmx_node = cvmx_get_node_num();

	cpu_speed = octeon_get_cpu_speed();

	octeon_set_uart(UART_NODE, UART_PORT);
	octeon_set_baudrate(UART_BAUD_RATE, 0);

	puts("Starting nand-boot.\n");

	l2c_wpar_ppx.u64 = 0;
	l2c_wpar_ppx.s.mask = 0;
	cvmx_write_csr(CVMX_L2C_WPAR_PPX(0), l2c_wpar_ppx.u64);


	l2c_lock_mem_region((u64)&__start, (char *)&__end - (char *)&__start);
	memset(&nand_chip, 0, sizeof(nand_chip));
	cvmx_nand_set_buffer(nand_chip.buffers.databuf);

	status = cvmx_nand_initialize(0, NAND_PROBE_MASK);
	if (status != CVMX_NAND_SUCCESS) {
		puts("Error intializing NAND\n");
		return -1;
	}

	debug("Getting active chips\n");
	chips = cvmx_nand_get_active_chips();
	debug("Active chips: 0x%x\n", chips);

	if (!chips) {
		puts("No NAND chips found!\n");
		return -1;
	}

	for (chip = 0; chips; chips >>= 1, chip++)
		if (chips & 1)
			break;

	debug("Resetting NAND chip %d\n", chip);
	status = cvmx_nand_reset(chip);
	if (status != CVMX_NAND_SUCCESS) {
		puts("Error resetting NAND\n");
		return -1;
	}

	debug("Getting page size\n");
	page_size = cvmx_nand_get_page_size(chip);
	debug("NAND page size: %d\n", page_size);

	nand_chip.selected_chip = chip;

	debug("Scanning ONFI\n");
	if (octeon_nand_scan_onfi(&nand_chip)) {
		printf("Error scanning ONFI\n");
		octeon_do_reset();
	}

	debug("Detecting ONFI\n");
	if (!nand_flash_detect_onfi(&nand_chip)) {
		puts("Error detecting ONFI\n");
		octeon_do_reset();
	}

	debug("%s: Initializing BCH\n", __func__);
	if (octeon_nand_hw_bch_init(&nand_chip)) {
		puts("Error initializing BCH\n");
		octeon_do_reset();
	}

	printf("NAND page size: %d, erase size: %d\n",
	       1 << nand_chip.page_shift, nand_chip.erasesize);
	octeon_cmdfunc(&nand_chip, NAND_CMD_RESET, 0, 0);

	if ((image_size = octeon_load_stage2(&nand_chip, STAGE2_LOAD_ADDR)) == 0) {
		puts("Error loading stage 2 bootloader\n");
		octeon_do_reset();
	}

	fn_size = (void *)(&move_and_start_end) - (void *)(&move_and_start);
	fn_ptr = (move_and_start_t)
		((uint8_t *)((STAGE2_LOAD_ADDR + image_size + 64) & ~7ull));
	memcpy(fn_ptr, &move_and_start, fn_size);
	printf("Starting stage 2 bootloader at 0x%llx\n", STAGE2_START_ADDR);

	CVMX_ICACHE_INVALIDATE;
	CVMX_ICACHE_INVALIDATE2;
	fn_ptr(STAGE2_START_ADDR, STAGE2_LOAD_ADDR, image_size);

	return 0;
}
