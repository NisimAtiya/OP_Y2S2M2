CC = gcc
all: cmp copy

copy:
	cc -o copy copy.c

cmp:
	cc -o cmp cmp.c


.PHONY: clean all

clean:
	rm -f copy cmp