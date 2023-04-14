CC = gcc
all: cmp copy decode encode

encode:
	cc -o encode encode.c

decode:
	cc -o decode decode.c

copy:
	cc -o copy copy.c

cmp:
	cc -o cmp cmp.c


.PHONY: clean all

clean:
	rm -f copy cmp