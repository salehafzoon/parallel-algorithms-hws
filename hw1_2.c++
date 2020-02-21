#include <iostream>
#include <time.h>

using namespace std;
#define N 11

template <typename Type>
void get_cofactor(Type M[N][N], Type t[N][N], int p, int q, int n)
{
    int i = 0, j = 0;

    for (int r = 0; r < n; r++)
    {
        //Copy only those elements which are not in given row r and column c:
        for (int c = 0; c < n; c++)
        {
            if (r != p && c != q)
            {
                t[i][j++] = M[r][c];
                //If row is filled increase r index and reset c index
                if (j == n - 1)
                {
                    j = 0;
                    i++;
                }
            }
        }
    }
}

template <typename Type>
float determinant(Type M[N][N], int n)
{
    float D = 0;
    if (n == 1)
        return M[0][0];
    Type t[N][N]; //store cofactors
    int s = 1;   //store sign multiplier //
                 //To Iterate each element of first row
    for (int f = 0; f < n; f++)
    {
        //For Getting Cofactor of M[0][f] do
        get_cofactor(M, t, 0, f, n);
        D += s * M[0][f] * determinant(t, n - 1);
        s = -s;
    }
    return D;
}

//to find adjoint matrix
template <typename Type>
void adjoint(Type M[N][N], Type adj[N][N])
{
    if (N == 1)
    {
        adj[0][0] = 1;
        return;
    }
    int s = 1;
    Type t[N][N];

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            //To get cofactor of M[i][j]
            get_cofactor(M, t, i, j, N);
            s = ((i + j) % 2 == 0) ? 1 : -1;           //change sign acording to positive
            adj[j][i] = (s) * (determinant(t, N - 1)); //Interchange rows and columns to get the transpose of the cofactor matrix
        }
    }
}

template <typename Type>
bool inverse(Type M[N][N], Type inv[N][N])
{
    int det = determinant(M, N);
    if (det == 0)
    {
        cout << "can't find its inverse";
        return false;
    }
    Type adj[N][N];
    adjoint(M, adj);

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            inv[i][j] = adj[i][j] / det;
    return true;
}

//print the matrix.
template <typename Type>
void print_matrix(Type A[N][N])
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
            cout << A[i][j] << " ";
        cout << endl;
    }
}
template <class T>
T generateRandomNumber(T startRange, T endRange)
{
    return startRange + T(rand()) / T(RAND_MAX) * (endRange - startRange);
}

int main()
{
    float matrix[N][N];
    float inv[N][N];

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
            matrix[i][j] = (float) rand()/100;
    }

    cout << "Input matrix is :\n";
    print_matrix(matrix);

    clock_t time = clock();

    cout << "\nThe Inverse is :\n";
    if (inverse(matrix, inv))
        print_matrix(inv);

    time = clock() - time;
    int seconds = ((int)time) / CLOCKS_PER_SEC; // in seconds
    int minutes = seconds / 60;
    int hours = minutes / 60;
    seconds = seconds % 60;

    printf("\n\ntime of execution : %d hours / %d minutes / %d seconds\n",
           int(hours), int(minutes % 60), int(seconds % 60));

    return 0;
}

//used linked for code :
// https://www.tutorialspoint.com/
// cplusplus-program-to-find-inverse-of-a-graph-matrix