#include <stdio.h>
#include <stdlib.h> //for  malloc/calloc/free/realloc

int ** init_matrix(int size){

    int ** matrix;
    matrix = (int **) malloc(size * sizeof(int));
    for (int i =0;i<size;i++){
        matrix[i] = (int *) malloc(size * sizeof(int));
    }
    for (int i =0;i<size;i++){
        for (int j = 0; j < size; j++)
        {
            matrix[i][j] = 0;
        }
    }
    return matrix;
}

int** read_matrix(char* file_name){
    FILE *file;
    file = fopen(file_name,"r");
    if (file == NULL)
        exit(1);
    int size = 0;
    fscanf( file, "%d",&size);

    int** matrix = init_matrix(size);

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            fscanf( file, "%d",&matrix[i][j]);
        }
    }

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            printf( "%d ",matrix[i][j]);
        }
        printf("\n");
    }

    fclose(file);

    return matrix;
}

int main(int argc , char **argv){

    int** matrix = read_matrix("matrix.txt");
    return 0;
}

