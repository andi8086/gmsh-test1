all: test1


CFLAGS +=

LDFLAGS += -lgmsh -lm

test1: test1.c
	gcc $(CFLAGS) -o $@ $< $(LDFLAGS)
