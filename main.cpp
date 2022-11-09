#include <omp.h>
#include <math.h>
#include <random>
#include <string>
#include <iostream>
#include "RngStream.h"

bool PRINT_ = false; // print debug info
int nT = 1;          // number of threads
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

// run one instance of experiment with set lenght and precision
bool_data experiment(double interval_, int length_, int precision_)
{
  double p1, p2;
  // std::uniform_real_distribution<double> distribution(0.0, 1.0);
  // std::default_random_engine generator;

  // random points in interval
  // double p1 = random.randint(0, interval_);
  // double p2 = random.randint(0, interval_);

  int myRank = omp_get_thread_num();
  // int myRank = 0; // simulate repeted rng stream

  // double precision from 0 to 1
  if (length_ == 1 && precision_ == 0)
  {
    p1 = RngArray[myRank].RandU01();
    p2 = RngArray[myRank].RandU01();
  }

  // precision set by precision_
  else
  {
    double low = 0;
    double high = interval_;
    //* long int cast for up to length_ = 1, precision_ = 19
    p1 = low + static_cast<long int>((high - low + 1.0) * RngArray[myRank].RandU01());
    p2 = low + static_cast<long int>((high - low + 1.0) * RngArray[myRank].RandU01());
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

  if (PRINT_)
  {
    // print("interval:")
    // print(f"{0:.{precision_}f}" + " to " + f"{length_:.{precision_}f}\n")
    // print("segments:")
    // print(f"{i_1/(10**precision_):.{precision_}f}")
    // print(f"{i_2/(10**precision_):.{precision_}f}")
    // print(f"{i_3/(10**precision_):.{precision_}f}\n")
  }

  // check triangle and obtuse conditions for given sub intervals
  bool_data ret_data = check_conditions(i_1, i_2, i_3);
  return ret_data;
}

// Simultation
void monte_carlo_sim(long int iterations = 10000, int length = 1, int precision = 1, int experiments = 10)
{
  long double trig_prob_full = 0;
  long double obt_prob_full = 0;

  printf("nt: %d\n", nT);

  // Monte Carlo Simulation (run expermient
  // n times and get average of results to
  // obtain approximate probability)

  double interval = length * pow(10, precision);

#pragma omp parallel for num_threads(nT) schedule(static) reduction(+ \
                                                                    : trig_prob_full, obt_prob_full)
  for (long int i = 0; i < iterations; i++)
  {
    bool_data ret = experiment(interval, length, precision);
    trig_prob_full += ret.isTriangle;
    obt_prob_full += ret.isObtuse;
  }

  printf("\n\nfinal added probs: %Lf, %Lf\n", trig_prob_full, obt_prob_full);
  printf("divide by: %ld\n", iterations);

  trig_prob_full /= iterations;
  obt_prob_full /= iterations;

  printf("parallel experiment: %Lf, %Lf\n", trig_prob_full, obt_prob_full);
}

int main(int argc, char *argv[])
{
  nT = std::stoi(argv[1]);
  long int nexp = std::stol(argv[2]);

  if (nT == 0)
    nT = omp_get_num_procs();

  unsigned long seed[6] = {1806547166, 3311292359,
                           643431772, 1162448557,
                           3335719306, 4161054083};
  RngStream::SetPackageSeed(seed);
  RngArray = new RngStream[nT];

  monte_carlo_sim(nexp, 1, 0);
  return 0;
}