#!/bin/make

all: clean build

build: index search

index:
	cc -g -o0 -std=c99 src/parse/*.c src/common/*.c -o ./index

search:
	cc -g -o0 -std=c99 src/search/*.c src/common/*.c -o ./search

clean:
	rm -rf ./index ./search src/*/*.o

.PHONY: all
