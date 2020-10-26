#!/bin/sh

make clean
make -f Makefile
time ./run.x > results.txt
