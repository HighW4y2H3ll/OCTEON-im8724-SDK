#include <common.h>
#include <niagara.h>
#include <malloc.h>
#include "im8724_cpld.h"
#include "niagara.h"

static int im8724_cpld_mdio_wait()
{
	int timeout = 1000000;

	while( (cpld_rd(CPLD_ADDR_MI_CSR) & 0x1) && timeout--)
	{
		udelay(1);
	}

	if(!timeout)
	{
		printf("Timeout!\n");
		return 1;
	}

	return 0;
}

int im8724_cpld_mdio_config(uint16_t bus, uint8_t mode)
{
	if(bus & 0x8000)
	{
		cpld_wr_remote((bus >> 8) & 0x1f, CPLD_ADDR_MI_BSL, bus & 0xff);
		cpld_wr(CPLD_ADDR_BA_CTL, 0xE0 | ((bus >> 8) & 0x1f) );
	}
	else
		cpld_wr(CPLD_ADDR_MI_BSL, bus);

	cpld_wr(CPLD_ADDR_MI_MCTL, (mode ? 0x80 : 0x00) | 0x30);

	return 0;
}

static int im8724_cpld_mdio_idle(int cycles)
{
	uint8_t mctl = cpld_rd(CPLD_ADDR_MI_MCTL);
	cpld_wr(CPLD_ADDR_MI_MCTL, 0x00);

	while(cycles--)
	{
		cpld_wr(CPLD_ADDR_MI_MCTL, 0x04);
		udelay(1);
		cpld_wr(CPLD_ADDR_MI_MCTL, 0x00);
		udelay(1);
	}

	cpld_wr(CPLD_ADDR_MI_MCTL, mctl);
}

int im8724_cpld_mdio45_write(uint8_t port_ad, uint8_t dev_ad, uint16_t reg_ad, uint16_t data)
{
	cpld_wr(CPLD_ADDR_MI_DEVAD, dev_ad);
	cpld_wr(CPLD_ADDR_MI_PRTAD, port_ad);

	cpld_wr(CPLD_ADDR_MI_AD_0, reg_ad & 0xff);
	cpld_wr(CPLD_ADDR_MI_AD_1, reg_ad >> 8);

	cpld_wr(CPLD_ADDR_MI_CSR, 0x01);	//write address operation

	if(im8724_cpld_mdio_wait())
		return 1;

	im8724_cpld_mdio_idle(8);

	cpld_wr(CPLD_ADDR_MI_AD_0, data & 0xff);
	cpld_wr(CPLD_ADDR_MI_AD_1, data >> 8);

	cpld_wr(CPLD_ADDR_MI_CSR, 0x11);	//write data operation

	//im8724_cpld_mdio_idle(8);

	if(im8724_cpld_mdio_wait())
		return 1;

	return 0;
}

int im8724_cpld_mdio45_read(uint8_t port_ad, uint8_t dev_ad, uint16_t reg_ad, int count, uint16_t* data)
{
	int i=0;

	cpld_wr(CPLD_ADDR_MI_DEVAD, dev_ad);
	cpld_wr(CPLD_ADDR_MI_PRTAD, port_ad);

	cpld_wr(CPLD_ADDR_MI_AD_0, reg_ad & 0xff);
	cpld_wr(CPLD_ADDR_MI_AD_1, reg_ad >> 8);

	cpld_wr(CPLD_ADDR_MI_CSR, 0x01);	//write address operation

	if(im8724_cpld_mdio_wait())
		return 1;

	im8724_cpld_mdio_idle(8);

	for(i=0; i<count; i++)
	{
		cpld_wr(CPLD_ADDR_MI_CSR, 0x21);	//read/inc operation

		if(im8724_cpld_mdio_wait())
			return 1;

		data[i] = cpld_rd(CPLD_ADDR_MI_AD_0) | ( cpld_rd(CPLD_ADDR_MI_AD_1) << 8 );

		im8724_cpld_mdio_idle(8);
	}

	return 0;
}

int do_cpld_mdio45(cmd_tbl_t * cmdtp, int flag, int argc, char *const argv[])
{
	uint8_t dev_ad, port_ad;
	uint16_t reg_ad, cpld_ad;

	if(argc < 6) goto usage;


	cpld_ad = simple_strtol(argv[2], NULL, 16);
	port_ad = simple_strtol(argv[3], NULL, 16);
	dev_ad  = simple_strtol(argv[4], NULL, 16);
	reg_ad  = simple_strtol(argv[5], NULL, 16);

	im8724_cpld_mdio_config(cpld_ad, CPLD_MDIO_CLAUSE45);

	if( argv[1][0] == 'r')
	{
		int count = 1, i;

		uint16_t* data;

		if(argc == 7) count = simple_strtol(argv[6], NULL, 0);

		data = malloc(count * sizeof(uint16_t));

		im8724_cpld_mdio45_read(port_ad, dev_ad, reg_ad, count, data);

		for(i=0; i<count; i++)
		{
			printf("[%02x:%02x:%04x] == 0x%04x\n", port_ad, dev_ad, reg_ad + i, data[i]);
		}
	}
	else if( argv[1][0] == 'w')
	{
		uint16_t data;

		if(argc != 7) goto usage;

		data = simple_strtol(argv[6], NULL, 16);

		im8724_cpld_mdio45_write(port_ad, dev_ad, reg_ad, data);

		printf("[%02x:%02x:%04x] <= 0x%04x\n", port_ad, dev_ad, reg_ad, data);
	}
	else
		goto usage;

	im8724_cpld_mdio_config(cpld_ad & ~0xff, CPLD_MDIO_CLAUSE45);

	return 0;

usage:
	printf("Usage:\n\t%s %s\n", argv[0], cmdtp->usage);
	return 1;

}

U_BOOT_CMD(cpld_mdio45, 7, 1, do_cpld_mdio45,
		"<r|w> <cpld_addr> <port_ad> <dev_ad> <reg_ad> <<data>|[count]>",
	  "Perform MDIO clause 45 transaction through CPLD");
