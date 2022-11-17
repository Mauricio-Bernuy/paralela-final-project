#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <omp.h>
#include <string>
#include <iostream>

int nT = 8;
bool PRINTING = true;

int main(int argc, char *argv[]);
double f(double x);
double cpu_time(void);
void timestamp(void);

/******************************************************************************/

int main(int argc, char *argv[])

/******************************************************************************/
{
  std::string TESTING = "-noevalprints";
  if (argc > 1)
    if (!TESTING.compare(argv[1]))
      PRINTING = false;
  std::cin >> nT;
  long int n = 100000000;
  std::cin >> n;

  double a = 0.0;
  double b = 10.0;
  double error;
  double exact = 0.49936338107645674464;
  double wtime;
  double wtime1;
  double wtime2;

  if (PRINTING)
  {
    timestamp();
    printf("\n");
    printf("QUAD_OMP:\n");
    printf("  Integral de f(x)= 50/( pi * ( 2500 * x * x + 1 ) ).\n");
    printf("  A (inicio) = %f\n", a);
    printf("  B (fin)= %f\n", b);
    printf("  N = %ld\n", n);
    printf("  valor exacto = %24.16f\n", exact);
    printf("\n");
  }
  wtime1 = omp_get_wtime();

  double total = 0.0;
#pragma omp parallel for default(none) firstprivate(n, a, b) num_threads(nT) schedule(static) reduction(+ \
                                                                                                        : total)
  for (long int i = 0; i < n; i++)
  {
    double x = ((n - i - 1) * a + (i)*b) / (n - 1);
    total += f(x);
  }

  wtime2 = omp_get_wtime();

  total = (b - a) * total / (double)n;
  error = fabs(total - exact);
  wtime = wtime2 - wtime1;

  if (PRINTING)
  {
    printf("\n");
    printf("OMP_EXECUTION:\n");
    printf("  Estimate = %24.16f\n", total);
    printf("  Error    = %e\n", error);
    printf("  Time     = %20f\n", wtime);
    /*
      Terminate.
    */
    printf("\n");
    printf("QUAD_OMP:\n");
    printf("  Normal end of execution.\n");
    printf("\n");
    timestamp();
  }
  else
    printf("%f\n", wtime);
  return 0;
}
double f(double x)
{
  double pi = 3.141592653589793;
  double value;

  value = 50.0 / (pi * (2500.0 * x * x + 1.0));

  return value;
}

void timestamp(void)
{
#define TIME_SIZE 40

  static char time_buffer[TIME_SIZE];
  const struct tm *tm;
  time_t now;

  now = time(NULL);
  tm = localtime(&now);

  strftime(time_buffer, TIME_SIZE, "%d %B %Y %I:%M:%S %p", tm);

  printf("%s\n", time_buffer);

  return;
#undef TIME_SIZE
}
