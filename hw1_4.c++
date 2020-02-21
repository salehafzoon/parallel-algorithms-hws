#include <iostream>
#include <cstdio>
#include <math.h>  

using namespace std;
#define N 12

void lu(float[][10], float[][10], float[][10], int n);
void output(float[][10], int);
int det(float a[][10], int n);
void identity(float id[][10], int n);

int main(int argc, char **argv)
{
    float a[10][10]={{1,3,5},{2,4,7},{1,1,0}};
    float l[10][10], u[10][10];
    int n = 0, i = 0, j = 0;
    cout << "Enter size of 2d array(Square matrix) : ";
    cin >> n;

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
            a[i][j] = rand() % 3;
    }

    n=3;

    lu(a, l, u, n);
    cout << "\nL Decomposition\n\n";
    output(l, n);
    cout << "\nU Decomposition\n\n";
    output(u, n);
    cout<<"determinant:"<<det(a,n)<<endl;

    return 0;
}

int det(float a[][10], int n)
{ //returns the determinant of square matrix A

    float l[10][10];
    float u[10][10];
    float p[10][10];

    lu(a, l, u, n);
    identity(p,n);
    int D = 1;

    // calculate determinant of u
    int u_det = 1;
    for (int i =0;i<n;i++){
        u_det *= u[i][i];
    }

    // calculate determinant as det(p) * det(l)=1 * det(u)
    D = D * pow(-1,n) * 1 * u_det;

    return D;
}

// returns an NxN identity matrix
void identity(float id[][10], int n)
{

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            id[i][j] = 0;
        }
        id[i][i] = 1;
    }
}

void lu(float a[][10], float l[][10], float u[][10], int n)
{
    int i = 0, j = 0, k = 0;
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            if (j < i)
                l[j][i] = 0;
            else
            {
                l[j][i] = a[j][i];
                for (k = 0; k < i; k++)
                {
                    l[j][i] = l[j][i] - l[j][k] * u[k][i];
                }
            }
        }
        for (j = 0; j < n; j++)
        {
            if (j < i)
                u[i][j] = 0;
            else if (j == i)
                u[i][j] = 1;
            else
            {
                u[i][j] = a[i][j] / l[i][i];
                for (k = 0; k < i; k++)
                {
                    u[i][j] = u[i][j] - ((l[i][k] * u[k][j]) / l[i][i]);
                }
            }
        }
    }
}
void output(float x[][10], int n)
{
    int i = 0, j = 0;
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            printf("%f ", x[i][j]);
        }
        cout << "\n";
    }
}