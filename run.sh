#!/bin/bash

EXE=./bin/executable.exe

make clean
make
printf "\nRunning program for test t1.dtw: -------------\n\n"
#$EXE --v=6 --curve1=tests/t1.dtw --curve2=tests/t2.dtw --rects_output_file="results/rects_simple.txt"
$EXE --v=6 --curve1=tests/t5.dtw --curve2=tests/t6.dtw --rects_output_file="results/rects1.txt" --eps=0.3
#$EXE tests/t3.dtw tests/t4.dtw --v=6

#$EXE --v=6 --curve1=tests/rand3.dtw --curve2=tests/rand4.dtw --rects_output_file="results/rects2.txt" --eps=0.5
