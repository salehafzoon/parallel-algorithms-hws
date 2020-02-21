#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>

using namespace std;
#define N 12

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
        else if (y == 1)
        {
            L[0][3] = 0;
            L[1][3] = 1;
            L[2][3] = 0;
        }
        else if (y == 2)
        {
            L[0][3] = 0;
            L[1][3] = 0;
            L[2][3] = 1;
        }
        A[0][3] = L[0][3] / L[0][0];
        for (i = 1; i <= 2; i++)
        {
            sum = 0;
            for (j = i - 1; j >= 0; j--)
            {
                sum = sum + L[i][j] * A[j][3];
            }
            A[i][3] = (L[i][3] - sum) / L[i][i];
        }
        x[2][y] = A[2][3] / A[2][2];
        for (i = 1; i >= 0; i--)
        {
            sum = 0;
            for (j = i + 1; j <= 2; j++)
            {
                sum = sum + A[i][j] * x[j][y];
            }
            x[i][y] = (A[i][3] - sum) / A[i][i];
        }
    }
    cout << "Matrix X" << endl;
    for (int i = 0; i <= 2; i++)
    {
        for (int j = 0; j <= 2; j++)
        {
            cout << x[i][j] << " ";
        }
        cout <<endl;
    }
    return 0;
}
