#!/bin/bash

EXE=./bin/curve_registration.exe

#make clean
make

$EXE --v=6 --curve1=data/t1.txt --curve2=data/t2.txt --output_dir="results/t1_icp" --eps=1.0 --method="DTW-approx"
