CC=gcc
CFLAGS=-Wall
DFLAGS=-g
LDFLAGS=-lm
PROGS= fp_parse

all: $(PROGS)

%: %.c
	$(CC) $(CFLAGS) $(DFLAGS) $< -o $@ $(LDFLAGS)

clean:
	rm -f $(PROGS) *.o ~*

