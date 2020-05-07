#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define n 1001
#define count 40
#define DEBUG 0
#define THEADS 2

struct timeval start, end;

double u[n];
double v[n];
double w[n];
double x[n];
double y[n];

double serial_calculate()
{

    int i, j, k;
    clock_t ser_msec;

    ser_msec = clock();

    double expmax = 20.0;
    u[n - 1] = 0.99 * expmax * v[n - 1];

    for (k = 0; k < n; k++)
    {
        y[k] = u[k] / v[k];
        w[k] = x[k] / (exp(y[k]) - 1.0);
    }

    ser_msec = (clock() - ser_msec) * 1000000 / CLOCKS_PER_SEC;

    return ser_msec;
}

double paralel_calculate()
{

    int i, j, k;
    clock_t par_msec;

    par_msec = clock();

    omp_set_num_threads(4);
#pragma omp parallel shared(y, u, v, w, x) private(k)
    {
        double expmax = 20.0;
        u[n - 1] = 0.99 * expmax * v[n - 1];

        // int nthreads = omp_get_num_threads();
        // printf("Number of threads = %d\n", nthreads);

#pragma omp for
        for (k = 0; k < n; k += 4)
        {
            y[k] = u[k] / v[k];
            w[k] = x[k] / (exp(y[k]) - 1.0);

            y[k + 1] = u[k + 1] / v[k + 1];
            w[k + 1] = x[k + 1] / (exp(y[k + 1]) - 1.0);

            y[k + 2] = u[k + 2] / v[k + 2];
            w[k + 2] = x[k + 2] / (exp(y[k + 2]) - 1.0);
            
            y[k + 3] = u[k + 3] / v[k + 3];
            w[k + 3] = x[k + 3] / (exp(y[k + 3]) - 1.0);
            
        }
        y[n - 1] = u[n - 1] / v[n - 1];
        w[n - 1] = x[n - 1] / (exp(y[n - 1]) - 1.0);
    }

    par_msec = (clock() - par_msec) * 1000000 / CLOCKS_PER_SEC;

    return par_msec;
}

int main(int argc, char *argv[])
{

    double avg_ser_msec = 0, avg_par_msec = 0;

    for (int i = 0; i < count; i++)
    {
        avg_ser_msec += serial_calculate();
        avg_par_msec += paralel_calculate();
    }

    avg_ser_msec = avg_ser_msec / count;
    avg_par_msec = avg_par_msec / count;

    printf("optimizing with list size n = %d \nresults based on %d times of run. \n",n,count);

    printf("average serial time: %.3f microsecond\n", avg_ser_msec);
    printf("average paralel time: %.3f microsecond\n", avg_par_msec);

    printf("speed up: %.3f microsecond\n", avg_ser_msec / avg_par_msec);

    return 0;
}
