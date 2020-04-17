#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h> 

#define MaxItems 5 
#define BufferSize 1 // Size of the buffer
#define n 1000

sem_t empty;
sem_t full;
int in = 0;
int out = 0;
int buffer[BufferSize];
pthread_mutex_t mutex;

void serial_pi_calculate(){
    double factor = 1, sum = 0, pi;
    int i;
    for(i = 0; i < n; i++){ 
        sum += factor/(2*i+1); 
        factor = -factor; 
    } 
    pi = 4 * sum; 
    printf("pi = %f\n", pi); 
}

typedef struct argument{
    int id;
    int size;
    double sum;
}argument;

void *paralel_pi_calculate(void *arg)
{   
    argument arg_ = *(argument*)arg;
    int id = arg_.id, size = arg_.size;
    double *sum = arg_.sum;
    int start = id * size;
    int end = start + size;

    printf("s:%d e:%d\n",start,end);

    double factor = 1;
    for(int i = start; i < end; i++) {
        sum += factor/(2*i+1); 
        factor = -factor; 
    }
    printf("sum:%f\n\n",sum);

    pthread_exit(NULL);
}

int main(int argc , char* argv[])
{   

    // serial_pi_calculate();
    int i;
    int threads_num = atoi(argv[1]);
    
    double *sums = (double *)malloc(sizeof(double) * threads_num);

    pthread_t threads[threads_num];
    argument arguments[threads_num];

    for(i =0;i<threads_num;i++){
        arguments[i].id = i;
        arguments[i].size = n/threads_num;
        arguments[i].sum = sums[i];
        pthread_create(&threads[i], NULL, paralel_pi_calculate, (void*)&arguments[i]);
    }
    double total =0;
    int status;
    for(i =0;i<threads_num;i++){
        pthread_join(threads[i],NULL);
        total =+sums[i];
    }
    int pi = total*4;
    printf("total=%f\n",total);
    printf("pi = %f\n", pi); 
    return 0;
    
}