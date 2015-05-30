#!/bin/bash
make clean
make
printf "\nRunning program for test t1.dtw: -------------\n\n"
#./executable tests/t1.dtw tests/t2.dtw --v=7

./executable tests/t3.dtw tests/t4.dtw --v=7

