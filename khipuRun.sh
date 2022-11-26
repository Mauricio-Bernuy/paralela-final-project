#!/bin/bash
# load 
module load gcc
module load python

# compile serial and omp
g++ BrokenTriangleSerial.cpp RngStream.cpp -o BrokenTriangleSerial 
g++ -fopenmp BrokenTriangleOMP.cpp RngStream.cpp -o BrokenTriangleOMP
# run python test scripts
python3 testerSerial.py > serialResults.txt
python3 testerOMP.py > OMPResults.txt

# unload 
module unload gcc
module unload python