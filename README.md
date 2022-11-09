# paralela-final-project
 
g++ -fopenmp ranOpenMP.cpp RngStream.cpp -o ranOpenMP

# run
g++ -fopenmp main.cpp RngStream.cpp -o main && time ./main 0 10000000