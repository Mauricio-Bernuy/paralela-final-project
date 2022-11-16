n = 100000000
echo 0 | sudo tee /proc/sys/kernel/yama/ptrace_scope # disable wsl vader error

# serial
g++ quad_serial.c -o serial && ./serial

# OMP
g++ -fopenmp quad_parallel_omp.c -o omp && ./omp
g++ -fopenmp quad_parallel_omp.c -o omp && ./omp -noevalprints
python3 tester_omp.py

# MPI
mpic++ quad_parallel_mpi.c -o mpi && mpirun -np 4 ./mpi
mpic++ quad_parallel_mpi.c -o mpi && mpirun -np 4 ./mpi -noevalprints
python3 tester_mpi.py
