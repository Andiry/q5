CC = gcc
CFLAGS = -O3
CLIB = -lm -lrt -lpthread

SRCS = $(wildcard *.c)
BUILD = $(patsubst %.c, %, $(SRCS))

all: $(BUILD)

.c:
	$(CC) $(CFLAGS) $< -o $@ $(CLIB)

clean:
	rm -rf q5


