#!/bin/make

all: clean build

build: index search

index:
	cc -g -o0 src/parse/*.c common/*.c -o ./index

search:
	cc -g -o0 src/search/*.c common/*.c -o ./search

clean:
	rm -rf ./index ./search src/*/*.o

.PHONY: all
