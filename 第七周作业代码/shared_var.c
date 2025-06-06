/* shared_var.c */
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

static int global = 1;

void* t1_execute(void* arg) {
    while (global < 5) {
        printf("Thread 1: %d\n", global);
        sleep(1);
    }
    return NULL;
}

void* t2_execute(void* arg) {
    while (global < 5) {
        global++;
        printf("Thread 2 updated\n");
        sleep(1);
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;
    pthread_create(&t1, NULL, t1_execute, NULL);
    pthread_create(&t2, NULL, t2_execute, NULL);
    
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    return 0;
}