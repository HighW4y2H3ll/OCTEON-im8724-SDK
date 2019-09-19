/*
 * Copyright 2013-2016 Cavium, Inc. <support@cavium.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <spi-config-1.5.h>
#include <tinylibc.h>
#include <spi.h>
#include <cvmx-bootloader.h>
#include <cvmx-mio-defs.h>
#include <cvmx-gpio-defs.h>
#include <cvmx-gpio.h>

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

static int spi_nor_read(int node, int cs, uint32_t offset, void *buf, int len)
{
	uint8_t spi_addr[4];
	int alen;
	int addr_offset;
	union cvmx_mio_boot_pin_defs pin_defs;
	uint32_t id;
	bool is_70xx;
	uint8_t defs;

	/* Read Octeon device ID */
	asm("mfc0 %0, $15, 0" : "=r"(id));

	pin_defs.u64 = cvmx_read_csr(CVMX_MIO_BOOT_PIN_DEFS);
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

	addr_offset = sizeof(spi_addr) - alen;
	spi_addr[0] = (offset >> 24) & 0xff;
	spi_addr[1] = (offset >> 16) & 0xff;
	spi_addr[2] = (offset >> 8) & 0xff;
	spi_addr[3] = offset & 0xff;

	return spi_read(node, cs, &spi_addr[addr_offset], alen, buf, len);
}

int find_header(int node, int cs, int start_offset, int end_offset,
		int interval, struct bootloader_header *header,
		int flags __attribute__((unused)))
{
	int offset;
	int rc;

	for (offset = start_offset; offset < end_offset; offset += interval) {
		debug("Searching offset 0x%x\n", offset);
		rc = spi_nor_read(node, cs, offset, header, sizeof(*header));
		if (rc < 0) {
			puts("Error reading bootloader header\n");
			return rc;
		}

#ifdef DEBUG
		print_buffer(offset, header, 1, sizeof(*header), 0);
#endif
		if (octeon_validate_bootloader_header(header))
			return offset;
	}
	return -1;
}

static inline int next_header_offset(const struct bootloader_header *header,
				     int offset)
{
	offset += header->hlen + header->dlen;
	offset += CONFIG_HEADER_SEARCH_INCREMENT - 1;
	offset &= ~(CONFIG_HEADER_SEARCH_INCREMENT - 1);
	return offset;
}

int load_spi(int node, int cs, int offset, u64 addr)
{
	struct bootloader_header *header;
	u8 *buffer;
	uint image_size = 0;
	int fn_size;
	move_and_start_t fn_ptr;
	int rc;
	u8 __attribute__((unused)) id[5];
	bool failsafe = false;
	int headers_found = 0;
	uint32_t offsets[4] = {0, 0, 0, 0};
	bootloader_image_t bootloader_type[4];
	uint32_t bootloader_size[4];
	uint32_t last_offset = 0;
	int i;
	bool stage2 = false;
	bool found = false;
	int found_idx = -1;

	spi_init();

#ifdef CONFIG_FAILSAFE_GPIO
	cvmx_gpio_cfg(0, 0);
	if (cvmx_gpio_read() & (1 << (CONFIG_FAILSAFE_GPIO)))
		failsafe = true;
#endif
	if (spi_read_id(node, cs, id)) {
		puts("Error reading ID\n");
		return -1;
	} else {
		printf("SPI ID: %02x:%02x:%02x:%02x:%02x\n",
		       id[0], id[1], id[2], id[3], id[4]);
	}
	for (i = 0; i < 4; i++)
		bootloader_type[i] = BL_HEADER_IMAGE_UNKNOWN;

	header = (struct bootloader_header *)addr;
	buffer = (uint8_t *)addr;
	do {

		offset = find_header(node, cs, offset, CONFIG_HEADER_SEARCH_END,
				     CONFIG_HEADER_SEARCH_INCREMENT, header, 0);
		if (offset < 0)
			break;
		bootloader_type[headers_found] = header->image_type;
		offsets[headers_found] = offset;
		bootloader_size[headers_found] = header->hlen + header->dlen;
		headers_found++;
		printf("Header %d found at offset 0x%x\n", headers_found,
		       offset);
		offset = next_header_offset(header, offset);
	} while (headers_found < 4);

	/* See if we're looking for a stage 2 bootloader */
	for (i = 0; i < headers_found; i++)
		if (bootloader_type[i] == BL_HEADER_IMAGE_STAGE2)
			stage2 = true;

	if (!headers_found) {
		printf("No bootloader headers found!\n");
		return -1;
	}

	/* If failsafe look for the first valid bootloader.  Note that the
	 * if/else is hidden in the for loop comparison.
	 */
	for (i = 0; failsafe && i < headers_found; i++) {
		if (stage2 && bootloader_type[i] != BL_HEADER_IMAGE_STAGE2)
			continue;
		image_size = bootloader_size[i];
		l2c_lock_mem_region(addr, image_size + 256);
		rc = spi_nor_read(node, cs, offsets[i], buffer, image_size);
		if (rc < 0) {
			printf("Error reading bootloader image at offset 0x%x\n",
			       offsets[i]);
			return -1;
		}
		if (!octeon_validate_data(header)) {
			found = true;
			last_offset = offsets[i];
			found_idx = i;
			debug("Found failsafe bootloader at offset 0x%x\n", last_offset);
			break;
		}
		printf("Corrupted bootloader found at offset 0x%x\n",
		       offsets[i]);
	}

	/* For non-failsafe look for the last valid bootloader */
	for (i = headers_found - 1; !failsafe && i >= 0; i--) {
		if (stage2 && bootloader_type[i] != BL_HEADER_IMAGE_STAGE2)
			continue;
		image_size = bootloader_size[i];
		l2c_lock_mem_region(addr, image_size + 256);
		rc = spi_nor_read(node, cs, offsets[i], buffer, image_size);
		if (rc < 0) {
			printf("Error reading bootloader image at offset 0x%x\n",
			       offsets[i]);
			return -1;
		}
		if (!octeon_validate_data(header)) {
			found = true;
			last_offset = offsets[i];
			found_idx = i;
			break;
		} else {
			printf("Corrupted bootloader found at offset 0x%x\n",
			       offsets[i]);
		}
	}

	if (!found) {
		puts("No valid bootloader found.\n");
		return -1;
	} else {
		offset = last_offset;
#ifdef CONFIG_FAILSAFE_GPIO
		printf("Found bootloaders, booting %sbootloader %d of %d at offset 0x%x.\n",
		       failsafe ? "failsafe " : "", found_idx + 1,
		       headers_found, offset);
#else
		printf("Found bootloaders, booting bootloader %d of %d at offset 0x%x.\n",
		       found_idx + 1, headers_found, offset);
#endif
	}

	/* Copy our trampoline code to a location past the end of the bootloader
	 */
	fn_size = (void *)(&move_and_start_end) - (void *)(&move_and_start);
	fn_ptr = (move_and_start_t)
		((uint8_t *)((STAGE2_LOAD_ADDR + image_size + 64) & ~7ull));
	memcpy(fn_ptr, &move_and_start, fn_size);
#ifdef CONFIG_FAILSAFE_GPIO
	printf("Starting next %sbootloader at 0x%llx\n",
	       failsafe ? "failsafe " : "", STAGE2_START_ADDR);
#else
	printf("Starting next bootloader at 0x%llx\n", STAGE2_START_ADDR);
#endif
	CVMX_ICACHE_INVALIDATE;
	CVMX_ICACHE_INVALIDATE2;
	fn_ptr(STAGE2_START_ADDR, STAGE2_LOAD_ADDR, image_size);

	return 0;
}

int main(void)
{
	cvmx_l2c_wpar_ppx_t l2c_wpar_ppx;

	l2c_wpar_ppx.u64 = 0;
	l2c_wpar_ppx.s.mask = 0;
	cvmx_write_csr(CVMX_L2C_WPAR_PPX(0), l2c_wpar_ppx.u64);

	cvmx_node = cvmx_get_node_num();

	cpu_speed = octeon_get_cpu_speed();

	octeon_set_uart(UART_NODE, UART_PORT);
	octeon_set_baudrate(UART_BAUD_RATE, 0);
	puts("SPI stage 1.5 bootloader\n");
	load_spi(0, 0, CONFIG_HEADER_SEARCH_START, STAGE2_LOAD_ADDR);

	puts("Could not start next bootloader\n");
	octeon_do_reset();
	return 0;
}
