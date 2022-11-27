// #include <omp.h>
#include <time.h>
#include <math.h>
#include <random>
#include <string>
#include <iostream>
#include "RngStream.h"

bool PRINT_ = false;   // print debug info
bool RES_ONLY = false; // print debug info

// int nT = 1; // number of threads
RngStream *RngArray;

// optimized condition, its obtuse if len is equal or lower than zero
double get_len(double s1, double s2, double s3)
{
  return pow(s2, 2) + pow(s3, 2) - pow(s1, 2);
}

// check conditions for being obtuse
bool obtuse_condition_optimized(double s1, double s2, double s3)
{
  double c1 = get_len(s1, s2, s3) < 0;
  double c2 = get_len(s2, s1, s3) < 0;
  double c3 = get_len(s3, s1, s2) < 0;
  return c1 || c2 || c3;
}

// check conditions for being a triangle
bool side_condition(double s1, double s2, double s3)
{
  bool c1 = s1 <= (s2 + s3);
  bool c2 = s2 <= (s1 + s3);
  bool c3 = s3 <= (s1 + s2);
  bool c4 = s1 != 0 && s2 != 0 && s3 != 0;
  return c1 && c2 && c3 && c4;
}

struct bool_data
{
  bool isTriangle;
  bool isObtuse;
};

// check both conditions and return a boolean for each
bool_data check_conditions(double a_, double b_, double c_)
{
  bool isTriangle = false;
  bool isObtuse = false;

  if (side_condition(a_, b_, c_))
  {

    if (PRINT_)
      printf("is triangle\n");
    isTriangle = true;
    if (obtuse_condition_optimized(a_, b_, c_))
    {
      if (PRINT_)
        printf("is obtuse\n");
      isObtuse = true;
    }
    else if (PRINT_)
      printf("is not obtuse\n");
  }
  else if (PRINT_)
    printf("is not triangle\n");

  return bool_data{isTriangle, isObtuse};
}

// run one instance of experiment with set length and precision
bool_data experiment(double interval_, int length_, int precision_)
{
  double p1, p2;

  // double precision from 0 to 1
  if (length_ == 1 && precision_ == 0)
  {
    p1 = RngArray[0].RandU01();
    p2 = RngArray[0].RandU01();
  }

  // precision set by precision_
  else
  {
    double low = 0;
    double high = interval_;
    //* long int cast for up to length_ = 1, precision_ = 19
    p1 = low + static_cast<long int>((high - low + 1.0) * RngArray[0].RandU01());
    p2 = low + static_cast<long int>((high - low + 1.0) * RngArray[0].RandU01());
  }

  // set p1 as min and p2 as max
  if (p1 > p2)
  {
    double temp = p1;
    p1 = p2;
    p2 = temp;
  }

  // create sub intervals
  double i_1 = p1 - 0;
  double i_2 = p2 - p1;
  double i_3 = interval_ - p2;

  // check triangle and obtuse conditions for given sub intervals
  bool_data ret_data = check_conditions(i_1, i_2, i_3);
  return ret_data;
}

// Simultation
void monte_carlo_sim(long int iterations = 10000, int length = 1, int precision = 1, int experiments = 10)
{
  clock_t w1, w2;
  long double trig_prob_full = 0;
  long double obt_prob_full = 0;

  if (!RES_ONLY)
    printf("Running in Serial");

  // Monte Carlo Simulation (run expermient
  // n times and get average of results to
  // obtain approximate probability)
  double interval = length * pow(10, precision);

  w1 = clock();

  for (long int i = 0; i < iterations; i++)
  {
    bool_data ret = experiment(interval, length, precision);
    trig_prob_full += ret.isTriangle;
    obt_prob_full += ret.isObtuse;
  }

  w2 = clock();

  if (!RES_ONLY)
  {
    printf("----------------\n");
    printf("reduced probability count: %Lf, %Lf\n", trig_prob_full, obt_prob_full);
    printf("divide by: %ld iterations\n", iterations);
  }

  trig_prob_full /= iterations;
  obt_prob_full /= iterations;

  if (!RES_ONLY)
  {
    printf("----------------\n");
    printf("Parallel Monte Carlo results:\n\n| triangle probability | obtuse probability |\n");
    printf("\t%Lf       |      %Lf\t\n", trig_prob_full, obt_prob_full);
    printf("----------------\n");
    printf("Parallel experiment took %f seconds\n", (double)(w2 - w1) / CLOCKS_PER_SEC);
  }
  else
    printf("%Lf\n%Lf\n%f\n", trig_prob_full, obt_prob_full, (double)(w2 - w1) / CLOCKS_PER_SEC);
}

int main(int argc, char *argv[])
{
  long int nexp = 10000;

  if (argc > 1)
    nexp = std::stol(argv[1]);
  if (argc > 2)
    if (std::string("RES_ONLY").compare(argv[2]) == 0)
      RES_ONLY = true;

  unsigned long seed[6] = {1806547166, 3311292359,
                           643431772, 1162448557,
                           3335719306, 4161054083};

  RngStream::SetPackageSeed(seed);
  RngArray = new RngStream[1];

  monte_carlo_sim(nexp, 1, 0);
  return 0;
}