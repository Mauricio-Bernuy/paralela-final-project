#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <mpi.h>
#include <string>
#include <iostream>

int nT = 4;
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

  int my_rank;

  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &nT);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  double a, b, error, exact;
  long int n;
  double total;

  double wtime;
  double wtime1;
  double wtime2;

  if (!my_rank)
  {
    a = 0.0;
    b = 10.0;
    n = 100000000;
    std::cin >> n;

    exact = 0.49936338107645674464;
    total = 0.0;

    if (PRINTING)
    {
      timestamp();
      printf("\n");
      printf("QUAD_MPI:\n");
      printf("  Integral de f(x)= 50/( pi * ( 2500 * x * x + 1 ) ).\n");
      printf("A (inicio) = %f\n", a);
      printf("B (fin)= %f\n", b);
      printf("  N = %ld\n", n);
      printf("valor exacto = %24.16f\n", exact);
      printf("\n");
    }

    wtime1 = cpu_time();
  }
  if (PRINTING)
    printf("Broadcasting:\n");

  MPI_Bcast(&a, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Bcast(&b, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Bcast(&n, 1, MPI_LONG, 0, MPI_COMM_WORLD);

  long int count = n / nT;
  long int remainder = n % nT;
  long int start, stop;

  if (my_rank < remainder)
  {
    start = my_rank * (count + 1);
    stop = start + count;
  }
  else
  {
    start = my_rank * count + remainder;
    stop = start + (count - 1);
  }

  if (PRINTING)
    printf("Thread %d start: %ld, stop: %ld, a: %lf, b:%lf\n", my_rank, start, stop, a, b);

  double loc_total = 0.0;
  for (long int i = start; i <= stop; ++i)
  {
    double x = ((n - i - 1) * a + (i)*b) / (n - 1);
    loc_total += f(x);
  }

  if (PRINTING)
    printf("Finished in %d, loc_total %lf\n", my_rank, loc_total);

  MPI_Reduce(&loc_total, &total, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

  if (!my_rank)
  {
    wtime2 = cpu_time();
    total = (b - a) * total / (double)n;
    error = fabs(total - exact);
    wtime = wtime2 - wtime1;

    if (PRINTING)
    {
      printf("\n");
      printf("  Estimate = %24.16f\n", total);
      printf("  Error    = %e\n", error);
      printf("  Time     = %f\n", wtime);
      printf("\n");
      printf("QUAD_MPI:\n");
      printf("  Normal end of execution.\n");
      printf("\n");
      timestamp();
    }
    else
      printf("%f\n", wtime);
  }
  MPI_Finalize();
  return 0;
}
double f(double x)
{
  double pi = 3.141592653589793;
  double value;

  value = 50.0 / (pi * (2500.0 * x * x + 1.0));

  return value;
}

double cpu_time(void)
{
  double value;

  value = (double)clock() / (double)CLOCKS_PER_SEC;

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
