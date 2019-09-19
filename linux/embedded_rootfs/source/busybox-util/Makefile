TARGET=cav_sh

.PHONY: all
all: ${TARGET} ${OBJS:.o=.d}

OBJS = cav_sh.o

SRCS = ${OBJS:.o:.c}

${TARGET}: ${OBJS}
	${CC} ${CFLAGS} -W -Wall -o $@ $<

# application object dependencies

-include ${OBJS:.o=.d}

%.d : %.c
	${CC} -M ${CFLAGS} $< -o $@

.PHONY: clean
clean:
	rm -f ${TARGET} ${OBJS} ${OBJS:.o=.d}

