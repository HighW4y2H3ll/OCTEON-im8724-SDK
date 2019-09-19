ifndef OCTEON_ROOT
OCTEON_ROOT = ../../../..
endif

I2C_HDRS = ../../build/i2c-tools-3.1.2/include

CFLAGS = -Wall -O2 -I$(I2C_HDRS)

CC = mips64-octeon-linux-gnu-gcc

all: airflow fanctl

clean:
	$(RM) airflow fanctl *.o *~
