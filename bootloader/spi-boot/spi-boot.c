/*
 * Copyright 2013-2014 Cavium, Inc. <support@cavium.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <spi-config.h>
#include <tinylibc.h>
#include <spi.h>
#include <cvmx-bootloader.h>
#include <cvmx-mio-defs.h>

#include "tiny_stdio.c"
#include "tinylibc.c"
#include "spi.c"
#include "cvmx-l2c-defs.h"

extern int cvmx_node;
extern int cvmx_uart;
extern unsigned long timestamp;
extern uint64_t cpu_speed;
extern unsigned long __start, __end, _edata;

extern void jump_to_bootloader(uint64_t addr);
typedef void (*move_and_start_t)(uint64_t dest, uint64_t src_start,
				 uint64_t src_end);

extern void move_and_start(uint64_t dest, uint64_t src_start, uint64_t src_end);
extern char move_and_start_end;

static int spi_nor_read(int node, int cs, uint32_t offset, void *buf, int len,
			uint32_t alen)
{
	uint8_t spi_addr[4];
	int addr_offset;


	addr_offset = sizeof(spi_addr) - alen;

	spi_addr[0] = (offset >> 24) & 0xff;
	spi_addr[1] = (offset >> 16) & 0xff;
	spi_addr[2] = (offset >> 8) & 0xff;
	spi_addr[3] = offset & 0xff;

	return spi_read(node, cs, &spi_addr[addr_offset], alen, buf, len);
}

int find_header(int node, int cs, int start_offset, int end_offset,
		int interval, struct bootloader_header *header,
		int flags __attribute__((unused)), uint32_t alen)
{
	int offset;
	int rc;

	for (offset = start_offset; offset < end_offset; offset += interval) {
		rc = spi_nor_read(node, cs, offset,
				  header, sizeof(*header), alen);
		if (rc < 0)
			return -1;
#ifdef DEBUG
		printf("Searching header at offset 0x%x, magic: 0x%x\n",
		       offset, header->magic);
#endif
		if (octeon_validate_bootloader_header(header))
			return offset;
	}
	return -1;
}

int load_spi(int node, int cs, int offset, u64 addr, uint32_t alen)
{
	struct bootloader_header *header = (struct bootloader_header *)addr;
	u8 *buffer;
	uint start;
	uint image_size;
	int fn_size;
	move_and_start_t fn_ptr;
	int rc;
	u8 __attribute__((unused)) id[5];

	spi_init();

	if (alen > 2) {
		if (spi_read_id(node, cs, id))
			puts("Error reading ID\n");
		else
			printf("SPI ID: %02x:%02x:%02x:%02x:%02x\n",
			       id[0], id[1], id[2], id[3], id[4]);
	}

	start = (uint)((char *)&_edata - (char *)&__start);
	start += CONFIG_HEADER_SEARCH_INCREMENT - 1;
	start &= ~(CONFIG_HEADER_SEARCH_INCREMENT - 1);

	offset = find_header(node, cs, start, CONFIG_HEADER_SEARCH_END,
			     CONFIG_HEADER_SEARCH_INCREMENT, header, 0, alen);
	if (offset < 0)
		puts("header not found\n");
	else {
		printf("header found at offset 0x%x\n", offset);

		printf("Image %d.%d: address: 0x%lx, header length: %d, data length: %d\n",
		       header->maj_rev, header->min_rev, header->address,
		       header->hlen, header->dlen);
		buffer = CASTPTR(uint8_t, STAGE2_LOAD_ADDR);
		image_size = header->hlen + header->dlen;
		l2c_lock_mem_region(STAGE2_LOAD_ADDR, image_size + 256);
		debug("Reading %u bytes to address %p\n", image_size, buffer);
		rc = spi_nor_read(node, cs, offset, buffer, image_size, alen);
		if (rc < 0) {
			puts("Error reading next bootloader\n");
			return -1;
		}
		printf("Validating data...\n");
		if (octeon_validate_data((struct bootloader_header *)buffer)) {
			puts("Corrupted bootloader\n");
			return -1;
		}
		fn_size = (void *)(&move_and_start_end) - (void *)(&move_and_start);
		fn_ptr = (move_and_start_t)
		((uint8_t *)((STAGE2_LOAD_ADDR + image_size + 64) & ~7ull));
		memcpy(fn_ptr, &move_and_start, fn_size);
		printf("Starting next bootloader at 0x%llx\n", STAGE2_START_ADDR);

		CVMX_ICACHE_INVALIDATE;
		CVMX_ICACHE_INVALIDATE2;
		fn_ptr(STAGE2_START_ADDR, STAGE2_LOAD_ADDR, image_size);
	}

	return 0;
}

int main(void)
{
	cvmx_l2c_wpar_ppx_t l2c_wpar_ppx;
	union cvmx_mio_boot_pin_defs pin_defs;
	uint32_t id;
	uint32_t alen;
	bool is_70xx;
	uint8_t defs;

	l2c_wpar_ppx.u64 = 0;
	l2c_wpar_ppx.s.mask = 0;
	cvmx_write_csr(CVMX_L2C_WPAR_PPX(0), l2c_wpar_ppx.u64);

	/* Read Octeon device ID */
	asm("mfc0 %0, $15,0" : "=r"(id));
	is_70xx = ((id >> 8) & 0xffff) == 0x0d96;
	pin_defs.u64 = cvmx_read_csr(CVMX_MIO_BOOT_PIN_DEFS);
	defs = is_70xx ? pin_defs.cn70xx.device : pin_defs.cn73xx.device;

	/* Assume if we're loaded from parallel NOR flash that a 32-bit SPI NOR
	 * is being used.
	 */
	if (defs == 0)
		alen = 4;
	else
		alen = (defs - 4) + 2;

	cvmx_node = cvmx_get_node_num();

	cpu_speed = octeon_get_cpu_speed();

	octeon_set_uart(UART_NODE, UART_PORT);
	octeon_set_baudrate(UART_BAUD_RATE, 0);
	puts("SPI stage 1 bootloader\n");
	load_spi(0, 0, 0x2000, STAGE2_LOAD_ADDR, alen);

	puts("Could not start next bootloader\n");
	mdelay(10);
	octeon_do_reset();
	return 0;
}
