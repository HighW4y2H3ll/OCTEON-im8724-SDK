/*
 * (C) Copyright 2015 Interface Masters
 * Marcel Birthelmer <marcelb@interfacemasters.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <command.h>
#include <asm/mipsregs.h>
#include <asm/arch/octeon_boot_bus.h>
#include <asm/gpio.h>
#include <asm/arch/cvmx.h>
#include <libfdt.h>
#include <fdtdec.h>
#include <fdt_support.h>
#include <asm/arch/cvmx-uart.h>
#include <asm/arch/cvmx-gpio.h>
#ifdef CONFIG_SYS_NS16550
# include <ns16550.h>
#endif

DECLARE_GLOBAL_DATA_PTR;

#define CPLD_ADDR_LPC_CMD		0x48
#define CPLD_ADDR_LPC_STAT		0x49
#define CPLD_LPC_STAT_EV		0x01
#define CPLD_LPC_STAT_TOUT		0x08
#define CPLD_LPC_STAT_PERR		0x10
#define CPLD_LPC_STAT_INV		0x20

#define CS_LPC_IO			3
#define CS_LPC_MEM			4

#define LPC_SUCCESS			0
#define LPC_ERROR_UNKNOWN		1
#define LPC_INV_SYNC			2
#define LPC_PERIPH_ERR			3
#define LPC_TIMEOUT			4

extern void ns16550_serial_initialize(void);

static uint64_t lpc_base = 0;

static int lpc_io_stat(void)
{
	return LPC_SUCCESS;
}

static int lpc_io_read(uint16_t addr)
{
	uint8_t data;
	int lpc_stat;

	data = cvmx_read64_uint8(lpc_base + addr);

	lpc_stat = lpc_io_stat();

	return (lpc_stat == LPC_SUCCESS) ? data : lpc_stat;
}

static int lpc_io_write(uint16_t addr, uint8_t data)
{
	cvmx_write64_uint8(lpc_base + addr, data);

	return lpc_io_stat();
}

int lpc_io_init(void)
{
	int bootbus_node_offset = fdt_path_offset(gd->fdt_blob, "/soc/bootbus");
	int node_offset;
	/* Initialize the serial port early */

	if (bootbus_node_offset < 0) {
		puts("Error: cannot find boot bus in device tree\n");
		printf("fdt blob: %p\n", gd->fdt_blob);
		return bootbus_node_offset;
	}
	node_offset = fdt_node_offset_by_compatible(gd->fdt_blob,
						    bootbus_node_offset,
						    "imt,n83x-cpld-lpc");
	if (node_offset < 0) {
		puts("Error: cannot find LPC in device tree\n");
		return node_offset;
	}
	return octeon_boot_bus_generic_init(gd->fdt_blob, node_offset,
					    bootbus_node_offset, NULL,
					    &lpc_base);
}

#ifdef CONFIG_OCTEON_NIAGARA83X_W83627

#define W83627_UART_A_BASE_DEFAULT	0x3F8
#define W83627_UART_B_BASE_DEFAULT	0x2F8

/** depends on the strapping on the carrier*/
#define W83627_EFER		0x4E
#define W83627_EF_ENTER	0x87
#define W83627_EF_EXIT	0xAA

#define W83627_EFIR		(W83627_EFER)
#define W83627_EFDR		(W83627_EFIR+1)

#define W83627_CR_GL_ID_MSB	0x20
#define W83627_CR_GL_ID_LSB	0x21
#define W83627_CR_GL_OPT	0x24

#define W83627_CR_GL_DEV	0x07
#define W83627_DEV_
#define W83627_DEV_UART_A	0x02
#define W83627_DEV_UART_B	0x03

#define W83627_CR_UARTA_BASE_H	0x60
#define W83627_CR_UARTA_BASE_L	0x61
#define W83627_CR_UARTA_CLK	0xF0
#define W83627_UART_CLK_14_769	0x03

#define W83627_ID		0xA020	/* from the spec: A02x, where x is the version */

static int w83627_ef_read(uint8_t addr)
{
	int ret = lpc_io_write(W83627_EFIR, addr);
	if (ret != LPC_SUCCESS)
		return ret;

	return lpc_io_read(W83627_EFDR);
}

static int w83627_ef_write(uint8_t addr, uint8_t data)
{
	int ret = lpc_io_write(W83627_EFIR, addr);
	if (ret != LPC_SUCCESS)
		return ret;

	return lpc_io_write(W83627_EFDR, data);
}

int w83627_init(void)
{
	int ret;
	char devid_str[64];
	uint16_t w83627_id;

	ret = lpc_io_write(W83627_EFER, W83627_EF_ENTER);
	if (ret != LPC_SUCCESS)
		return ret;

	ret = lpc_io_write(W83627_EFER, W83627_EF_ENTER);
	if (ret != LPC_SUCCESS)
		return ret;

	w83627_id =
	    (w83627_ef_read(W83627_CR_GL_ID_MSB) << 8) |
	    (w83627_ef_read(W83627_CR_GL_ID_LSB));

	sprintf(devid_str, "SuperIO dev id: %x\r\n", w83627_id);
	cvmx_uart_write_string(0, devid_str);

	if ((w83627_id & 0xfff0) != W83627_ID)
		return 1;

	ret = w83627_ef_write(W83627_CR_GL_OPT, 0x06);	//set the input clock to 24MHz
	if (ret != LPC_SUCCESS)
		return ret;

	ret = w83627_ef_write(W83627_CR_GL_DEV, W83627_DEV_UART_A);
	if (ret != LPC_SUCCESS)
		return ret;

	ret = w83627_ef_write(W83627_CR_UARTA_CLK, W83627_UART_CLK_14_769);
	if (ret != LPC_SUCCESS)
		return ret;

	return lpc_io_write(W83627_EFER, W83627_EF_EXIT);
}

int do_w83627_init(cmd_tbl_t * cmdtp, int flag, int argc, char *const argv[])
{
	int ret = w83627_init();

	if (ret == LPC_SUCCESS)
		printf("W83627 initialized successfully\n");
	else
		printf("W83627 initialization error: %d\n", -ret);
	return 0;
}

U_BOOT_CMD(w83627_init, 1, 0, do_w83627_init,
	   "Initialize the W83627 Super I/O Chip", "");

#endif

int do_lpc_rd(cmd_tbl_t * cmdtp, int flag, int argc, char *const argv[])
{
	uint16_t addr, val;
	char *tmp;
	int len = 1, i;
	int ret;

	if ((argc != 3) && (argc != 2))
		return CMD_RET_USAGE;

	if (lpc_base == 0) {
		printf("Error: LPC Base not initialized!\n");
		return 1;
	}

	addr = simple_strtol(argv[1], &tmp, 16);

	if (*tmp == ':' || *tmp == '-') {
		len = simple_strtol(tmp, NULL, 16) - addr;

		if (len + addr > 0x100)
			len = addr - 0x100;
	}

	if (argc == 3) {
		if (!strncmp(argv[2], "loop", 4)) {
			int count = 100, err = 0;
			uint8_t orig;

			if (argv[2][4] == ':')
				count = simple_strtol(&argv[2][5], NULL, 10);

			printf("looping on address 0x%04x for %d cycles...\n",
			       addr, count);

			ret = lpc_io_read(addr);

			if (ret < 0) {
				printf("Error reading address %04x: %d\n",
				       addr, -ret);
				return 1;
			}

			orig = ret;

			for (i = 1; i < count; i++) {
				ret = lpc_io_read(addr);

				if (ret < 0) {
					printf
					    ("Error reading address %04x at cycle %d: %d\n",
					     addr, i, -ret);
					return 1;
				}
				if ((uint8_t) ret != orig)
					err++;
			}

			printf("Errors: %d/%d\n", err, count - 1);

			return 0;
		} else if (len == 1) {
			len = simple_strtol(argv[2], NULL, 16);

			if (len == 0)
				len = 1;
			else if (len > 0x100)
				len = 0x100;
		}
	}

	for (i = 0; i < len; i++, addr++) {
		if ((i % 8) == 0)
			printf("\n%04x:  ", addr);

		ret = lpc_io_read(addr);

		if (ret < 0) {
			printf("\nError reading address %04x: %d\n", addr,
			       -ret);
			return 1;
		}

		val = (uint8_t) ret;

		printf("%02x ", val);

	}
	printf("\n");

	return CMD_RET_SUCCESS;
}

int do_lpc_wr(cmd_tbl_t * cmdtp, int flag, int argc, char *const argv[])
{
	uint16_t addr, val;
	int ret;

	if (argc != 3)
		return CMD_RET_USAGE;

	if (lpc_base == 0) {
		printf("Error: LPC Base not initialized!\n");
		return 1;
	}

	addr = simple_strtol(argv[1], NULL, 16);
	val = simple_strtol(argv[2], NULL, 16);
	ret = lpc_io_write(addr, val);

	if (ret < 0) {
		printf("Error writing address %04x: %d\n", addr, -ret);
		return CMD_RET_FAILURE;
	}

	printf("[%04x] <= 0x%02x\n", addr, val);

	return CMD_RET_SUCCESS;
}

int do_lpc_init(cmd_tbl_t * cmdtp, int flag, int argc, char *const argv[])
{
	lpc_io_init();
	return 0;
}

U_BOOT_CMD(lpc_rd, 3, 1, do_lpc_rd,
	   "lpc_rd 8 bit word from LPC",
	   "\n    - read 8 bit from LPC\n"
	   "lpc_rd  addr [len] \n    - read 8 bit  from LPC\n"
	   "lpc_rd  start:end \n    - read range from LPC\n"
	   "lpc_rd addr loop:count \n    - read the same address repeatedly");

U_BOOT_CMD(lpc_wr, 3, 1, do_lpc_wr,
	   "lpc_wr 8 bit word from LPC",
	   "\n    - write  8 bit from LPC\n"
	   "lpc_wr  addr data \n    - write 8 bit  from LPC\n");

U_BOOT_CMD(lpc_init, 1, 0, do_lpc_init,
	   "lpc_init", "\n    - initialize lpc interface\n" "lpc_init\n");

#ifdef CONFIG_OCTEON_LPC_UART
int early_uart_init_f(void)
{
	int bootbus_node_offset, lpc_node_offset, uart_node_offset;
	int rc, tmp;
	uint64_t tmr_freq = 0x01fca055;  // 33,333,333Hz

	char str[64];

	sprintf(str, "Setting up GPIO as LPC clock (n=%lld)\r\n", tmr_freq);
	cvmx_uart_write_string(0, str);

	cvmx_gpio_set_freq(0, 0, tmr_freq); // set GPIO timer0 frequency
	cvmx_gpio_cfg_sel(0, 5, 0x10);	//set GPIO mode to CLK_GEN0
	cvmx_gpio_cfg(5, 1);	//set GPIO as output

	cvmx_uart_write_string(0, "Setting up LPC boot bus\r\n");
	rc = lpc_io_init();

	if (rc) {
		char err_str[64];
		sprintf(err_str, "Boot bus initialization failed: %d\r\n", rc);
		cvmx_uart_write_string(0, err_str);
	} else {
		char stat_str[64];
		sprintf(stat_str, "Boot Bus OK: 0x%llx\r\n", lpc_base);
		cvmx_uart_write_string(0, stat_str);
	}

	/* Initialize the serial port early */
	gd->baudrate = CONFIG_BAUDRATE;

	cvmx_uart_write_string(0, "Initializing W83627\r\n");

	rc = w83627_init();

	if (rc == 0) {
		cvmx_uart_write_string(0, "W83627 init done\r\n");
		gd->arch.lpc_uart_addr = lpc_base + W83627_UART_A_BASE_DEFAULT;
		/* initialize to 115200 baud */
		NS16550_init(gd->arch.lpc_uart_addr, 8);
		cvmx_uart_write_string(0, "NS16550 init done\r\n");
	} else {
		cvmx_uart_write_string(0, "W83627 not present\r\n");
		gd->arch.lpc_uart_addr = 0;
	}
#ifdef CONFIG_OCTEON_DEFAULT_CONSOLE_UART_PORT
	gd->arch.console_uart = CONFIG_OCTEON_DEFAULT_CONSOLE_UART_PORT;
#endif
	/* Initialize the serial port early */
	octeon_uart_setup(gd->arch.console_uart);

	return 0;
}
#endif
