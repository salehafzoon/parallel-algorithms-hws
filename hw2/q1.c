#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h> 


#define MaxItems 5 
#define BufferSize 1 // Size of the buffer

sem_t empty,full;
pthread_mutex_t mutex;

void *producer(void *pno)
{   
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

int main()
{   

    pthread_t thrd1,thrd2;

    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty,0,BufferSize);
    sem_init(&full,0,0);

    int a[2] = {1,2}; //Just used for numbering the producer and consumer

    pthread_create(&thrd1, NULL, producer, NULL);
    pthread_create(&thrd2, NULL, consumer, NULL);

    pthread_join(thrd1, NULL);
    pthread_join(thrd2, NULL);
    
    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);

    return 0;
    
}