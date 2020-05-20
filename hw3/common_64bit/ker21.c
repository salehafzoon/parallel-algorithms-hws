
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define n 101
#define count 20
#define DEBUG 0
#define THEADS 2

struct timeval start, end;

double px[n][25];
double cx[n][25];
double vy[25][n];

void print_array(double arr[n][25])
{

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < 25; j++)
        {
            printf("%.2f\t", arr[i][j]);
        }
        printf("\n");
    }
}

double serial_calculate()
{

    int i, j, k;
    clock_t ser_msec;

    // initializing
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < 25; j++)
        {
            px[i][j] = 0;
            cx[i][j] = 2;

            vy[j][i] = 2;
        }
    }

    ser_msec = clock();

    for (k = 0; k < 25; k++)
    {
        for (i = 0; i < 25; i++)
        {
            for (j = 0; j < n; j++)
            {
                px[j][i] += vy[k][i] * cx[j][k];
            }
        }
    }

    ser_msec = (clock() - ser_msec) * 1000000 / CLOCKS_PER_SEC;

    return ser_msec;
}

double paralel_calculate()
{
    int i, j, k;
    clock_t par_msec;

    // initializing
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < 25; j++)
        {
            px[i][j] = 0;
            cx[i][j] = 2;
            vy[j][i] = 2;
        }
    }

    par_msec = clock();
    omp_set_num_threads(THEADS);

#pragma omp parallel shared(px, vy, cx) private(k, i, j)
    {
#pragma omp for collapse(2)
        for (j = 0; j < n - 1; j++)
        {
            // int nthreads = omp_get_num_threads();
            // printf("Number of threads = %d\n", nthreads);

            for (k = 0; k < 25; k++)
            {
                for (i = 0; i < 25; i += 5)
                {
                    {
                        px[j][i] += vy[k][i] * cx[j][k];
                        px[j][i + 1] += vy[k][i + 1] * cx[j][k];
                        px[j][i + 2] += vy[k][i + 2] * cx[j][k];
                        px[j][i + 3] += vy[k][i + 3] * cx[j][k];
                        px[j][i + 4] += vy[k][i + 4] * cx[j][k];
                    }
                }
            }
        }
    }

    par_msec = (clock() - par_msec) * 1000000 / CLOCKS_PER_SEC;

    if (DEBUG)
        print_array(px);

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
