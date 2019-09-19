/***********************license start************************************
 * Copyright (c) 2011  Cavium Inc. (support@cavium.com).
 * All rights reserved.
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
 * AND WITH ALL FAULTS AND CAVIUM INC. MAKES NO PROMISES, REPRESENTATIONS
 * OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH
 * RESPECT TO THE SOFTWARE, INCLUDING ITS CONDITION, ITS CONFORMITY TO ANY
 * REPRESENTATION OR DESCRIPTION, OR THE EXISTENCE OF ANY LATENT OR PATENT
 * DEFECTS, AND CAVIUM SPECIFICALLY DISCLAIMS ALL IMPLIED (IF ANY) WARRANTIES
 * OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR
 * PURPOSE, LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET
 * POSSESSION OR CORRESPONDENCE TO DESCRIPTION.  THE ENTIRE RISK ARISING OUT
 * OF USE OR PERFORMANCE OF THE SOFTWARE LIES WITH YOU.
 *
 ***********************license end**************************************/

#include <common.h>
#include <command.h>
#include <asm/arch/cvmx.h>
#include <ata.h>
#include <part.h>
#include <fat.h>
#include <../disk/part_dos.h>
#include <asm/arch/cvmx-bootloader.h>
#include <asm/gpio.h>
#include <asm/arch/lib_octeon.h>
#ifdef CONFIG_OCTEON_SPI_STAGE2
# include <spi_flash.h>
#endif
#include <linux/ctype.h>
#include <environment.h>

DECLARE_GLOBAL_DATA_PTR;

extern unsigned long do_go_exec(ulong (*entry)(int, char * const []), int argc,
				char * const argv[]);

#ifdef CONFIG_OCTEON_SPI_STAGE2
# ifndef CONFIG_SF_DEFAULT_SPEED
#  define CONFIG_SF_DEFAULT_SPEED	1000000
# endif
# ifndef CONFIG_SF_DEFAULT_MODE
#  define CONFIG_SF_DEFAULT_MODE	SPI_MODE_3
# endif
# ifndef CONFIG_SF_DEFAULT_CS
#  define CONFIG_SF_DEFAULT_CS		0
# endif
# ifndef CONFIG_SF_DEFAULT_BUS
#  define CONFIG_SF_DEFAULT_BUS		0
# endif
#endif

#ifdef CONFIG_OCTEON_EMMC_STAGE2
/* Fix issue #4616, if env variable dram_size_mbytes is not set - stage 3 try to
 * reinit DRAM and crash. */
void fix_eeprom_dram_size(void)
{
    env_t3 env_new;
    int env_leng;
    int rcode;
    int i,j;
    int dram_str_size;
    int position;
    int dram_size;
    int dram_size_env;
    int position_end;
    int new_ddr_length;
    char *buf;
    char *dram_str;
    char new_ddr_size_value[100];
    char temp_buf[4*1024];

    env_leng = 0;
    buf = (char *)&env_new.data;
    eeprom_init();
    rcode = eeprom_read(CONFIG_SYS_DEF_EEPROM_ADDR, 0, (uchar *)&env_new, CONFIG_ENV_SIZE_S3);

    for ( j = 0; j < ENV_SIZE_S3; j++){
        if( buf[j] == 0x00
           && buf[j+1] == 0x00
           && buf[j+2] == 0x00){
            env_leng = j+1;
            break;
        }
    }
    position = 0;
    dram_str = "dram_size_mbytes";
    dram_str_size = strlen(dram_str);

    for(j = 0; j < env_leng; j++){
        for(i = 0; i < dram_str_size-1; i++){
            if(buf[j+i] == dram_str[i]){
                if( i >= dram_str_size -2 ){
                    position = j;
                    break;
                }
            }
            else{
                break;
            }
        }
        if(position > 0){
            break;
        }
    }
    position_end = env_leng;
    dram_size = gd->arch.ram_sizes[0];
    dram_size = abs(dram_size);
    sprintf(new_ddr_size_value,"dram_size_mbytes=%d", dram_size);
    new_ddr_length = strlen(new_ddr_size_value)+1;

    if(position > 0){
        dram_size_env = simple_strtoul(&buf[position+dram_str_size+1], NULL, 10);
        if(dram_size_env == dram_size){
            return;
        }
        for(i = position; i < env_leng; i++){
            if(buf[i] == ';' || buf[i] == 0x00 ){
                position_end = i;
                break;
            }
        }
        memcpy(&temp_buf[0], &buf[position_end+1], env_leng-position_end);
        memcpy(&buf[position], new_ddr_size_value, new_ddr_length);
        memcpy(&buf[position+new_ddr_length ], &temp_buf[0], env_leng-position_end);
    }
    else{
        memcpy(&buf[env_leng], new_ddr_size_value, new_ddr_length);
    }
    env_new.crc = crc32(0, (unsigned char const *)&env_new.data, ENV_SIZE_S3);
    rcode = eeprom_write (CONFIG_SYS_DEF_EEPROM_ADDR, CONFIG_ENV_OFFSET, (uchar *)&env_new, CONFIG_ENV_SIZE_S3);
}

/**
 * Search for a bootable FAT partition
 *
 * @param dev_desc	Block descriptor to search partition table of
 *
 * @return	-1 on error, otherwise the primary FAT bootable partition number
 *		(0 based)
 */
static int find_bootable_fat_partition(block_dev_desc_t *dev_desc)
{
	dos_partition_t *pt;
	int i;

	ALLOC_CACHE_ALIGN_BUFFER(uint8_t, buffer, dev_desc->blksz);

	if ((dev_desc->block_read(dev_desc->dev, 0, 1, (ulong *)buffer) != 1) ||
	    (buffer[DOS_PART_MAGIC_OFFSET + 0] != 0x55) ||
	    (buffer[DOS_PART_MAGIC_OFFSET + 1] != 0xaa) ) {
		printf("Could not read DOS partition table\n");
		return -1;
	}

	pt = (dos_partition_t *)(buffer + DOS_PART_TBL_OFFSET);

	for (i = 0; i < 4; i++) {
		if (pt->boot_ind != 0x80)
			continue;
		switch (pt->sys_ind) {
		case 0x4:		/* FAT16 < 32M */
		case 0x6:		/* FAT16 */
		case 0x14:		/* FAT16 < 32M */
		case 0x16:		/* FAT16 */
		case 0xb:		/* FAT32 */
		case 0xc:		/* FAT32 */
		case 0xe:		/* LBA FAT16 */
		case 0x1b:		/* FAT32 */
		case 0x1c:		/* LBA FAT32 */
		case 0x1e:		/* LBA FAT16 */
			return i+1;
		default:
			break;
		}
	}
	return -1;
}

int do_octbootstage3(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	uint32_t addr;
	const char *filename;
	const char *failsafe_filename;
	const char *dev_name;
#ifdef CONFIG_ENV_IS_NOWHERE
	char *s;
	char str_temp[255];
	char *s_tmp = &str_temp[0];
#endif
	int failsafe;
	int size = 0;
	int max_size = 0;
	int part_no;
	int dev_no = 0;
	char str_buf[64];
	block_dev_desc_t *dev_desc = NULL;

	fix_eeprom_dram_size();

#ifdef CONFIG_ENV_IS_NOWHERE
	strcpy(s_tmp, cvmx_board_type_to_string(cvmx_sysinfo_get()->board_type));
	while (*s_tmp) {
		*s_tmp = tolower(*s_tmp);
		s_tmp++;
	}
	s_tmp = &str_temp[0];
	if( strstr(s_tmp, "cust_") !=  NULL){
		snprintf(str_buf, sizeof(str_buf), "u-boot-octeon_%s.bin", s_tmp+5);
	}else{
		snprintf(str_buf, sizeof(str_buf), "u-boot-octeon_%s.bin", s_tmp);
	}
	filename = str_buf;
	addr = CONFIG_OCTEON_STAGE3_LOAD_ADDR;
	dev_no = CONFIG_OCTEON_STAGE3_DEVNO;
	dev_name = NULL;
#else
	filename = getenv("octeon_stage3_bootloader");
	if (!filename) {
		const char *boardname;
		boardname = getenv("boardname");
		if (!boardname) {
			printf("Undefined stage 3 filename\n");
			return CMD_RET_FAILURE;
		}
		snprintf(str_buf, sizeof(str_buf), "u-boot-octeon_%s.bin",
			 boardname);
		filename = str_buf;
	}

	addr = getenv_ulong("octeon_stage3_load_addr", 16,
			    CONFIG_OCTEON_STAGE3_LOAD_ADDR);
	max_size = getenv_ulong("octeon_stage3_max_size", 0,
				CONFIG_OCTEON_STAGE3_MAX_SIZE);
	dev_no = getenv_ulong("octeon_stage3_devno", 0,
			   CONFIG_OCTEON_STAGE3_DEVNO);
	dev_name = getenv("octeon_stage3_devname");
#endif
	if (!dev_name)
		dev_name = CONFIG_OCTEON_STAGE3_DEVNAME;

	dev_desc = get_dev(dev_name, dev_no);
	if (!dev_desc) {
		printf("Could not find device %s %d\n", dev_name, dev_no);
		return -1;
	}

	part_no = find_bootable_fat_partition(dev_desc);
	if (part_no < 0) {
		printf("No bootable FAT partition found\n");
		return -1;
	}

	if (fat_register_device(dev_desc, part_no) != 0) {
		printf("Unable to use %s %d:%d for FAT partition\n", dev_name,
		       dev_no, part_no);
	}

#ifndef CONFIG_OCTEON_NO_STAGE3_FAILSAFE
	gpio_direction_input(CONFIG_OCTEON_FAILSAFE_GPIO);
	failsafe = gpio_get_value(CONFIG_OCTEON_FAILSAFE_GPIO);

	if (!filename)
		failsafe = 1;

	if (!failsafe) {
		size = file_fat_read(filename, (unsigned char *)addr, max_size);
		if (size <= 0) {
			printf("Could not read %s, trying failsafe\n", filename);
			goto failsafe;
		}
		do_go_exec((void *)addr, argc - 1, argv + 1);
	}

failsafe:
	failsafe_filename = getenv("octeon_stage3_failsafe_bootloader");
	if (!failsafe_filename) {
		failsafe_filename = CONFIG_OCTEON_STAGE3_FAILSAFE_FILENAME;
		printf("Error: environment variable octeon_stage3_failsafe_bootloader is not set.\n");
		return 0;
	}
	if (failsafe_filename) {
		size = file_fat_read(failsafe_filename, (unsigned char *)addr,
				     max_size);
		if (size <= 0) {
			printf("Could not read failsafe bootloader %s, "
			       "trying %s\n", failsafe_filename,
			       CONFIG_OCTEON_STAGE3_FAILSAFE_FILENAME);
			failsafe_filename = CONFIG_OCTEON_STAGE3_FAILSAFE_FILENAME;
			return -1;
		}
		do_go_exec((void *)addr, argc -1, argv +1);
	} else {
		printf("No failsafe available!\n");
		return -1;
	}
#else
	if (!filename)
		filename = CONFIG_OCTEON_STAGE3_FILENAME;

	size = file_fat_read(filename, (unsigned char *)addr, max_size);
	if (size <= 0) {
		printf("Could not open stage 3 bootloader %s\n", filename);
		return -1;
	}
	do_go_exec((void *)addr, argc - 1, argv + 1);
#endif
	return -1;
}

#elif defined(CONFIG_OCTEON_SPI_STAGE2)
/**
 * This will search for one or more bootloaders in the SPI NOR and boot
 * either the failsafe one or the last valid one found.
 *
 * @param cmdtp		Command data structure
 * @param flag		flags, not used
 * @param argc		argument count, not used, passed on
 * @param argv		arguments, not used, passed on
 *
 * @return		-1 on error otherwise no return.
 */
int do_octbootstage3(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	struct spi_flash *flash;
	struct bootloader_header *header;
	uint32_t addr;
	const int bus = 0;
	const int cs = 0;
	const int speed = CONFIG_SF_DEFAULT_SPEED;
	const int mode = CONFIG_SF_DEFAULT_MODE;
	void *buf;
	int len;
	int offset;
	int found_offset = -1;
	int found_size = 0;
	int rc;
	bool failsafe;

	flash = spi_flash_probe(bus, cs, speed, mode);
	if (!flash) {
		printf("Failed to initialize SPI flash at %u:%u", bus, cs);
		return -1;
	}
	addr = getenv_ulong("octeon_stage3_load_addr", 16,
			    CONFIG_OCTEON_STAGE3_LOAD_ADDR);

	header = CASTPTR(struct bootloader_header, addr);
	buf = (void *)header;
#if defined(CONFIG_OCTEON_FAILSAFE_GPIO) && \
    !defined(CONFIG_OCTEON_NO_STAGE3_FAILSAFE)
	gpio_direction_input(CONFIG_OCTEON_FAILSAFE_GPIO);
	udelay(100);	/* Give time to settle */
	failsafe = !!gpio_get_value(CONFIG_OCTEON_FAILSAFE_GPIO);
	debug("Failsafe bootloader is %sabled\n", failsafe ? "en" : "dis");
#else
	failsafe = false;
#endif

	offset = CONFIG_OCTEON_SPI_BOOT_START;
	do {
		debug("%s: Looking for bootloader at SPI offset 0x%x\n",
		      __func__, offset);
		if (spi_flash_read(flash, offset, sizeof(*header), header)) {
			printf("Could not read SPI flash to find bootloader\n");
			return -1;
		}

		if (!validate_bootloader_header(header) ||
		    (header->board_type != gd->arch.board_desc.board_type)) {
			offset += flash->erase_size;
			debug("No valid header found, continuing at offset 0x%x, header board type: %u, expected type: %u\n",
			      offset,
			      header->board_type,
			      gd->arch.board_desc.board_type);
			continue;
		}

		len = header->hlen + header->dlen - sizeof(*header);
		if (len < 0) {
			printf("Invalid length calculated, hlen: %d, dlen: %d\n",
			       header->hlen, header->dlen);
			offset += flash->erase_size;
			continue;
		}
		debug("Reading rest of bootloader, %u bytes starting at offset 0x%x\n",
		      len, offset + sizeof(*header));
		/* Read rest of bootloader */
		rc = spi_flash_read(flash, offset + sizeof(*header), len,
				    &header[1]);
		if (rc) {
			printf("Could not read %d bytes from SPI flash\n",
			       header->dlen + header->hlen);
			return -1;
		}
		if (calculate_image_crc(header) != header->dcrc) {
			printf("Found corrupted image at offset 0x%x, continuing search\n",
			       offset);
			offset += flash->erase_size;
			continue;
		}
		found_offset = offset;
		found_size = header->hlen + header->dlen;
		if (header->image_type == BL_HEADER_IMAGE_STAGE2) {
			debug("Found stage 2 bootloader image, skipping\n");
			offset += (found_size + flash->erase_size - 1) &
						~(flash->erase_size - 1);
			continue;
		}
		printf("Found valid %sSPI bootloader at offset: 0x%x, size: %d bytes\n",
		       failsafe ? "failsafe " : "", found_offset, found_size);
		if (failsafe) {
			debug("Stopping search, boot failsafe\n");
			break;
		}
		/* Skip past the current image to the next one */

		offset += (found_size + flash->erase_size - 1) &
							~(flash->erase_size - 1);
		debug("Start searching for next image at offset 0x%x, found_size: 0x%x, erase size: 0x%x\n",
		      offset, found_size, flash->erase_size);
	} while (offset < CONFIG_OCTEON_SPI_BOOT_END);

	if (found_offset < 0) {
		printf("Could not find stage 3 bootloader\n");
		return -1;
	}

	/* If we searched for multiple bootloaders and didn't stop at the first
	 * one (i.e. failsafe) then re-read the last good one found into
	 * memory.
	 */
	if (found_offset != offset) {
		debug("Last found offset: 0x%x, offset: 0x%x\n",
		      found_offset, offset);
		printf("Loading %sbootloader from SPI offset 0x%x, size: %u bytes\n",
		       failsafe ? "failsafe " : "", found_offset, found_size);
		rc = spi_flash_read(flash, found_offset, found_size, header);
		if (rc) {
			printf("Error reading bootloader from offset 0x%x, size: 0x%x\n",
			       found_offset, found_size);
			return -1;
		}
	}

	do_go_exec(buf, argc - 1, argv + 1);

	return 0;
}
#endif

U_BOOT_CMD(bootstage3, 1, 0, do_octbootstage3,
	   "Load and execute the stage 3 bootloader",
	   "Load and execute the stage 3 bootloader");
