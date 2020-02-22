// CPP Program to decompose a matrix into
// lower and upper traingular matrix
#include <iostream>
#include <math.h>

using namespace std;
#define N 3

// const int MAX = 20;

template <typename Type>
void init_matirx(Type mat[][N])
{
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			mat[i][j] = 0;
		}
	}
}

template <typename Type>
void luDecomposition(Type mat[][N], Type lower[][N], Type upper[][N])
{
	int exchange = 0;
	// Decomposing matrix into Upper and Lower
	// triangular matrix
	for (int i = 0; i < N; i++)
	{

		// Upper Triangular
		for (int k = i; k < N; k++)
		{

			// Summation of L(i, j) * U(j, k)
			int sum = 0;
			for (int j = 0; j < i; j++)
				sum += (lower[i][j] * upper[j][k]);

			// Evaluating U(i, k)
			upper[i][k] = mat[i][k] - sum;
		}

		// Lower Triangular
		for (int k = i; k < N; k++)
		{
			if (i == k)
				lower[i][i] = 1; // Diagonal as 1
			else
			{

				// Summation of L(k, j) * U(j, i)
				int sum = 0;
				for (int j = 0; j < i; j++)
					sum += (lower[k][j] * upper[j][i]);

				// Evaluating L(k, i)
				lower[k][i] = (mat[k][i] - sum) / upper[i][i];
			}
		}
	}

	// Displaying the result :
	for (int i = 0; i < N; i++)
	{
		// Lower
		for (int j = 0; j < N; j++)
			cout << lower[i][j] << "\t";
		cout << "\t";

		// Upper
		for (int j = 0; j < N; j++)
			cout << upper[i][j] << "\t";
		cout << endl;
	}
}

// returns an NxN identity matrix
template <typename Type>
void identity(Type id[N][N])
{

	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			id[i][j] = (Type)0;
		}
		id[i][i] = (Type)1;
	}
}

template <typename Type>
int det(Type mat[][N], Type lower[][N], Type upper[][N], Type p[][N])
{ //returns the determinant of square matrix A

	luDecomposition(mat, lower, upper);
	int D = 1;

	// calculate determinant of u
	int u_det = 1;
	for (int i = 0; i < N; i++)
	{
		u_det *= upper[i][i];
	}

	// calculate determinant as det(p) * det(l)=1 * det(u)
	D = D * pow(-1, N + 1) * 1 * u_det;

	cout << "determinant:" << D << endl;

	return D;
}

// Driver code
int main()
{
	// int mat[][N] = { { 2, -1, -2 },
	// 				{ -4, 6, 3 },
	// 				{ -4, -2, 8 } };

	int mat[][N] = {{1,12, 	3},
					{5, -6, 4},
					{8, 1, -7}};

	int lower[N][N], upper[N][N], p[N][N];
	init_matirx(lower);
	init_matirx(upper);
	identity(p);

	det(mat, lower, upper, p);

	return 0;
}
