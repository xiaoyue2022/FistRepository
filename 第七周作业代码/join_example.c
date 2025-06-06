/* join_example.c */
#include <stdio.h>
#include <pthread.h>

void* t1_func(void* arg) {
    for (int i=1; i<=5; i++) 
        printf("Thread 1: %d\n", i);
    return NULL;
}

void* t2_func(void* arg) {
    pthread_t t1 = *(pthread_t*)arg;
    pthread_join(t1, NULL);
    
    for (int i=6; i<=10; i++)
        printf("Thread 2: %d\n", i);
    return NULL;
}

int main() {
    pthread_t t1, t2;
    pthread_create(&t1, NULL, t1_func, NULL);
    pthread_create(&t2, NULL, t2_func, &t1);
    
    pthread_join(t2, NULL);
    return 0;
}