/* Copyright 2015 Chandra Shekhar (chandraiitk AT yahoo DOT co DOT in).  
   Homepage: https://sites.google.com/site/chandraacads  
  * * */  
   
   
 /* This program is free software: you can redistribute it and/or modify  
   it under the terms of the GNU General Public License as published by  
   the Free Software Foundation, either version 3 of the License, or  
   any later version.  
   
   This program is distributed in the hope that it will be useful,  
   but WITHOUT ANY WARRANTY; without even the implied warranty of  
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the  
   GNU General Public License for more details.  
   
   You should have received a copy of the GNU General Public License  
   along with this program. If not, see <http://www.gnu.org/licenses/>.  
  * * */  
   
   
 /* This program computes inverse of a square matrix, based on LUP decomposition.  
  *   
  * Tested with GCC-4.8.3 on 64 bit Linux (Fedora-20).  
  *   
  * Compilation:     "gcc -O2 Matrix_inverse_LUP.c -o Mat_inv_LUP.exe -lm -Wall"  
  * Execution:     "./Mat_inv_LUP.exe"  
  * * */  
   
   
 # include <stdio.h>  
 # include <float.h>  
 # include <math.h>  
   
 /* The following header file contains a 128 bit float data type '__float128',  
  * used for high-precision calculations. This data-type is available only in the  
  * GCC version 4.6 or later. In order to enable high-precision calculations,  
  * uncomment the following line, followed by type-defining 'sankhya' appropriately  
  * and including an additional '-lquadmath' flag during the compilation.  
  * * */  
 //# include <quadmath.h>  
   
   
 /* The following type-define can be used to change the precision of the calculation.  
  * Possible values:  
  *  'float'          - for least precision  
  *  'double'          - for medium precision  
  *  '__float128'     - for highest precision  
  * * */  
 typedef      double      sankhya;  
   
   
 # define     N     5 //Size of the to-be-inverted N x N square matrix 'A'.  
   
   
 /* This function performs LUP decomposition of the to-be-inverted matrix 'A'. It is  
  * defined after the function 'main()'.  
  * * */  
 static int LUPdecompose(int size, sankhya A[size][size], int P[size]);  
   
 /* This function calculates inverse of the matrix A. It accepts the LUP decomposed  
  * matrix through 'LU' and the corresponding pivot through 'P'. The inverse is  
  * returned through 'LU' itself. The spaces 'B', 'X', and 'Y' are used temporary,  
  * merely to facilitate the computation. This function is defined after the function  
  * 'LUPdecompose()'.  
  * * */  
 static int LUPinverse(int size, int P[size], sankhya LU[size][size],\  
            sankhya B[size][size], sankhya X[size], sankhya Y[size]);  
   
   
 int main()  
  {  
  int i, j, k; //Running indices.  
   
 /* 'A' is the to-be-inverted matrix. A1 is its copy, which is used to calculate  
  * 'I = inverse(A).A1'. Ideally, 'I' should be a perfect identity matrix. 'I' is  
  * used to check the quality of the calculated inverse. The quality increases as  
  * we use 'float', 'double', and '__float128', respectively. */  
  sankhya A[N+1][N+1], A1[N+1][N+1], I[N+1][N+1];  
   
 /* Its i-th row shows the position of '1' in the i-th row of the pivot that is used  
  * when performing the LUP decomposition of A. The rest of the elements in that row of  
  * the pivot would be zero. In this program, we call this array 'P' a 'permutation'. */  
  int P[N+1];  
   
  sankhya B[N+1][N+1], X[N+1], Y[N+1]; //Temporaty spaces.  
   
 /* Copyright notice. */  
  printf("\nCopyright 2015 Chandra Shekhar (chandraiitk AT yahoo DOT co DOT in).\n"\  
      "Homepage: https://sites.google.com/site/chandraacads\n\n");  
        
   
 /* Printing information on size and precision of various data types. */  
  printf("On this machine, size (in bytes) and precision (in number of decimal digits)"\  
      " of\n\tfloat: %ld and %d,\n\tdouble: %ld and %d,", sizeof(float), FLT_DIG,\  
      sizeof(double), DBL_DIG);  
   
 # ifdef QUADMATH_H  
  printf("\n\t__float128: %ld and %d,", sizeof(__float128), FLT128_DIG);  
 # endif  
  printf(" respectively.");  
   
 /* Defining the to-be-inverted matrix, A. A1 would be used later to test the inverted  
  * matrix. */  
  for(i = 1; i <= N; i++) for(j = 1; j <= N; j++)  
   A[i][j] = A1[i][j] = sin(i*j*j+i)*2;  
   
  printf("\n\nThe to-be-inverted matrix 'A':\n");  
  for(i = 1; i <= N; i++)  
    {  
    for(j = 1; j <= N; j++) printf("\t%E", (float)A[i][j]);  
    printf("\n");  
    }  
   
 /* Performing LUP-decomposition of the matrix 'A'. If successful, the 'U' is stored in  
  * its upper diagonal, and the 'L' is stored in the remaining traigular space. Note that  
  * all the diagonal elements of 'L' are 1, which are not stored. */  
  if(LUPdecompose(N+1, A, P) < 0) return -1;  
  printf("\n\nThe LUP decomposition of 'A' is successful.\nPivot:\n");  
  for(i = 1; i <= N; i++)  
    {  
        for(j = 1; j <= N; j++) printf("\t%d", j == P[i] ? 1:0);  
        printf("\n");  
    }  
   
  printf("\nLU (where 1. the diagonal of the matrix belongs to 'U', and 2. the\n"\  
      "diagonal elements of 'L' are not printed, because they are all 1):\n");  
  for(i = 1; i <= N; i++)  
    {  
    for(j = 1; j <= N; j++) printf("\t%E", (float)A[i][j]);  
    printf("\n");  
    }  
   
 /* Inverting the matrix based on the LUP decomposed A. The inverse is returned through  
  * the matrix 'A' itself. */  
   if(LUPinverse(N+1, P, A, B, X, Y) < 0) return -1;  
   printf("\n\nMatrix inversion successful.\nInverse of A:\n");  
   for(j = 1; j <= N; j++)  
    {  
     for(i = 1; i <= N; i++) printf("\t%E", (float)A[i][j]);  
     printf("\n");  
    }  
   
 /* Multiplying the inverse-of-A (stored in A) with A (stored in A1). The product is  
  * stored in 'I'. Ideally, 'I' should be a perfect identity matrix. */  
  for(i=1; i <= N; i++) for(j = 1; j <= N; j++)  
   for(I[i][j] = 0, k = 1; k <= N; k++) I[i][j] += A[i][k]*A1[k][j];  
   
  printf("\nProduct of the calculated inverse-of-A with A:\n");  
  for(i = 1; i <= N; i++)  
    {  
    for(j = 1; j <= N; j++) printf("\t%E", (float)I[i][j]);  
    printf("\n");  
    }  
  printf("\n");  
   
  return 0;  
  }  
   
      
   
 /* This function decomposes the matrix 'A' into L, U, and P. If successful,  
  * the L and the U are stored in 'A', and information about the pivot in 'P'.  
  * The diagonal elements of 'L' are all 1, and therefore they are not stored. */  
 static int LUPdecompose(int size, sankhya A[size][size], int P[size])  
   {  
   int i, j, k, kd = 0, T;  
   sankhya p, t;  
   
  /* Finding the pivot of the LUP decomposition. */  
   for(i=1; i<size; i++) P[i] = i; //Initializing.  
   
   for(k=1; k<size-1; k++)  
     {  
     p = 0;  
     for(i=k; i<size; i++)  
       {  
       t = A[i][k];  
       if(t < 0) t *= -1; //Abosolute value of 't'.  
       if(t > p)  
         {  
          p = t;  
          kd = i;  
         }  
       }  
   
     if(p == 0)  
       {  
       printf("\nLUPdecompose(): ERROR: A singular matrix is supplied.\n"\  
           "\tRefusing to proceed any further.\n");  
       return -1;  
       }  
   
    /* Exchanging the rows according to the pivot determined above. */  
     T = P[kd];  
     P[kd] = P[k];  
     P[k] = T;  
     for(i=1; i<size; i++)  
       {  
        t = A[kd][i];  
        A[kd][i] = A[k][i];  
        A[k][i] = t;  
       }  
   
     for(i=k+1; i<size; i++) //Performing substraction to decompose A as LU.  
       {  
        A[i][k] = A[i][k]/A[k][k];  
        for(j=k+1; j<size; j++) A[i][j] -= A[i][k]*A[k][j];  
       }  
     } //Now, 'A' contains the L (without the diagonal elements, which are all 1)  
      //and the U.  
   
   return 0;  
   }  
   
      
   
 /* This function calculates the inverse of the LUP decomposed matrix 'LU' and pivoting  
  * information stored in 'P'. The inverse is returned through the matrix 'LU' itselt.  
  * 'B', X', and 'Y' are used as temporary spaces. */  
 static int LUPinverse(int size, int P[size], sankhya LU[size][size],\  
            sankhya B[size][size], sankhya X[size], sankhya Y[size])  
   {  
   int i, j, n, m;  
   sankhya t;  
   
  //Initializing X and Y.  
   for(n=1; n<size; n++) X[n] = Y[n] = 0;  
   
  /* Solving LUX = Pe, in order to calculate the inverse of 'A'. Here, 'e' is a column  
  * vector of the identity matrix of size 'size-1'. Solving for all 'e'. */  
   for(i=1; i<size; i++)  
    {  
   //Storing elements of the i-th column of the identity matrix in i-th row of 'B'.  
    for(j = 1; j<size; j++) B[i][j] = 0;  
    B[i][i] = 1;  
   
   //Solving Ly = Pb.  
    for(n=1; n<size; n++)  
     {  
     t = 0;  
     for(m=1; m<=n-1; m++) t += LU[n][m]*Y[m];  
     Y[n] = B[i][P[n]]-t;  
     }  
   
   //Solving Ux = y.  
    for(n=size-1; n>=1; n--)  
     {  
     t = 0;  
     for(m = n+1; m < size; m++) t += LU[n][m]*X[m];  
     X[n] = (Y[n]-t)/LU[n][n];  
     }//Now, X contains the solution.  
   
    for(j = 1; j<size; j++) B[i][j] = X[j]; //Copying 'X' into the same row of 'B'.  
    } //Now, 'B' the transpose of the inverse of 'A'.  
   
  /* Copying transpose of 'B' into 'LU', which would the inverse of 'A'. */  
   for(i=1; i<size; i++) for(j=1; j<size; j++) LU[i][j] = B[j][i];  
   
   return 0;  
   }  
   