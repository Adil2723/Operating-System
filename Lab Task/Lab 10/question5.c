#include <pthread.h> 
#include <stdio.h> 
#include <unistd.h> 
 
#define USERS 7 
 
pthread_mutex_t mutex; 
 
void *printer(void *arg) 
{ 
    int id = *((int*)arg); 
 
    pthread_mutex_lock(&mutex); 
 
    printf("User %d is Printing...\n", id); 
    sleep(2); 
    printf("User %d Finished Printing.\n", id); 
 
    pthread_mutex_unlock(&mutex); 
 
    pthread_exit(NULL); 
} 
 
int main() 
{ 
    pthread_t threads[USERS]; 
    int ids[USERS]; 
 
    pthread_mutex_init(&mutex, NULL); 
 
    for(int i = 0; i < USERS; i++) 
    { 
        ids[i] = i + 1; 
} 
pthread_create(&threads[i], NULL, printer, &ids[i]); 
for(int i = 0; i < USERS; i++) 
pthread_join(threads[i], NULL); 
pthread_mutex_destroy(&mutex); 
return 0; 
}
