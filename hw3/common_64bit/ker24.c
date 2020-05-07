
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define n 101
#define count 20
#define DEBUG 0

struct timeval start, end;

double x[1001];
long m24;

double serial_calculate()
{

    int i, j, k;
    clock_t ser_msec;

    ser_msec = clock();

    x[n / 2] = -1.0e+10;
    m24 = 0;
    for (k = 1; k < n; k++)
    {
        if (x[k] < x[m24])
            m24 = k;
    }

    ser_msec = (clock() - ser_msec) * 1000000 / CLOCKS_PER_SEC;

    return ser_msec;
}

double paralel_calculate()
{

    int i, j, k;
    clock_t par_msec;

    par_msec = clock();

// #pragma omp parallel for private(i,j,k) collapse(2)
// #pragma omp parallel shared(px, vy, cx) private(k, i, j)
#pragma omp parallel
    {
#pragma omp for
        x[n / 2] = -1.0e+10;
        m24 = 0;
        for (k = 1; k < n; k++)
        {
            if (x[k] < x[m24])
                m24 = k;
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

    printf("average serial time: %.3f microsecond\n", avg_ser_msec);
    printf("average paralel time: %.3f microsecond\n", avg_par_msec);

    printf("speed up: %.3f microsecond\n", avg_ser_msec / avg_par_msec);


    return 0;
}
