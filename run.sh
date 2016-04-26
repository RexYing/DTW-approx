#!/bin/bash

EXE=./bin/executable.exe

#make clean
make
printf "\nRunning program for test t1.dtw: -------------\n\n"
#$EXE --v=6 --curve1=data/t1.dtw --curve2=data/t2.dtw --rects_output_file="results/rects_simple.txt"
#$EXE --v=6 --curve1=data/t5.dtw --curve2=data/t6.dtw --rects_output_file="results/rects1.txt" --eps=0.3
#$EXE data/t3.dtw data/t4.dtw --v=6

#$EXE --v=6 --curve1=data/rand1.dtw --curve2=data/rand2.dtw --rects_output_file="results/rects_rand_1_2.txt" --eps=0.5
#$EXE --v=6 --curve1=data/rand3.dtw --curve2=data/rand4.dtw --rects_output_file="results/rects_rand_3_4.txt" --eps=1

#$EXE --v=6 --curve1=data/rand5.dtw --curve2=data/rand6.dtw --rects_output_file="results/rects_rand_5_6.txt" --eps=1.0
#$EXE --v=6 --curve1=data/line1.dtw --curve2=data/line2.dtw --rects_output_file="results/rects_line_1_2.txt" --eps=0.5
#$EXE --v=6 --curve1=data/rand_linear1.dtw --curve2=data/rand_linear2.dtw --rects_output_file="results/rects_rand_lin_1_2.txt" --eps=1
#$EXE --v=6 --curve1=data/rand3.dtw --curve2=data/rand7.dtw --rects_output_file="results/rects_rand_3_7.txt" --align_output_file="results/rects_rand_3_7_align.txt" --exact_align_output_file="results/rects_rand_3_7_exact_align.txt" --approx_frechet_align_output_file="results/rects_rand_3_7_approx_frechet_align.txt" --eps=1.0
#$EXE --v=6 --curve1=data/rand5.dtw --curve2=data/rand6.dtw --rects_output_file="results/rects_rand_5_6.txt" --align_output_file="results/rects_rand_5_6_align.txt" --exact_align_output_file="results/rects_rand_5_6_exact_align.txt" --approx_frechet_align_output_file="results/rects_rand_5_6_approx_frechet_align.txt" --eps=1.0

$EXE --v=6 --curve1=data/GeoLife/005/20090119101553.txt --curve2=data/GeoLife/005/20090208045522_translated_1.txt --rects_output_file="results/trajectories/20090119101553_20090208045522_translated_1.txt" --align_output_file="results/trajectories/20090119101553_20090208045522_translated_1_approx_align.txt" --exact_align_output_file="results/trajectories/20090119101553_20090208045522_translated_1_exact_align.txt" --approx_frechet_align_output_file="results/trajectories/20090119101553_20090208045522_translated_1_frechet_align.txt" --eps=1.0