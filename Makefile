#!/bin/make

all: clean build

build: index search

index:
	gcc -D_GNU_SOURCE -std=c99 src/parse/*.c src/common/*.c -o ./index

search:
	gcc -D_GNU_SOURCE -std=c99 src/search/*.c src/common/*.c -o ./search

clean:
	rm -rf ./index ./search src/*/*.o

.PHONY: all
