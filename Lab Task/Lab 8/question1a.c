#include <stdio.h>
#include <pthread.h>

int shared = 0;

void* increment(void* arg) {
    for(int i = 0; i < 5; i++) {
        int temp = shared;
        temp++;
        shared = temp;
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;

    pthread_create(&t1, NULL, increment, NULL);
    pthread_create(&t2, NULL, increment, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Final value (without sync): %d\n", shared);

    return 0;
}
