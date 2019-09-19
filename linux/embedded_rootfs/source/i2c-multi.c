#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#include <linux/i2c-dev.h>

/*
 * Read the first 8 registers out of the DS1337 with a single
 * multi-byte read.
 */
int main(int argc, char *argv[])
{
  int i2cfd;
  int ds1337 = 0x68;
  int nr;
  int i;
  char buffer[16];

  printf("Multi-byte I2C Read...\n");

  if ((i2cfd = open("/dev/i2c-0", O_RDWR)) < 0) {
    perror("open(\"/dev/i2c-0\", O_RDWR) failed");
    exit(2);
  }
  if (ioctl(i2cfd, I2C_SLAVE, ds1337) < 0) {
    perror("ioctl failed");
    exit(2);
  }
  buffer[0] = 0;

  nr = write(i2cfd, buffer, 1);

  if (nr != 1) {
    perror("write failed");
    exit(2);
  }

  nr = read(i2cfd, buffer, sizeof(buffer));

  if (nr != sizeof(buffer)) {
    perror("read failed");
    exit(2);
  }

  for (i = 0; i < sizeof(buffer); i++) {
    printf("%2.2x ", buffer[i]);
  }
  printf("\n");

  i = buffer[0] | buffer[1] | buffer[2] | buffer[3];

  if (i == 0) {
    /* According to DS1337 manual, register 0-3 contain valid information */
    printf("Error: registers 0..3 all zero\n");
    exit(1);
  }
  printf("...Success\n");
  exit(0);
}
