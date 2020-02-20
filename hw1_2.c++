#include <iostream>
#include <time.h>

using namespace std;
#define N 10

template<typename Type>
void getCfactor(Type M[N][N], Type t[N][N], int p, int q, int n) {
   int i = 0, j = 0;
   
    for (int r= 0; r< n; r++) {
       //Copy only those elements which are not in given row r and column c:  
        for (int c = 0; c< n; c++)  {
            if (r != p && c != q) 
            { 
                t[i][j++] = M[r][c]; 
                //If row is filled increase r index and reset c index
                if (j == n - 1) {
                j = 0; i++;
                }
            }
        }
    }
}

template<typename Type>
int DET(Type M[N][N], int n){
   int D = 0;
   if (n == 1)
      return M[0][0];
   int t[N][N]; //store cofactors
   int s = 1; //store sign multiplier //
    //To Iterate each element of first row
   for (int f = 0; f < n; f++) {
      //For Getting Cofactor of M[0][f] do 
      getCfactor(M, t, 0, f, n); 
      D += s * M[0][f] * DET(t, n - 1);
      s = -s;
   }
   return D;
}

//to find adjoint matrix 

template<typename Type>
void ADJ(Type M[N][N],Type adj[N][N]){
   if (N == 1) {
      adj[0][0] = 1; return;
   }
   int s = 1,
   t[N][N];
   for (int i=0; i<N; i++) {
      for (int j=0; j<N; j++) {
         //To get cofactor of M[i][j]
         getCfactor(M, t, i, j, N);
         s = ((i+j)%2==0)? 1: -1; //sign of adj[j][i] positive if sum of row and column indexes is even.
         adj[j][i] = (s)*(DET(t, N-1)); //Interchange rows and columns to get the transpose of the cofactor matrix
      }
   }
}

template<typename Type>
bool INV(Type M[N][N], float inv[N][N]) {
   int det = DET(M, N);
   if (det == 0) {
      cout << "can't find its inverse";
      return false;
   }
   int adj[N][N]; ADJ(M, adj);
   for (int i=0; i<N; i++) for (int j=0; j<N; j++) inv[i][j] = adj[i][j]/float(det);
   return true;
}

 //print the matrix. 
template<typename Type>
void print(Type A[N][N]){
   for (int i=0; i<N; i++) { for (int j=0; j<N; j++) cout << A[i][j] << " "; cout << endl; }
}

int main() {

   int M[N][N];

   for (int i=0 ; i< N; i++){
       for (int j=0; j< N; j++)
            M[i][j] = rand()%10;
   }

    float inv[N][N];
    
    cout << "Input matrix is :\n"; print(M);
    clock_t time = clock();
    cout << "\nThe Inverse is :\n"; if (INV(M, inv)) print(inv);
    
    time = clock() - time; 
    double time_taken = ((double)time)/CLOCKS_PER_SEC; // in seconds 
    printf("\n\ntime of execution : %f seconds\n", time_taken); 
    
   return 0;
}