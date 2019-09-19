CFLAGS = -g -O0 -W -Wall -Wno-unused-parameter -I$(OCTEON_ROOT)/executive
LDFLAGS = -O2 -lz


.PHONY: all
all: write_stage1 translate_env
	- sudo cp -pf $^ $(OCTEON_ROOT)/host/bin

write_stage1: write_stage1.o
	$(CC) $(LDFLAGS) -o $@ $?
	- sudo chown -f root $@
	- sudo chmod -f +s,g+r,o+r $@

translate_env: translate_env.o
	$(CC) $(LDFLAGS) -o $@ $?

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f write_stage1 *.o