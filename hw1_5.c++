
void LUP_decomp(A) { //LU decomposition with partial pivoting
  //returns the LU matrix, the permutation matrix of matrix A, and the number of row swaps 
  //This function overwrites A
  N = A.length; 
  P = identity(N); 
  exchanges = 0; //count the number of row swaps

  for (i=0; i<N; i++) { 

    //start pivot section
    Umax = 0;
    for (r=i; r<N; r++) {
      Uii=A[r][i];
      q = 0;
      while (q<i) {
        Uii -= A[r][q]*A[q][r];
        q++;
      } 
      if (Math.abs(Uii)>Umax) {
        Umax = Math.abs(Uii);
        row = r;
      }
    }
    if (i!=row) {//swap rows
      exchanges++;
      for (q=0; q<N; q++) {
        tmp = P[i][q];
        P[i][q]=P[row][q];
        P[row][q]=tmp;
        tmp = A[i][q];
        A[i][q]=A[row][q];
        A[row][q]=tmp;
      } 
    }
   //end pivot section

    j = i;
    while (j<N) { //Determine U across row i
      q = 0;
      while (q<i) {
        A[i][j] -= A[i][q]*A[q][j];
        q++;
      } 
      j++;
    }
    j = i+1;
    while (j<N) { //Determine L down column i
      q = 0;
      while (q<i) {
        A[j][i] -= A[j][q]*A[q][i];
        q++;
      } 
      A[j][i] = A[j][i]/A[i][i];
      j++;
    } 
  }
  return {LU:A,P:P,exchanges:exchanges};
}


function identity(N) { // returns an NxN identity matrix
  var I = [];
  for (j=0; j<N; j++) {
    I[j] = new Array(N);
    for (k=0; k<N; k++) {
      I[j][k] = 0;
    }
   I[j][j]=1;
  }
  return I;
}

function write_matrix(A) {
  rows = A.length;
  cols = A[0].length;
  for (i=0; i<rows; i++) {
    for (j=0; j<cols; j++) {
      document.write(A[i][j]+" ");
    }
    document.write("<\/br> ");
  }
  document.write("<\/br> ");
}

var A = [[2,7,6,2],[9,5,1,3],[4,3,8,4],[5,6,7,8]];

 
document.write("A = <br \/>");
write_matrix(A);

LUP = LUP_decomp(A);

document.write("LU = <br />");
write_matrix(LUP.LU);

document.write("P = <br />");
write_matrix(LUP.P);

</script>
