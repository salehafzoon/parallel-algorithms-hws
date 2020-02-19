/**************************************************
******DETERMINANT FROM GAUSS ELIMINATION***********
**************************************************/
#include<stdio.h>
#include<math.h>
/*******
Function that calculates the determinant of a square matrix using Gauss-Elimination :
Pass the square matrix as a parameter, and calculate and return the dete
Parameters: order(n),matrix[n][n]
********/
#define N 4;

double determinant(int N, double a[N][N]){
	double det=1;
	int i;
	int swapCount=gaussElimination(N,N,a);
	for(i=0;i<N;i++){
		det =det*a[i][i];
	}
	return det*pow(-1,swapCount);
}
/********
Function that perform Gauss Elimination
Pass the square matrix as a parameter, and calculate and store the upperTriangular(Gauss-Eliminated Matrix) in it
Parameters: rows(m),columns(n),matrix[m][n]
********/
int gaussElimination(int m, int n, double a[m][n]){
	int i,j,k;
	int swapCount=0;
	for(i=0;i<m-1;i++){
		//Partial Pivoting
		for(k=i+1;k<m;k++){
			//If diagonal element(absolute vallue) is smaller than any of the terms below it
			if(fabs(a[i][i])<fabs(a[k][i])){
				//Swap the rows
				swapCount++;
				for(j=0;j<n;j++){				
					double temp;
					temp=a[i][j];
					a[i][j]=a[k][j];
					a[k][j]=temp;
				}
			}
		}
		//Begin Gauss Elimination
		for(k=i+1;k<m;k++){
			double  term=a[k][i]/ a[i][i];
			for(j=0;j<n;j++){
				a[k][j]=a[k][j]-term*a[i][j];
			}
		}
	}
	return swapCount;		
}
/*******
Function that reads the elements of a matrix row-wise
Parameters: rows(m),columns(n),matrix[m][n] 
*******/
void readMatrix(int m, int n, double matrix[m][n]){
	int i,j;
	for(i=0;i<m;i++){
		for(j=0;j<n;j++){
			scanf("%lf",&matrix[i][j]);
		}
	} 
}
/*******
Function that prints the elements of a matrix row-wise
Parameters: rows(m),columns(n),matrix[m][n] 
*******/
void printMatrix(int m, int n, double matrix[m][n]){
	int i,j;
	for(i=0;i<m;i++){
		for(j=0;j<n;j++){
			printf("%lf\t",matrix[i][j]);
		}
		printf("\n");
	} 
}
/*******
Function that copies the elements of a matrix to another matrix
Parameters: rows(m),columns(n),matrix1[m][n] , matrix2[m][n]
*******/
void copyMatrix(int m, int n, double matrix1[m][n], double matrix2[m][n]){
	int i,j;
	for(i=0;i<m;i++){
		for(j=0;j<n;j++){
			matrix2[i][j]=matrix1[i][j];
		}
	} 
}

int main(){
	int n,i,j;
	printf("Enter the order of the matrix:\n(No. of rows/columns (n))\n");
	scanf("%d",&n);
	//Declare a matrix to store the user given matrix
	double a[n][n];
	printf("\nEnter the elements of matrix:\n");
	readMatrix(n,n,a); 
	printf("\nThe determinant using Gauss Eliminiation is:\n\n%lf\n",determinant(n,a));
	
	
}