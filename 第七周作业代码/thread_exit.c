/* thread_exit.c */
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

void* thread_func(void* arg) {
    sleep(1);
    if (*(int*)arg == 1) {
        printf("Using pthread_exit\n");
        pthread_exit(NULL);
    } else {
        printf("Using exit\n");
        exit(0);
    }
}

int main(int argc, char​**​ argv) {
    int mode = atoi(argv[1]);
    pthread_t tid;
    
    pthread_create(&tid, NULL, thread_func, &mode);
    pthread_join(tid, NULL);
    printf("Main thread continues\n");
    return 0;
}