/******************************************************************************
* FILE: omp_workshare1.c
* DESCRIPTION:
*   OpenMP Example - Loop Work-sharing - C/C++ Version
*   In this example, the iterations of a loop are scheduled dynamically
*   across the team of threads.  A thread will perform CHUNK iterations
*   at a time before being scheduled for the next CHUNK of work.
* AUTHOR: Blaise Barney  5/99
* LAST REVISED: 04/06/05
******************************************************************************/
#include "omp.h"
#include <stdio.h>
#include <stdlib.h>
#define CHUNKSIZE   20
#define N       100

int main (int argc, char *argv[]) 
{
int nthreads, tid, i, chunk;
float a[N], b[N], c[N];

/* Some initializations */
for (i=0; i < N; i++)
  a[i] = b[i] = i * 1.0;
chunk = CHUNKSIZE;

// omp_set_num_threads(4);
// #pragma omp parallel

// #pragma omp parallel num_threads(4)

#pragma omp parallel shared(a,b,c,nthreads,chunk) private(i,tid)
  {
  tid = omp_get_thread_num();
  if (tid == 0)
    {
    nthreads = omp_get_num_threads();
    printf("Number of threads = %d\n", nthreads);
    }
  printf("Thread %d starting...\n",tid);

  #pragma omp for schedule(dynamic,chunk)
  for (i=0; i<N; i++)
    {
    c[i] = a[i] + b[i];
    printf("Thread %d: c[%d]= %f\n",tid,i,c[i]);
    }

  }  /* end of parallel section */

}



/*

wiki pedia
It consists of 24 do loops, some of which can be vectorized, and some of which cannot. 
 
 matrix multipli

    The three loops in iterative matrix multiplication can be arbitrarily swapped with each 
    other without an effect on correctness or asymptotic running time
    
    incurs a cache miss when accessing an element of B.
    Θ(n3) cache misses 

    The optimal variant of the iterative algorithm for A and B in row-major layout is a tiled version,
     where the matrix is implicitly divided into square tiles of size √M by √M:
 */


//  #pragma omp parallel for private(i,j,k,ii,jj,kk, compute)  
      // firstprivate(we_need, gimmie, dim,old,BLOCK_SIZE) reduction(+:out)


//#pragma omp parallel for private(jj,kk,i,j,k,sum) collapse(3)