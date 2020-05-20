
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

double za[7][n];
double zr[7][n];
double zb[7][n];
double zu[7][n];
double zv[7][n];
double zz[7][n];
double qa;

double serial_calculate()
{

    int i, j, k;
    clock_t ser_msec;

    ser_msec = clock();

    for (j = 1; j < 6; j++)
    {
        for (k = 1; k < n; k++)
        {
            qa = za[j + 1][k] * zr[j][k] + za[j - 1][k] * zb[j][k] +
                 za[j][k + 1] * zu[j][k] + za[j][k - 1] * zv[j][k] + zz[j][k];

            za[j][k] += 0.175 * (qa - za[j][k]);
        }
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
#pragma omp parallel
    {
#pragma omp for private(j) collapse(2)
        for (k = 1; k < n; k++)
        {
            for (j = 1; j < 6; j += 2)
            {
                qa = za[j + 1][k] * zr[j][k] + za[j - 1][k] * zb[j][k] +
                     za[j][k + 1] * zu[j][k] + za[j][k - 1] * zv[j][k] + zz[j][k];

                za[j][k] += 0.175 * (qa - za[j][k]);

                if (j + 1 < 6)
                {
                    qa = za[j + 1][k] * zr[j][k] + za[j - 1][k] * zb[j][k] +
                         za[j][k + 1] * zu[j][k] + za[j][k - 1] * zv[j][k] + zz[j][k];

                    za[j][k] += 0.175 * (qa - za[j][k]);
                }
            }
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
