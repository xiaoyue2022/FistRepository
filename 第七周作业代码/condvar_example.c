/* condvar_example.c */
#include <pthread.h>
#include <stdio.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int flag = 0;

void* threadA(void* arg) {
    pthread_mutex_lock(&mutex);
    while (flag != 0)
        pthread_cond_wait(&cond, &mutex);
    printf("Thread A\n");
    flag = 1;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
    return NULL;
}

void* threadB(void* arg) {
    pthread_mutex_lock(&mutex);
    while (flag != 1)
        pthread_cond_wait(&cond, &mutex);
    printf("Thread B\n");
    flag = 2;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
    return NULL;
}

int main() {
    pthread_t tA, tB;
    pthread_create(&tA, NULL, threadA, NULL);
    pthread_create(&tB, NULL, threadB, NULL);
    
    pthread_join(tA, NULL);
    pthread_join(tB, NULL);
    return 0;
}