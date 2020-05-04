#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h> 
#include<time.h>

#define MaxItems 5
#define BufferSize 1 // Size of the buffer

sem_t empty,full;
pthread_mutex_t mutex;

void *producer(void *pno)
{   
    for(int i = 0; i < MaxItems; i++) {
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);
        // sleep(1); 
        printf("hello");
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
    }
    pthread_exit(NULL);
}
void *consumer(void *cno)
{   
    for(int i = 0; i < MaxItems; i++) {
        sem_wait(&full);
        pthread_mutex_lock(&mutex);
        printf(" world\n");
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
    }
    pthread_exit(NULL);
}

void *serial_helloworld(){
    printf("--------serial--------\n");
    for (int i = 0;i<5;i++)
        printf("hello world\n");    
    
}

int main(int argc , char* argv[])
{   
    
    clock_t ser_msec , par_msec;

    ser_msec = clock();
    
    serial_helloworld();
    
    ser_msec = (double)(clock() - ser_msec) * 1000000 /CLOCKS_PER_SEC; 
    
    pthread_t thrd1,thrd2;
        
    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty,0,BufferSize);
    sem_init(&full,0,0);

    int a[2] = {1,2}; //Just used for numbering the producer and consumer

    par_msec = clock();
    
    printf("--------parallel--------\n");

    pthread_create(&thrd1, NULL, producer, NULL);
    pthread_create(&thrd2, NULL, consumer, NULL);

    pthread_join(thrd1, NULL);
    pthread_join(thrd2, NULL);
    
    par_msec = (double)(clock() - par_msec) * 1000000 /CLOCKS_PER_SEC; 
    printf("\nserial: %d microsecond\n",ser_msec);
    printf("parallel: %d microsecond\n",par_msec);
    
    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);

    return 0;
    
}