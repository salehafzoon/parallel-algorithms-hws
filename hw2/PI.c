#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h> 

#define MaxItems 5 
#define BufferSize 1 // Size of the buffer
#define n 1000000

sem_t empty;
sem_t full;
int in = 0;
int out = 0;
int buffer[BufferSize];
pthread_mutex_t mutex;

void *producer(void *pno)
{   
    int item;
    for(int i = 0; i < MaxItems; i++) {
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);
        sleep(1); 
        printf("hello");
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
    }
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
}

typedef struct arg{
    int thread_num;
}arg;

int main()
{   

    pthread_t thrd1,thrd2;

    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty,0,BufferSize);
    sem_init(&full,0,0);

    int a[2] = {1,2}; //Just used for numbering the producer and consumer

    arg args[2];
    args[0].thread_num = 1;
    args[1].thread_num = 2;

    pthread_create(&thrd1, NULL, producer, (void*)&args[0]);
    pthread_create(&thrd2, NULL, consumer, (void*)&args[1]);

    pthread_join(thrd1, NULL);
    pthread_join(thrd2, NULL);
    
    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);

    return 0;
    
}