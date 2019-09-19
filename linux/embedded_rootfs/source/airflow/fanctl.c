#include <stdio.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(_a) (sizeof(_a)/sizeof(_a[0]))
#endif

#define PWM_REG_BASE 0x32

static int set_slave_addr(int i2c, int address, int force)
{
    /* With force, let the user read from/write to the registers
       even when a driver is also running */
    if (ioctl(i2c, force ? I2C_SLAVE_FORCE : I2C_SLAVE, address) < 0)
    {
        return (-1);
    }
    return 0;
}

static int set_fan_speed(int i2c, uint32_t speed)
{
	static const unsigned int devaddr[] = {
		0x2c, 0x2f
	};
	int res;
	int spd = 255*speed/100;
	int i, j;
	for(i=0; i < ARRAY_SIZE(devaddr); i++) {
		set_slave_addr(i2c, devaddr[i], 1);
		for(j=0; j<4; j++) {
			res = i2c_smbus_write_byte_data(i2c, PWM_REG_BASE+j, spd);
			if(res < 0) {
				printf("Error setting speed to fan %d (0x%x:0x%x): %d\n",
					   j, devaddr[i], j+PWM_REG_BASE, res);
				perror("\t");
			}
		}
	}
	return 1;
}

void usage(const char* name)
{
	printf("Usage: %s -b <bus number> -s <speed>\n", name);
	exit(1);
}

int main(int argc, char** argv)
{
	uint32_t speed=40;
	int i2c;
	int bus=-1;
	char o;
	char buspath[16];

	while((o=getopt(argc, argv, "s:b:"))!=-1) {
		switch(o)
		{
		case 's':
			speed = strtoul(optarg, 0, 0);
			break;
		case 'b':
			bus = strtoul(optarg, 0, 0);
			break;
		default:
			usage(argv[0]);
			break;
		}
	}
	if(bus < 0)
		usage(argv[0]);
	snprintf(buspath, sizeof(buspath), "/dev/i2c-%d", bus);
	i2c = open(buspath, O_RDWR);
	if(i2c<0) {
		perror("Error opening I2C device: ");
		return 1;
	}
	set_fan_speed(i2c, speed);
	close(i2c);
	return 0;
}
