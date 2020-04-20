#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h> 
#include<time.h>
#include<math.h>

#define min(a,b) ((a) < (b) ? a : b)

#define n 1000000

void serial_pi_calculate(){
    double sum = 0, pi;
    int i;
    double factor = 1;

    for(i = 0; i < n; i++){ 
        sum += factor/(2*i+1); 
        factor = -factor; 
    } 

    pi = 4 * sum; 
    // printf("sum (serial) = %f\n",sum);
    // printf("pi (serial) = %f\t\t", pi); 
}

typedef struct argument{
    int tid;
    int size;
    int threads_num;
    double *sum;
}argument;

void *thread_pi_calculate(void *arg)
{   
    
    argument arg_ = *(argument*)arg;
    int id = arg_.tid, size = arg_.size;
    int start = (n / arg_.threads_num) * arg_.tid; 
    int end = start + (size / arg_.threads_num);
    // int end = min(start + size / arg_.threads_num, size);
    
    if(arg_.tid == arg_.threads_num-1)
        end = n;

    // printf("tid:%d, s:%d , e:%d\n",arg_.tid,start,end);
    
    double factor = 1;
    *arg_.sum = 0;

    for(int i = start ; i < end ; i++) { 
        *arg_.sum += factor/(2*i+1);
        factor = -factor; 
    }
    // printf("sum %d:%f\n",id,*arg_.sum);

    pthread_exit(NULL);
}

typedef struct result{
    double time;
    double value;
}result;

result paralel_pi_calculate(int threads_num , double ser_msec)
{   
    
    int i;
    clock_t par_msec;
    double total =0 , pi;
    
    double **sums = (double **)malloc(sizeof(double*) * threads_num);
    for (i=0;i<threads_num;i++){
        sums[i] = (double *)malloc(sizeof(double));
    }
    pthread_t threads[threads_num];
    argument arguments[threads_num];

    par_msec = clock();
    
    for(int i =0;i<threads_num;i++){
        arguments[i].tid = i;
        arguments[i].size = n;
        arguments[i].sum = sums[i];
        arguments[i].threads_num = threads_num;
        pthread_create(&threads[i], NULL, thread_pi_calculate, (void*)&arguments[i]);
    }
    for(i =0;i<threads_num;i++){
        pthread_join(threads[i],NULL);
        total += *sums[i];
    }
    
    // printf("sum (parallel) = %f\n",total);
    
    pi = total*4;

    par_msec = (double)(clock() - par_msec) * 1000000 /CLOCKS_PER_SEC;
    
    result res;
    res.value = pi;
    res.time = par_msec;
    
    for (i=0;i<threads_num;i++){
        free(sums[i]);
    }
    free(sums);

    return res;
}  
int main(int argc , char* argv[])
{   

    FILE * fptr;
    fptr = fopen("speedups.txt", "w");

    int i;
    int threads_num = atoi(argv[1]);
    clock_t ser_msec;
    int runs = 40;

    for (int t = 1; t <= threads_num; t++)
    {
        printf("\nn = %d , threads = %d \n",n,t);
        ser_msec = clock();
    
        serial_pi_calculate();
        
        ser_msec = (double)(clock() - ser_msec) * 1000000 /CLOCKS_PER_SEC;
        
        // printf("%d microsecond\n",ser_msec);

        double avgValue=0,avgTime=0;

        for (i = 0;i<runs;i++){
            result res = paralel_pi_calculate(t,ser_msec);
            avgValue += res.value;
            avgTime += res.time;
        }
        avgValue/=(double)runs;
        avgTime/=(double)runs;
        
        // printf("\n---------\nresults of %d time runs\n",runs);
        printf("avg pi = %f \n",avgValue);
        // printf("avg Time = %f \n",avgTime);
        printf("avg speed up = %f \n--------\n", ((double)ser_msec/avgTime));
        fprintf(fptr, "%f \n", ((double)ser_msec/avgTime));
        
    }

    
    return 0;
    
}