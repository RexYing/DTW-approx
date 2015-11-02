#!/bin/bash

EXE=./bin/executable.exe

#make clean
make
printf "\nRunning program for test t1.dtw: -------------\n\n"
#$EXE --v=6 --curve1=data/t1.dtw --curve2=data/t2.dtw --rects_output_file="results/rects_simple.txt"
#$EXE --v=6 --curve1=data/t5.dtw --curve2=data/t6.dtw --rects_output_file="results/rects1.txt" --eps=0.3
#$EXE data/t3.dtw data/t4.dtw --v=6

$EXE --v=6 --curve1=data/rand1.dtw --curve2=data/rand2.dtw --rects_output_file="results/rects_rand_1_2.txt" --eps=0.5
#$EXE --v=6 --curve1=data/rand3.dtw --curve2=data/rand4.dtw --rects_output_file="results/rects2.txt" --eps=0.5
#$EXE --v=6 --curve1=data/line1.dtw --curve2=data/line2.dtw --rects_output_file="results/rects_line_1_2.txt" --eps=0.5
