CC = gcc
all: cmp copy decode encode libcodecA libcodecB

libcodecA:
	gcc -shared -o libcodecA.so codecA.c

libcodecB:
	gcc -shared -o libcodecB.so codecB.c

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
	rm -f copy cmp decode encode libcodecA.so libcodecB.so