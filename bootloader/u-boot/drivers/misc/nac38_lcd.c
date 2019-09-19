#include <mtd/cfi_flash.h>
#include <common.h>
#include <command.h>
#include <pci.h>
#include <asm/arch/octeon_boot_bus.h>

#define CONFIG_LCD_INIT_DELAY		10000
#define CONFIG_LCD_DISPLAY_DELAY	100

#define CLR_LCD_RS	0x01
#define SET_LCD_RS	0x00
#define CLR_LCD_RW	0x10
#define SET_LCD_RW	0x00

#define LCDM_KEY_LEFT	0x80
#define LCDM_KEY_RIGHT	0x02
#define LCDM_KEY_UP	0x01
#define LCDM_KEY_DOWN	0x10
#define LCDM_KEY_MID	0x04



/*
LCD module "chip select" is  BOOT_CE#7
A0 : 0 =  DATA REGISTER, 1 =  INSTRUCTION REGISTER
A4 : 0 = read, 1 = write
other  address : don't care

ex : 	address 0x00 : read  DATA REGISTER
	address 0x01 : read  INSTRUCTION REGISTER
	address 0x10 : write  DATA REGISTER
	address 0x11 : write  INSTRUCTION REGISTER

============= transfer to boot bus  =====================
time unit : ns  , buffer 10 ns,  assume Tr,Tf  << 1ns
		2.7~4.5V	4.5~5V
   Tadr		230		110
   Tce		70		50
   Toe		460		230
   Trh	 	30		20
   Twe		460		230
   Twh	 	30		20
   Tpause	230		110

============= transfer to boot bus  =====================
   Tadr	= (Tc - max( Twe, Toe) - max( Trh, Twh) - Tce)/2
   Tce	= Tasu
   Toe	= max( Tdrsh, Tw)
   Trh	= max(Tahd,Tdrhd)
   Twe	= max( Tdwsh, Tw)
   Twh	= max(Tahd,Tdwhd)
   Tpause= (Tc - max( Twe, Toe) - max( Trh, Twh) - Tce)/2

============= SAMSUNG S6A0069  =====================
LCD spec.			2.7~4.5V	4.5~5V
Tc : E cycle 			>1000		>500
Tr,Tf :   E rise/fall time		<25		< 20
Tw : E pulse width		>450		>230
Tasu : R/W RS setup time	>60		>40
Tahd : R/W RS hold time		>20		>10
Tdrsh : DATA read delay time	<360		<120
Tdrhd : DATA read hold time	>5		>5
Tdwsh : DATA write setup time	>195		>80
Tdwhd : DATA write hold time	>10		>10

*/

static uint64_t base_addr;

void InitLCD(void)
{
	int command;

	if (octeon_boot_bus_get_dev_info("/soc/bootbus/lcd-display",
					 "samsung,s6a0069", &base_addr, NULL)) {
		printf("Could not find LCD display in FDT\n");
		return;
	}

	command = CLR_LCD_RS | CLR_LCD_RW;
}
