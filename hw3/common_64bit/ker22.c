#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define n 101
    
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
    
    ser_msec = (double)(clock() - ser_msec) * 1000000 /CLOCKS_PER_SEC; 
    printf("\n serial time: %d microsecond\n",ser_msec);
    
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
    
        #pragma omp for
        for (k = 0; k < n; k++)
        {
            y[k] = u[k] / v[k];
            w[k] = x[k] / (exp(y[k]) - 1.0);
        }
    }

    par_msec = (double)(clock() - par_msec) * 1000000 /CLOCKS_PER_SEC; 
    
    printf("\n paralel time: %d microsecond\n",par_msec);
    

    return par_msec; 
}

int main(int argc, char *argv[])
{

    double ser_msec = 0, par_msec = 0;

    ser_msec = serial_calculate();
	// printf("\n serial time: %d microsecond\n",ser_msec);
    
    par_msec = paralel_calculate();
    // printf("\n paralel time: %d microsecond\n",par_msec);
    	
    return 0;
}
