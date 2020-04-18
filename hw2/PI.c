#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h> 
#include<time.h>

#define n 1000000

void serial_pi_calculate(){
    double factor = 1, sum = 0, pi;
    int i;
    for(i = 0; i < n; i++){ 
        sum += factor/(2*i+1); 
        factor = -factor; 
    } 
    pi = 4 * sum; 
    // printf("sum = %f\n",sum);
    printf("pi (serial) = %f\n", pi); 
}

typedef struct argument{
    int id;
    int size;
    double *sum;
}argument;

void *paralel_pi_calculate(void *arg)
{   
    argument arg_ = *(argument*)arg;
    int id = arg_.id, size = arg_.size;
    int start = id * size;
    int end = start + size;

    // printf("s:%d e:%d\n",start,end);

    double factor = 1;
    for(int i = start; i < end; i++) {
        *arg_.sum += factor/(2*i+1); 
        factor = -factor; 
    }
    // printf("sum:%f\n\n",*arg_.sum);

    pthread_exit(NULL);
    return 0;
}

int main(int argc , char* argv[])
{   

    int i , status , t_serial = 0 , t_parallel = 0;
    int threads_num = atoi(argv[1]);
    double total =0 , pi;
    clock_t ser_msec , par_msec;
    
    ser_msec = clock();
    
    serial_pi_calculate();
    
    ser_msec = (double)(clock() - ser_msec) * 1000000 /CLOCKS_PER_SEC;
    
    printf("%d micro seconds\n",ser_msec);

    double **sums = (double **)malloc(sizeof(double*) * threads_num);
    for (i=0;i<threads_num;i++){
        sums[i] = (double *)malloc(sizeof(double));
    }
    pthread_t threads[threads_num];
    argument arguments[threads_num];

    par_msec = clock();
    
    for(i =0;i<threads_num;i++){
        arguments[i].id = i;
        arguments[i].size = n/threads_num;
        arguments[i].sum = sums[i];
        pthread_create(&threads[i], NULL, paralel_pi_calculate, (void*)&arguments[i]);
    }
    for(i =0;i<threads_num;i++){
        pthread_join(threads[i],NULL);
        // printf("\ts:%f", *sums[i]);
        total += *sums[i];
    }
    pi = total*4;

    printf("pi (parallel) = %f\n", pi); 

    par_msec = (double)(clock() - par_msec) * 1000000 /CLOCKS_PER_SEC;
    printf("%d micro seconds\n",par_msec);

    printf("speed up : %.2f\n", ((double)ser_msec/par_msec));
    

    for (i=0;i<threads_num;i++){
        free(sums[i]);
    }
    free(sums);

    return 0;
    
}