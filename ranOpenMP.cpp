// ranOpenMP.cpp

#include <omp.h>
#include "RngStream.h"
#include <iostream>
int main()
{
  int nP = omp_get_num_procs();
  omp_set_num_threads(nP); // set number of threads
  unsigned long seed[6] = {1806547166, 3311292359,
                           643431772, 1162448557,
                           3335719306, 4161054083};
  RngStream::SetPackageSeed(seed);
  RngStream RngArray[nP]; // array of RngStream objects
  int myRank;
#pragma omp parallel private(myRank)
  {
    myRank = omp_get_thread_num();
#pragma omp critical
    {
      // std::cout << RngArray[myRank].RandU01() << " ";
      for (int i = 0; i < 100; i++)
        std::cout << RngArray[myRank].RandInt(0, 10) << " ";
    }
  }
  std::cout << std::endl;
  return 0;
}