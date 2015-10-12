#!/bin/bash

BIN=./bin/gen_tests.exe

OPT_OUTPUT=--output_file
OPT_TYPE=--curve_type
OPT_PARAM=--params

$BIN $OPT_OUTPUT=tests/line1.dtw $OPT_TYPE=line $OPT_PARAM="start=(0,0) angle=0.5 step=1 n=500"
$BIN $OPT_OUTPUT=tests/line2.dtw $OPT_TYPE=line $OPT_PARAM="start=(0,5) angle=0.5 step=1 n=500"

$BIN $OPT_OUTPUT=tests/rand1.dtw $OPT_TYPE=rand $OPT_PARAM="start=(0,0) angle=(-0.5,0.5) step=(1,2) n=500"
$BIN $OPT_OUTPUT=tests/rand2.dtw $OPT_TYPE=rand $OPT_PARAM="start=(0,-20) angle=(-0.5,0.5) step=(1,2) n=500"
