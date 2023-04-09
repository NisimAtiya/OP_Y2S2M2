CC = gcc
all: cmp copy

copy:
	cc copy copy.c

cmp:
	cc -o cmp cmp.c


.PHONY: clean all

clean:
	rm -f copy cmp