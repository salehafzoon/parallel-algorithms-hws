#include<stdio.h>
#include<math.h>
#include <time.h>
#define N 100

float determinant(float [][25], float);
void cofactor(float [][25], float);
void transpose(float [][25], float [][25], float);
int determinantOfMatrix(int matrix[25][25], int size);
void element_cofactor(int matrix[N][N], int temp[N][N], int p, int q, int size);
void print_matix(){

}
int main()
{
    float b[25][25], size, d;
    // int i, j;
    printf("Enter the order of the Matrix : ");
    scanf("%f", &size);
    // for (i = 0;i < size; i++)
    // {
    //     for (j = 0;j < size; j++)
    //    {
    //         a[i][j] = rand()%10 +0.0f;
    //     }
    // }

    // 3 5 2 1 5  8 3 9 2
    // float a[25][25]= {
    //   {3,5,2}, 
    //   {1,5,8}, 
    //   {3,9,2},}; 

    int a[25][25] = {{1, 0, 2, -1}, 
                     {3, 0, 0, 5}, 
                     {2, 1, 4, -3}, 
                     {1, 0, 5, 0} 
                    }; 

    clock_t time = clock();
  
    d = determinant(a, size);
    printf("%f\n",d);

    // d = determinantOfMatrix(a,size);
    // printf("%d\n",d);
    
    if (d == 0)
        printf("\nInverse of Entered Matrix is not possible\n");
    else
    cofactor(a, size);

    time = clock() - time; 
    double time_taken = ((double)time)/CLOCKS_PER_SEC; // in seconds 
  
    printf("time of execution : %f seconds\n", time_taken); 
    
}
 
 void element_cofactor(int matrix[N][N], int temp[N][N], int p, int q, int size) 
{ 
    int i = 0, j = 0; 
  
    for (int row = 0; row < size; row++) 
    { 
        for (int col = 0; col < size; col++) 
        { 
            //store only those element which aren't in given row and column 
            if (row != p && col != q) 
            { 
                temp[i][j++] = matrix[row][col]; 
  
                //Row is filled, go to next row
                if (j == size - 1) 
                { 
                    j = 0; 
                    i++; 
                } 
            } 
        } 
    } 
} 

void getCofactor2(int mat[N][N], int temp[N][N], int p, int q, int n) 
{ 
    int i = 0, j = 0; 
  
    // Looping for each element of the matrix 
    for (int row = 0; row < n; row++) 
    { 
        for (int col = 0; col < n; col++) 
        { 
            //  Copying into temporary matrix only those element 
            //  which are not in given row and column 
            if (row != p && col != q) 
            { 
                temp[i][j++] = mat[row][col]; 
  
                // Row is filled, so increase row index and 
                // reset col index 
                if (j == n - 1) 
                { 
                    j = 0; 
                    i++; 
                } 
            } 
        } 
    } 
} 

int determinantOfMatrix2(int mat[N][N], int n) 
{ 
    int D = 0; // Initialize result 
  
    //  Base case : if matrix contains single element 
    if (n == 1) 
        return mat[0][0]; 
  
    int temp[N][N]; // To store cofactors 
  
    int sign = 1;  // To store sign multiplier 
  
     // Iterate for each element of first row 
    for (int f = 0; f < n; f++) 
    { 
        // Getting Cofactor of mat[0][f] 
        getCofactor2(mat, temp, 0, f, n); 
        D += sign * mat[0][f] * determinantOfMatrix2(temp, n - 1); 
  
        // terms are to be added with alternate sign 
        sign = -sign; 
    } 
  
    return D; 
} 

int determinantOfMatrix(int matrix[25][25], int size) 
{ 
    int determinant = 0;
  
    //  Base case(single element)
    if (size == 1) 
        return matrix[0][0]; 
  
    int cofactor_stores[25][25]; // To store cofactors 
  
    int sign = 1;  // To store sign multiplier 
  
     // Iterate for each element of first row 
    for (int i = 0; i < size; i++) 
    { 
        // Getting Cofactor of mat[0][i] 
        element_cofactor(matrix, cofactor_stores, 0, i, size); 
        determinant += sign * matrix[0][i] * determinantOfMatrix(cofactor_stores, i - 1); 
  
        sign = -sign; 
    } 
  
    return determinant; 
} 

float determinant(float a[25][25], float k)
{
  float s = 1, det = 0, b[25][25];
  int i, j, m, n, c;
  if (k == 1)
    {
     return (a[0][0]);
    }
  else
    {
     det = 0;
     for (c = 0; c < k; c++)
       {
        m = 0;
        n = 0;
        for (i = 0;i < k; i++)
          {
            for (j = 0 ;j < k; j++)
              {
                b[i][j] = 0;
                if (i != 0 && j != c)
                 {
                   b[m][n] = a[i][j];
                   if (n < (k - 2))
                    n++;
                   else
                    {
                     n = 0;
                     m++;
                     }
                   }
               }
             }
          det = det + s * (a[0][c] * determinant(b, k - 1));
          s = -1 * s;
          }
    }
 
    return (det);
}
 
void cofactor(float num[25][25], float f)
{
 float b[25][25], fac[25][25];
 int p, q, m, n, i, j;
 for (q = 0;q < f; q++)
 {
   for (p = 0;p < f; p++)
    {
     m = 0;
     n = 0;
     for (i = 0;i < f; i++)
     {
       for (j = 0;j < f; j++)
        {
          if (i != q && j != p)
          {
            b[m][n] = num[i][j];
            if (n < (f - 2))
             n++;
            else
             {
               n = 0;
               m++;
               }
            }
        }
      }
      fac[q][p] = pow(-1, q + p) * determinant(b, f - 1);
    }
  }
  transpose(num, fac, f);
}
/*Finding transpose of matrix*/ 
void transpose(float num[25][25], float fac[25][25], float r)
{
  int i, j;
  float b[25][25], inverse[25][25], d;
 
  for (i = 0;i < r; i++)
    {
     for (j = 0;j < r; j++)
       {
         b[i][j] = fac[j][i];
        }
    }
  d = determinant(num, r);
  for (i = 0;i < r; i++)
    {
     for (j = 0;j < r; j++)
       {
        inverse[i][j] = b[i][j] / d;
        }
    }
   printf("\n\n\nThe inverse of matrix is : \n");
 
   for (i = 0;i < r; i++)
    {
     for (j = 0;j < r; j++)
       {
         printf("\t%f", inverse[i][j]);
        }
    printf("\n");
     }
}