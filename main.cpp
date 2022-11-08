#include <math.h>
#include <random>
#include <iostream>

bool PRINT_ = false;

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
  // return isTriangle, isObtuse
}

// run one instance of experiment with set lenght and precision
bool_data experiment(double interval_, int length_, int precision_)
{
  // std::uniform_real_distribution<double> distribution(0.0, 1.0);
  // std::default_random_engine generator;

  // random points in interval
  // double p1 = random.randint(0, interval_);
  // double p2 = random.randint(0, interval_);

  double p1 = 0.2;
  double p2 = 0.6;

  // set p1 as min and p2 as max
  if (p1 > p2)
  {
    double temp = p1;
    p1 = p2;
    p2 = temp;
  }

  // create sub intervals
  // double i_1 = p1 - 0;
  // double i_2 = p2 - p1;
  // double i_3 = interval_ - p2;
  double i_1 = 0.3;
  double i_2 = 0.4;
  double i_3 = 0.6;

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
void monte_carlo_sim(int iterations = 10000, int length = 1, int precision = 1, int experiments = 10)
{
  // for t in range(experiments):
  for (int t = 0; t < experiments; t++)
  {
    // Monte Carlo Simulation (run expermient
    // n times and get average of results to
    // obtain approximate probability)
    // bool [iterations] trig_hist;
    // bool [iterations] obt_hist;
    double trig_prob = 0;
    double obt_prob = 0;
    double interval = length * pow(10, precision);

    for (int i = 0; i < iterations; i++)
    {
      bool_data ret = experiment(interval, length, precision);
      trig_prob += ret.isTriangle;
      obt_prob += ret.isObtuse;
    }

    trig_prob /= iterations;
    obt_prob /= iterations;
    printf("experiment %d: %f, %f\n", t, trig_prob, obt_prob);
  }
}

int main()
{
  // std::cout << side_condition(3, 4, 5) << "\n";
  // std::cout << obtuse_condition_optimized(3, 4, 5) << "\n";

  // bool_data ret = check_conditions(3, 4, 5);
  // std::cout << ret.isTriangle << " " << ret.isObtuse << "\n";
  monte_carlo_sim();
  return 0;
}