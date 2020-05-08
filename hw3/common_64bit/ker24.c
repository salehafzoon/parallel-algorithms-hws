
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define n 1001 //dosen't change
#define count 20
#define DEBUG 0
#define THEADS 4

struct timeval start, end;

double x[n];
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
    int nthreads, tid, chunk;
    clock_t par_msec;

    par_msec = clock();

    omp_set_num_threads(THEADS);
#pragma omp parallel private(k)
    {
        x[n / 2] = -1.0e+10;
        m24 = 0;

#pragma omp for private(k)
        for (k = 1; k < n; k += 4)
        {
            if (x[k] < x[m24])
                m24 = k;
            if (x[k + 1] < x[m24])
                m24 = k + 1;
            if (x[k + 2] < x[m24])
                m24 = k + 2;
            if (x[k + 3] < x[m24])
                m24 = k + 3;
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
