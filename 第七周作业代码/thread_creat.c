/* thread_creat.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

typedef struct student {
    int age;
    char name[20];
} STU;

void* create(void* arg) {
    STU* temp = (STU*)arg;
    printf("Transferred data:\n");
    printf("Age: %d\nName: %s\n", temp->age, temp->name);
    return NULL;
}

int main() {
    pthread_t tidp;
    STU* stu = malloc(sizeof(STU));
    stu->age = 20;
    strcpy(stu->name, "Alice");

    int error = pthread_create(&tidp, NULL, create, stu);
    if (error) {
        perror("pthread_create");
        return -1;
    }

    pthread_join(tidp, NULL); // 等待线程结束
    free(stu);
    return 0;
}