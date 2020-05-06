
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define n 101
#define count 20
#define DEBUG 0

struct timeval start, end;

double px[101][25];
double cx[101][25];
double vy[25][101];

void print_array(double arr[n][25])
{

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < 10; j++)
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
    
    ser_msec = (clock() - ser_msec) * 1000000 /CLOCKS_PER_SEC; 
    
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
    
    // #pragma omp parallel for private(i,j,k) collapse(2)
    #pragma omp parallel shared(px,vy,cx) private(k,i,j)
    #pragma omp parallel
    {
        #pragma omp for
        for (k = 0; k < 25; k++)
            {
                for (i = 0; i < 25; i++)
                {
                    for (j = 0; j < n; j+=4)
                    {
                        px[j][i] += vy[k][i] * cx[j][k];                
                        px[j+1][i] += vy[k][i] * cx[j+1][k];                
                        px[j+2][i] += vy[k][i] * cx[j+2][k];                
                        px[j+3][i] += vy[k][i] * cx[j+3][k];                
                    }
                    px[n-1][i] += vy[k][i] * cx[n-1][k];                
                    
                }
            }
    }

    par_msec = (clock() - par_msec) * 1000000 /CLOCKS_PER_SEC; 
    
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
	printf("average serial time: %.3f microsecond\n",avg_ser_msec/count);
    printf("average paralel time: %.3f microsecond\n",avg_par_msec/count);
    
    return 0;
}
