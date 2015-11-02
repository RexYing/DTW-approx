#!/bin/bash

BIN=./bin/gen_tests.exe

OPT_OUTPUT=--output_file
OPT_TYPE=--curve_type
OPT_PARAM=--params

$BIN $OPT_OUTPUT=data/line1.dtw $OPT_TYPE=line $OPT_PARAM="start=(0,0) angle=0.5 step=1 n=500"
$BIN $OPT_OUTPUT=data/line2.dtw $OPT_TYPE=line $OPT_PARAM="start=(0,5) angle=0.5 step=1 n=500"

$BIN $OPT_OUTPUT=data/rand3.dtw $OPT_TYPE=rand $OPT_PARAM="start=(0,0) angle=(-0.2,0.2) step=(2,4) n=500"
$BIN $OPT_OUTPUT=data/rand4.dtw $OPT_TYPE=rand $OPT_PARAM="start=(0,-10) angle=(-0.2,0.2) step=(2,4) n=500"

$BIN $OPT_OUTPUT=data/rand1.dtw $OPT_TYPE=rand $OPT_PARAM="start=(0,0) angle=(-0.1,0.1) step=(2,4) n=5000"
$BIN $OPT_OUTPUT=data/rand2.dtw $OPT_TYPE=rand $OPT_PARAM="start=(0,-10) angle=(-0.1,0.1) step=(2,4) n=5000"
