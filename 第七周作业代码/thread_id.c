/* thread_id.c */
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void* thread_func(void* arg) {
    printf("New Thread:\n");
    printf("PID: %d\n", getpid());
    printf("TID: %lu\n", (unsigned long)pthread_self());
    return NULL;
}

int main() {
    pthread_t tid;
    printf("Main Thread:\n");
    printf("PID: %d\n", getpid());
    printf("TID: %lu\n", (unsigned long)pthread_self());
    
    pthread_create(&tid, NULL, thread_func, NULL);
    pthread_join(tid, NULL);
    return 0;
}