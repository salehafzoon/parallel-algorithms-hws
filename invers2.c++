#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>

using namespace std;

int main(int argc, char const *argv[])
{
    int e, i, j, k, y;
    float A[3][4] = {{3, 0, 6, 1}, {4, 1, 0, 2}, {7, 8, 0, 3}};
    float x[3][3], c, sum;
    float L[3][4] = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}};
    for (j = 0; j <= 2; j++)
    {
        for (i = 0; i <= 2; i++)
        {
            if (i > j)
            {
                c = A[i][j] / A[j][j];
                L[i][j] = c;
                for (k = 0; k <= 2; k++)
                {
                    A[i][k] = A[i][k] - c * A[j][k];
                }
            }
        }
    }
    for (y = 0; y <= 2; y++)
    {
        if (y == 0)
        {
            L[0][3] = 1;
            L[1][3] = 0;
            L[2][3] = 0;
        }
    
        return 0;
    }
