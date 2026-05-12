#include <stdio.h>
#include <pthread.h>

int shared = 0;
pthread_mutex_t lock;

void* increment(void* arg) {
    for(int i = 0; i < 5; i++) {
        pthread_mutex_lock(&lock);

        shared++;

        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;

    pthread_mutex_init(&lock, NULL);

    pthread_create(&t1, NULL, increment, NULL);
    pthread_create(&t2, NULL, increment, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Final value (with mutex): %d\n", shared);

    pthread_mutex_destroy(&lock);

    return 0;
}
