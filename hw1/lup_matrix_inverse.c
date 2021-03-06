/*  Copyright 2015 Chandra Shekhar (chandraiitk AT yahoo DOT co DOT in).
    Homepage: https://sites.google.com/site/chandraacads
 * * */

#include <stdio.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>

/* The following header file contains a 128 bit float data type '__float128'*/
//# include <quadmath.h>

typedef float Type;

#define N 100 //Size of square matrix 'A'.

static int LUPdecompose(int size, Type **A, int *P);

static int LUPinverse(int size, int *P, Type **LU, Type **B, Type *X, Type *Y);

void permutation_print(int *P)
{
  printf("\n\nThe LUP decomposition of 'A' is successful.\nPivot:\n");
  for (int i = 1; i <= N; i++)
  {
    for (int j = 1; j <= N; j++)
      printf("\t%d", j == P[i] ? 1 : 0);
    printf("\n");
  }
}
void print_system_data_type_info()
{

  printf("On this machine, size (in bytes) and precision (in number of decimal digits)"
         " of\n\tfloat: %ld and %d,\n\tdouble: %d and %d,",
         sizeof(float), FLT_DIG,
         sizeof(double), DBL_DIG);

  printf("\n\nmatrix size is : %d", N);
}

void initial_matix(Type **A, Type **A1)
{

  for (int i = 1; i <= N; i++)
    for (int j = 1; j <= N; j++)
      A[i][j] = A1[i][j] = sin(i * j * j + i) * 2;

  if (A == NULL || A1 == NULL)
  {
    printf("Memory not allocated.\n");
    exit(0);
  }
  printf("initial matrix done");
}

void print_matrix(Type **A)
{
  printf("\n\nThe to-be-inverted matrix 'A':\n");

  for (int i = 1; i <= N; i++)
  {
    for (int j = 1; j <= N; j++)
      printf("\t%E", (Type)A[i][j]);
    printf("\n");
  }
}

void print_LU_matrix(Type **A)
{

  printf("\nLU (where 1. the diagonal of the matrix belongs to 'U', and 2. the\n"
         "diagonal elements of 'L' are not printed, because they are all 1):\n");
  for (int i = 1; i <= N; i++)
  {
    for (int j = 1; j <= N; j++)
      printf("\t%E", (float)A[i][j]);
    printf("\n");
  }
}

void print_inverting_result(Type **A)
{
  printf("\n\nMatrix inversion successful.\nInverse of A:\n");
  for (int j = 1; j <= N; j++)
  {
    for (int i = 1; i <= N; i++)
      printf("\t%E", (float)A[i][j]);
    printf("\n");
  }
}
void check_inverse_correction(Type **A, Type **A1, Type **I)
{
  int i, j, k;

  for (i = 1; i <= N; i++)
    for (j = 1; j <= N; j++)
      for (I[i][j] = 0, k = 1; k <= N; k++)
        I[i][j] += A[i][k] * A1[k][j];

  printf("\nProduct of the calculated inverse-of-A with A:\n");
  for (int i = 1; i <= N; i++)
  {
    for (int j = 1; j <= N; j++)
      printf("\t%E", (float)I[i][j]);
    printf("\n");
  }
  printf("\n");
}

Type **allocate_2d(Type **pointer)
{
  pointer = (Type **)malloc((N + 1) * sizeof(Type *));
  for (int i = 0; i < N + 1; i++)
    pointer[i] = (Type *)malloc((N + 1) * sizeof(Type));

  return pointer;
}

int main()
{

  // Type A[N + 1][N + 1], A1[N + 1][N + 1];
  Type **A, **A1, **I;
  A = allocate_2d(A);
  A1 = allocate_2d(A1);
  I = allocate_2d(I);

  //permutation matrix
  // int P[N + 1];
  int *P = (int *)malloc((N + 1) * sizeof(int));

  // Type B[N + 1][N + 1], X[N + 1], Y[N + 1]; //Temporaty spaces.
  Type **B, *X, *Y; //Temporaty spaces.
  B = allocate_2d(B);
  X = (Type *)malloc((N + 1) * sizeof(Type));
  Y = (Type *)malloc((N + 1) * sizeof(Type));

  printf("\n\nmatrix size is : %d\n", N);

  /* Defining the to-be-inverted matrix, A. A1 would be used later to test the inverted
 * matrix. */

  initial_matix(A, A1);

  // print_matrix(A);

  /* Performing LUP-decomposition of the matrix 'A'*/
  if (LUPdecompose(N + 1, A, P) < 0)
  {
    printf("\nLUPdecompose faild");
    return -1;
  }

  // permutation_print(P);

  // print_LU_matrix(A);

  /* Inverting based on the LUP ,The inverse is stored in 'A' itself. */
  if (LUPinverse(N + 1, P, A, B, X, Y) < 0)
  {
    printf("\nLUPinverse faild");
    return -1;
  }

  // print_inverting_result(A);

  /* check correctness of result by calculate A.A1 ,result must be I */

  // check_inverse_correction(A, A1, I);

  printf("\n\nsuccessfully finished\n");

  free(A);
  free(A1);
  free(I);
  free(P);
  free(B);
  free(X);
  free(Y);

  return 0;
}

/* This function decomposes the matrix 'A' into L, U, and P. If successful,
 * the L and the U are stored in 'A', and information about the pivot in 'P'. */
static int LUPdecompose(int size, Type **A, int *P)
{
  int i, j, k, kd = 0, T;
  Type p, t;

  /* Finding the pivot of the LUP decomposition. */
  for (i = 1; i < size; i++)
    P[i] = i; //Initializing.

  for (k = 1; k < size - 1; k++)
  {
    p = 0;
    for (i = k; i < size; i++)
    {
      t = A[i][k];
      if (t < 0)
        t *= -1; //Abosolute value of 't'.
      if (t > p)
      {
        p = t;
        kd = i;
      }
    }

    if (p == 0)
    {
      printf("\nLUPdecompose(): ERROR: A singular matrix is supplied.\n"
             "\tRefusing to proceed any further.\n");
      return -1;
    }

    /* Exchanging the rows according to the pivot determined above. */
    T = P[kd];
    P[kd] = P[k];
    P[k] = T;
    for (i = 1; i < size; i++)
    {
      t = A[kd][i];
      A[kd][i] = A[k][i];
      A[k][i] = t;
    }

    for (i = k + 1; i < size; i++) //Performing substraction to decompose A as LU.
    {
      A[i][k] = A[i][k] / A[k][k];
      for (j = k + 1; j < size; j++)
        A[i][j] -= A[i][k] * A[k][j];
    }
  } //Now, 'A' contains the L (without the diagonal elements, which are all 1)
  //and the U.

  printf("\nLUPdecompose successfully done");
  return 0;
}

/* inverse of the LUP decomposed matrix 'LU' and pivot 'P'.
 * The inverse is returned as 'LU'.
 * 'B', X', and 'Y' are used as temporary spaces. */
static int LUPinverse(int size, int *P, Type **LU, Type **B, Type *X, Type *Y)
{
  int i, j, n, m;
  Type t;

  //Initializing X and Y.
  for (n = 1; n < size; n++)
    X[n] = Y[n] = 0;

  /* Solving LUX = Pe, in order to calculate the inverse of 'A'. Here, 'e' is a column
  * vector of the identity matrix of size 'size-1'. Solving for all 'e'. */
  for (i = 1; i < size; i++)
  {
    //Storing elements of the i-th column of the identity matrix in i-th row of 'B'.
    for (j = 1; j < size; j++)
      B[i][j] = 0;

    B[i][i] = 1;

    //Solving Ly = Pb.
    for (n = 1; n < size; n++)
    {
      t = 0;
      for (m = 1; m <= n - 1; m++)
        t += LU[n][m] * Y[m];
      Y[n] = B[i][P[n]] - t;
    }

    //Solving Ux = y.
    for (n = size - 1; n >= 1; n--)
    {
      t = 0;
      for (m = n + 1; m < size; m++)
        t += LU[n][m] * X[m];
      X[n] = (Y[n] - t) / LU[n][n];
    } //Now, X contains the solution.

    for (j = 1; j < size; j++)
      B[i][j] = X[j]; //Copying 'X' into the same row of 'B'.
  }                   //Now, 'B' the transpose of the inverse of 'A'.

  /* Copying transpose of 'B' into 'LU', which would the inverse of 'A'. */
  for (i = 1; i < size; i++)
    for (j = 1; j < size; j++)
      LU[i][j] = B[j][i];

  printf("\nLUPinverse successfully done");
  return 0;
}
