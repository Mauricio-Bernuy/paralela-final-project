#!/bin/bash

# Nombre del job:
#SBATCH --job-name=BrokenTriangleOMP
# Nombre del archivo de salida:
#SBATCH --output=BrokenTriangleOMP.txt
# Numero de tasks:
#SBATCH --ntasks=1
# Numero de CPUs por task:
#SBATCH --cpus-per-task=80
# Límite de tiempo de 60 min:
#SBATCH --time=60:00

export OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK
bash ./khipuRun.sh