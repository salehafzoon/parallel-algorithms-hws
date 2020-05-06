#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define n 101
#define count 20     
#define DEBUG 0

struct timeval start, end;

double u[1001];
double v[1001];
double w[1001];
double x[1001];
double y[1001];


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
    
    ser_msec = (clock() - ser_msec) * 1000000 /CLOCKS_PER_SEC; 

    return ser_msec;
}

double paralel_calculate()
{

    int i, j, k;
    clock_t par_msec;

    
    par_msec = clock();
    
    // #pragma omp parallel for private(i,j,k) collapse(2)
    #pragma omp parallel
    {
        double expmax = 20.0;
        u[n - 1] = 0.99 * expmax * v[n - 1];
    
        #pragma omp for private(y,u,v,w,x)
        for (k = 0; k < n; k+=2)
        {
            y[k] = u[k] / v[k];
            w[k] = x[k] / (exp(y[k]) - 1.0);

            y[k+1] = u[k+1] / v[k+1];
            w[k+1] = x[k+1] / (exp(y[k+1]) - 1.0);

        }
        y[n-1] = u[n-1] / v[n-1];
        w[n-1] = x[n-1] / (exp(y[n-1]) - 1.0);
    }

    par_msec = (clock() - par_msec) * 1000000 /CLOCKS_PER_SEC; 

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
	printf("average serial time: %.3f microsecond\n",avg_ser_msec/count);
    printf("average paralel time: %.3f microsecond\n",avg_par_msec/count);
    
    return 0;
}
