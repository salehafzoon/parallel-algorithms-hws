// CPP Program to decompose a matrix into 
// lower and upper traingular matrix 
#include <iostream>
#include <math.h> 
using namespace std; 

const int MAX = 20; 

void init_matirx(int mat[][20]){
    for(int i = 0;i<20;i++){
        for(int j = 0;j<20;j++){
            mat[i][j] = 0;
        }
    }
}
void luDecomposition(int mat[][MAX],int lower[20][20],int upper[20][20], int n) 
{ 
	
	// Decomposing matrix into Upper and Lower 
	// triangular matrix 
	for (int i = 0; i < n; i++) { 

		// Upper Triangular 
		for (int k = i; k < n; k++) { 

			// Summation of L(i, j) * U(j, k) 
			int sum = 0; 
			for (int j = 0; j < i; j++) 
				sum += (lower[i][j] * upper[j][k]); 

			// Evaluating U(i, k) 
			upper[i][k] = mat[i][k] - sum; 
		} 

		// Lower Triangular 
		for (int k = i; k < n; k++) { 
			if (i == k) 
				lower[i][i] = 1; // Diagonal as 1 
			else { 

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
	for (int i = 0; i < n; i++) { 
		// Lower 
		for (int j = 0; j < n; j++) 
			cout <<lower[i][j] << "\t"; 
		cout << "\t"; 

		// Upper 
		for (int j = 0; j < n; j++) 
			cout << upper[i][j] << "\t"; 
		cout << endl; 
	} 
} 


// returns an NxN identity matrix
void identity(int id[][20], int n)
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

int det(int mat[][MAX],int lower[][MAX],int upper[][MAX], int p[][MAX],int n)
{ //returns the determinant of square matrix A

    luDecomposition(mat, lower, upper, n);
    int D = 1;

    // calculate determinant of u
    int u_det = 1;
    for (int i =0;i<n;i++){
        u_det *= upper[i][i];
    }

    // calculate determinant as det(p) * det(l)=1 * det(u)
    D = D * pow(-1,n) * 1 * u_det;

    cout<<"determinant:"<<D<<endl;

    return D;
}


// Driver code 
int main() 
{ 
	int mat[][MAX] = { { 2, -1, -2 }, 
					{ -4, 6, 3 }, 
					{ -4, -2, 8 } }; 

    int lower[20][20], upper[20][20], p[20][20]; 
	init_matirx(lower);
    init_matirx(upper);
    identity(p,3);
	
    // luDecomposition(mat, 3); 
    det(mat,lower,upper,p,3);

	return 0; 
} 
