/* mutex_example.c */
#include <pthread.h>
#include <stdio.h>
#define MAX 5

int counter = 0;
pthread_mutex_t mutex;

void* increment(void* arg) {
    for (int i=0; i<10000; i++) {
        pthread_mutex_lock(&mutex);
        counter++;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;
    pthread_mutex_init(&mutex, NULL);
    
    pthread_create(&t1, NULL, increment, NULL);
    pthread_create(&t2, NULL, increment, NULL);
    
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    
    printf("Final Counter: %d (Expected: 20000)\n", counter);
    pthread_mutex_destroy(&mutex);
    return 0;
}