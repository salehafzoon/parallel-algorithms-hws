
#include <stdio.h>
#include <stdlib.h>
#include<time.h>
#include<math.h>
#include <omp.h>

#define N       100



     double Px[101][25];
     double Cx[101][25];
     double Vy[25][101];

double serial_calculate(){

    int i,j,k;
    double a[N][10], b[N][10], c[N][10];
    clock_t ser_msec;

    // initializing
    for (int i = 0; i < N ;i++) {
        for (int  j= 0; j < 10 ;j++) {
            // a[f][f] = (rand() % 20);
            // b[f][f]= (rand() % 20);
            a[i][j] = 2;
            b[i][j] = 2;
            
            c[i][j] = 0;
        }
    }
    
    ser_msec = clock();
    
    for ( k=0 ; k<10 ; k++ ){
                for ( i=0 ; i<10 ; i++ )
                {
                    for ( j=0 ; j<N ; j++ )
                    {
                        c[j][i] += a[k][i] * b[j][k];
                    }
                }
            }
    
    for (size_t i = 0; i < N; i++)
    {
        for (size_t j = 0; j < 10; j++)
        {
            printf("%.2f\t",c[i][j]);
        }
        printf("\n");
    }
    
    ser_msec = (double)(clock() - ser_msec) * 1000000 /CLOCKS_PER_SEC;
    
    return ser_msec;
}


double paralel_calculate(){
    
    int i,j,k;
    double a[N][25], b[N][25], c[N][25];
    clock_t par_msec;

    // // initializing
    // for (int f = 0; f < N ;f++) {
    //     for (int f = 0; f < 25 ;f++) {
    //         a[f][f] = (rand() % 20);
    //         b[f][f]= (rand() % 20);
    //         c[f][f] = 0;
    //     }
    // }
    
    par_msec = clock();
    int tid;
    omp_set_dynamic(0);     // Explicitly disable dynamic teams
    omp_set_num_threads(8);
    
    // #pragma omp parallel for private(i,j,k) collapse(2)
    #pragma omp parallel
    for ( k=0 ; k<25 ; k++ ){
                for ( i=0 ; i<25 ; i++ )
                {
                    for ( j=0 ; j<N ; j+=4 )
                    {
                        c[j][i] += a[k][i] * b[j][k];
                        c[j+1][i] += a[k][i] * b[j+1][k];
                        c[j+2][i] += a[k][i] * b[j+2][k];
                        c[j+3][i] += a[k][i] * b[j+3][k];
                        
                    }
                }

                // tid = omp_get_thread_num();
                // printf("tid:%d",tid);
            }
    
    par_msec = (double)(clock() - par_msec) * 1000000 /CLOCKS_PER_SEC;
    
    return par_msec;
}

int main (int argc, char *argv[]) 
{

    double ser_msec = 0,par_msec = 0;
    
    
    ser_msec = serial_calculate();

    printf("%f serial microsecond\n",ser_msec);

    
    // par_msec = paralel_calculate();
    // printf("%f paralel microsecond\n",par_msec);

    return 0;

}