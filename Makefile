CC := clang

all: mhowf

mhowf: main.c mhowf.c
	$(CC) -O3 -o $@ $^

clean:
	rm -f mhowf

.PHONY: all clean
