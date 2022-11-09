#!/bin/bash

# Nombre del job:
#SBATCH --job-name=multi_thread_omp_job
# Nombre del archivo de salida:
#SBATCH --output=multi_thread_omp_job.txt
# Numero de tasks:
#SBATCH --ntasks=1
# Numero de CPUs por task:
#SBATCH --cpus-per-task=4
# Límite de tiempo de 10 min:
#SBATCH --time=10:00

export OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK

module load gcc/
g++ -fopenmp main.cpp RngStream.cpp -o main 
module unload gcc/5.5.0 

time ./main