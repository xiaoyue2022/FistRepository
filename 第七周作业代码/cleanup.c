/* cleanup.c */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void clean1(void* arg) { printf("Cleanup 1: %s\n", (char*)arg); }
void clean2(void* arg) { printf("Cleanup 2: %s\n", (char*)arg); }

void* thread_func(void* arg) {
    pthread_cleanup_push(clean1, "Handler 1");
    pthread_cleanup_push(clean2, "Handler 2");
    
    if (*(int*)arg == 1) {
        pthread_exit(NULL);
    } else {
        return NULL;
    }
    
    pthread_cleanup_pop(0);
    pthread_cleanup_pop(0);
}

int main(int argc, char​**​ argv) {
    int mode = atoi(argv[1]);
    pthread_t tid;
    pthread_create(&tid, NULL, thread_func, &mode);
    pthread_join(tid, NULL);
    return 0;
}