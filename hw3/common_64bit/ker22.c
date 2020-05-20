#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define n 1001
#define count 20
#define DEBUG 0
#define THEADS 4

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

    omp_set_num_threads(THEADS);
#pragma omp parallel private(k)
    {
        double expmax = 20.0;
        u[n - 1] = 0.99 * expmax * v[n - 1];

#pragma omp for
        for (k = 0; k < n; k += 4)
        {
            y[k] = u[k] / v[k];
            w[k] = x[k] / (exp(y[k]) - 1.0);

            // because 101 or 1001 has remaining as 1 when dividing by 4
            // checking for last loop iteration
            if (k + 1 < n)
                y[k + 1] = u[k + 1] / v[k + 1];
                w[k + 1] = x[k + 1] / (exp(y[k + 1]) - 1.0);

                y[k + 2] = u[k + 2] / v[k + 2];
                w[k + 2] = x[k + 2] / (exp(y[k + 2]) - 1.0);

                y[k + 3] = u[k + 3] / v[k + 3];
                w[k + 3] = x[k + 3] / (exp(y[k + 3]) - 1.0);
        }
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

    printf("list size n = %d\n", n);
    printf("number of threads= %d\n", THEADS);
    printf("times of run = %d. \n-------------\n", count);

    printf("average serial time: %.3f microsecond\n", avg_ser_msec);
    printf("average paralel time: %.3f microsecond\n", avg_par_msec);

    printf("-------------\nspeed up: %.3f microsecond\n", avg_ser_msec / avg_par_msec);

    return 0;
}
