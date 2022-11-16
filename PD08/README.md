
g++ -fopenmp quad_serial_omp.c -o omp && ./omp
g++ -fopenmp quad_serial_omp.c -o omp && ./omp -noevalprints


mpic++ quad_serial_mpi.c -o mpi && mpirun -np 4 ./mpi
mpic++ quad_serial_mpi.c -o mpi && mpirun -np 4 ./mpi -noevalprints
