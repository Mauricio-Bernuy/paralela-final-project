#!/bin/bash
# load 
module load gcc
module load python

# compile serial and omp
g++ BrokenTriangleSerial.cpp RngStream.cpp -o BrokenTriangleSerial 
g++ -fopenmp BrokenTriangleOMP.cpp RngStream.cpp -o BrokenTriangleOMP

# run python test scripts
unbuffer python3 testerSerial.py | tee test_results/serialResults.txt
unbuffer python3 testerOMP.py | tee test_results/OMPResults.txt

# run final with n=10^11 
unbuffer ./BrokenTriangleOMP 80 100000000000 | tee test_results/bigOMPResult.txt

# unload 
module unload gcc
module unload python