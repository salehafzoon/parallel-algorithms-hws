#include <iostream>
using namespace std;
#define N 10

template<typename Type>
void determinant(Type a[N][N], int k){

    float det = 0;

    a[0][0] = 10;
    if (a[0][0] == 10){
        cout << "success" <<endl;
    }
}

template<typename Type>

void foo(Type t1)
{   
    cout << t1 <<endl;
}

int main(int argc, char const *argv[])
{
    int a[N][N];
    float b[N][N];
    double c[N][N];

    // int size = 1;
    // foo(size);
    // foo(3.4);
    
    determinant(a,N);
    determinant(b,N);
    determinant(c,N);
    
    
    return 0;
}
