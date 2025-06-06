/* semaphore_example.c */
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

sem_t sem_producer, sem_consumer;
int buffer = 0;

void* producer(void* arg) {
    for (int i=1; i<=5; i++) {
        sem_wait(&sem_producer);
        buffer = i;
        printf("Produced: %d\n", buffer);
        sem_post(&sem_consumer);
    }
    return NULL;
}

void* consumer(void* arg) {
    for (int i=0; i<5; i++) {
        sem_wait(&sem_consumer);
        printf("Consumed: %d\n", buffer);
        sem_post(&sem_producer);
    }
    return NULL;
}

int main() {
    sem_init(&sem_producer, 0, 1);
    sem_init(&sem_consumer, 0, 0);
    
    pthread_t t1, t2;
    pthread_create(&t1, NULL, producer, NULL);
    pthread_create(&t2, NULL, consumer, NULL);
    
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    
    sem_destroy(&sem_producer);
    sem_destroy(&sem_consumer);
    return 0;
}