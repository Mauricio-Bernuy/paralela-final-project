#!/bin/bash
# load 
module load gcc
module load python

# compile serial and omp
g++ BrokenTriangleSerial.cpp RngStream.cpp -o BrokenTriangleSerial 
g++ -fopenmp BrokenTriangleOMP.cpp RngStream.cpp -o BrokenTriangleOMP

# run python test scripts
unbuffer python3 testerSerial.py | tee serialResults.txt
unbuffer python3 testerOMP.py | tee OMPResults.txt

# unload 
module unload gcc
module unload python