#!/bin/sh

make clean
make -f Makefile_A_8_8
time ./run_A_8_8.x > results_A_8_8.txt

make clean
make -f Makefile_A_8_32
time ./run_A_8_32.x > results_A_8_32.txt

make clean
make -f Makefile_A_8_64
time ./run_A_8_64.x > results_A_8_64.txt

make clean
make -f Makefile_A_8_96
time ./run_A_8_96.x > results_A_8_96.txt

make clean
make -f Makefile_A_32_8
time ./run_A_32_8.x > results_A_32_8.txt

make clean
make -f Makefile_A_64_8
time ./run_A_64_8.x > results_A_64_8.txt

make clean
make -f Makefile_A_96_8
time ./run_A_96_8.x > results_A_96_8.txt

make clean
make -f Makefile_B
time ./run_B.x > results_B.txt
