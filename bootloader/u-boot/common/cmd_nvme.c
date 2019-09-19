/*
 * Copyright (C) 2017 NXP Semiconductors
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <command.h>
#include <part.h>
#include <nvme.h>
#include <asm/errno.h>

static int nvme_curr_device = -1;
block_dev_desc_t nvme_dev_desc[CONFIG_SYS_NVME_MAX_DEVICE];

int nvme_scan_devices(void) __attribute__((weak, alias("__nvme_scan_devices")));
int __nvme_scan_devices(void)
{
	int i, rc;
	for (i = 0; i < CONFIG_SYS_NVME_MAX_DEVICE; i++) {
		if (nvme_dev_desc[i].type == DEV_TYPE_HARDDISK) {
			rc = nvme_blk_probe(i);
			if (!rc && (nvme_dev_desc[i].lba > 0))
				init_part(&nvme_dev_desc[i]);
		}
	}
	return 0;
}

int __nvme_initialize(void)
{
	int rc;
	int i;
	bool scan_device[CONFIG_SYS_NVME_MAX_DEVICE];
	bool found = false;

	memset(scan_device, 0, sizeof(scan_device));
	for (i = 0; i < CONFIG_SYS_NVME_MAX_DEVICE; i++) {
		memset(&nvme_dev_desc[i], 0, sizeof(struct block_dev_desc));
		nvme_dev_desc[i].if_type = IF_TYPE_NVME;
		nvme_dev_desc[i].dev = i;
		nvme_dev_desc[i].part_type = PART_TYPE_UNKNOWN;
		nvme_dev_desc[i].type = DEV_TYPE_UNKNOWN;
		nvme_dev_desc[i].lba = 0;
		nvme_dev_desc[i].blksz = 512;
		nvme_dev_desc[i].log2blksz = LOG2(nvme_dev_desc[i].blksz);
		nvme_dev_desc[i].block_read = nvme_read;
		nvme_dev_desc[i].block_write = nvme_write;

		rc = init_nvme(i);
		if (!rc) {
			scan_device[i] = true;
			found = true;
		}
	}

	nvme_scan_devices();
	nvme_curr_device = 0;
	return !found;
}
int nvme_initialize(void) __attribute__((weak, alias("__nvme_initialize")));

#ifdef CONFIG_PARTITIONS
block_dev_desc_t *nvme_get_dev(int dev)
{
	debug("%s(%d)\n", __func__, dev);
	return (dev < CONFIG_SYS_NVME_MAX_DEVICE) ? &nvme_dev_desc[dev] : NULL;
}
#endif

static int do_nvme_list(cmd_tbl_t *cmdtp, int flag,
		int argc, char * const argv[])
{
	int i;
	block_dev_desc_t *dev;
	for (i = 0; i < CONFIG_SYS_NVME_MAX_DEVICE; i++) {
		dev = nvme_get_dev(i);
		if (dev->type != DEV_TYPE_UNKNOWN &&
		    dev->if_type == IF_TYPE_NVME) {
			printf("NVME %d: ", i);
			dev_print(dev);
		}
	}

	return CMD_RET_SUCCESS;
}

static int do_nvme_info(cmd_tbl_t *cmdtp, int flag,
		int argc, char * const argv[])
{
	int devnum;
	if (argc > 1)
		devnum = (int)simple_strtoul(argv[1], NULL, 10);
	else
		devnum = nvme_curr_device;
	nvme_print_info(devnum);
	return CMD_RET_SUCCESS;
}

static int blk_show_device(int if_type, int devnum)
{
	block_dev_desc_t *desc;

	desc = nvme_get_dev(devnum);
	if (desc)
		dev_print(desc);

	if (desc->type == DEV_TYPE_UNKNOWN)
		return -ENOENT;
	return 0;
}

static int blk_print_part_devnum(int if_type, int devnum)
{
	block_dev_desc_t *desc;

	desc = nvme_get_dev(devnum);
	if (!desc)
		return -ENOSYS;

	if (desc->type == DEV_TYPE_UNKNOWN)
		return -ENOENT;

	print_part(desc);

	return 0;
}

static int do_nvme_device(cmd_tbl_t *cmdtp, int flag,
		int argc, char * const argv[])
{
	if (argc > 1) {
		int devnum = (int)simple_strtoul(argv[1], NULL, 10);
		if (!blk_show_device(IF_TYPE_NVME, devnum)) {
			nvme_curr_device = devnum;
			printf("... is now current device\n");
		} else {
			return CMD_RET_FAILURE;
		}
	} else {
		blk_show_device(IF_TYPE_NVME, nvme_curr_device);
	}

	return CMD_RET_SUCCESS;
}

static int do_nvme_part(cmd_tbl_t *cmdtp, int flag,
		int argc, char * const argv[])
{
	if (argc > 1) {
		int devnum = (int)simple_strtoul(argv[2], NULL, 10);
		if (blk_print_part_devnum(IF_TYPE_NVME, devnum)) {
			printf("\nNVME device %d not available\n", devnum);
			return CMD_RET_FAILURE;
		}
	} else {
		blk_print_part_devnum(IF_TYPE_NVME, nvme_curr_device);
	}

	return CMD_RET_SUCCESS;
}

static int do_nvme_read(cmd_tbl_t *cmdtp, int flag, int argc,
		char * const argv[])
{
	unsigned long time;
	ulong addr;
	ulong cnt;
	ulong n;
	lbaint_t blk;

	if (argc != 4)
		return CMD_RET_USAGE;

	addr = simple_strtoul(argv[1], NULL, 16);
	cnt = simple_strtoul(argv[3], NULL, 16);
	blk = simple_strtoull(argv[2], NULL, 16);

	printf("\nNVME read: device %d block # %lld, count %ld ... ",
	       nvme_curr_device, (long long)blk, cnt);

	time = get_timer(0);
	n = nvme_read(nvme_curr_device, blk, cnt, (ulong *)addr);
	time = get_timer(time);

	printf("read: %s\n", (n == cnt) ? "OK" : "ERROR");
	printf("%lu bytes read in %lu ms", cnt * 512, time);
	if (time > 0) {
		puts(" (");
		print_size(((cnt * 512)/ time) * 1000, "/s");
		puts(")");
	}
	puts("\n");
	return (n == cnt) ? 0 : 1;
}

static int do_nvme_write(cmd_tbl_t *cmdtp, int flag, int argc,
		char * const argv[])
{
	unsigned long time;
	ulong addr;
	ulong cnt;
	ulong n;
	lbaint_t blk;
	if (argc != 4)
		return CMD_RET_USAGE;

	addr = simple_strtoul(argv[1], NULL, 16);
	cnt = simple_strtoul(argv[3], NULL, 16);
	blk = simple_strtoull(argv[2], NULL, 16);

	printf("\nNVME write: device %d block # %lld, count %ld ... ",
	       nvme_curr_device, (long long)blk, cnt);

	time = get_timer(0);
	n = nvme_write(nvme_curr_device, blk, cnt, (ulong *)addr);
	time = get_timer(time);

	printf("write: %s\n", (n == cnt) ? "OK" : "ERROR");
	printf("%lu bytes read in %lu ms", cnt * 512, time);
	if (time > 0) {
		puts(" (");
		print_size(((cnt * 512) / time) * 1000, "/s");
		puts(")");
	}
	puts("\n");
	return (n == cnt) ? 0 : 1;
}

static cmd_tbl_t cmd_nvme[] = {
	U_BOOT_CMD_MKENT(list, 1, 1, do_nvme_list, "", ""),
	U_BOOT_CMD_MKENT(info, 2, 1, do_nvme_info, "", ""),
	U_BOOT_CMD_MKENT(device, 2, 1, do_nvme_device, "", ""),
	U_BOOT_CMD_MKENT(part, 2, 1, do_nvme_part, "", ""),
	U_BOOT_CMD_MKENT(write, 4, 0, do_nvme_write, "", ""),
	U_BOOT_CMD_MKENT(read, 4, 0, do_nvme_read, "", "")
};

static int do_nvmecops(cmd_tbl_t *cmdtp, int flag, int argc,
		char * const argv[])
{
	cmd_tbl_t *cp;

	cp = find_cmd_tbl(argv[1], cmd_nvme, ARRAY_SIZE(cmd_nvme));

	argc--;
	argv++;

	if (cp == NULL || argc > cp->maxargs)
		return CMD_RET_USAGE;

	if (flag == CMD_FLAG_REPEAT && !cp->repeatable)
		return CMD_RET_SUCCESS;

	if (nvme_curr_device < 0) {
		nvme_curr_device = nvme_initialize();
		if (nvme_curr_device < 0) {
			puts("No nvme device available\n");
			return CMD_RET_FAILURE;
		}
	}
	return cp->cmd(cmdtp, flag, argc, argv);
}

U_BOOT_CMD(
	nvme, 8, 1, do_nvmecops,
	"NVME sub system",
	"\nnvme list - show all available NVME blk devices\n"
	"nvme info [dev]- show current or a specific NVME blk device\n"
	"nvme device [dev] - show or set current device\n"
	"nvme part [dev] - print partition table\n"
	"nvme read  addr blk# cnt\n"
	"nvme write addr blk# cnt"
);
