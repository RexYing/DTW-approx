#!/bin/bash

make clean
make
printf "\nRunning program for test t1.dtw: -------------\n\n"
./executable tests/t1.dtw --v=8


