#include <stdio.h> 
#include <pthread.h> 
#include <unistd.h> 
 
void* download(void* arg) { 
    printf("Downloading started...\n"); 
    sleep(2); 
    printf("Downloading completed...\n"); 
    return NULL; 
} 
 
void* process(void* arg) { 
    printf("Processing started...\n"); 
    sleep(3); 
    printf("Processing completed...\n"); 
    return NULL; 
} 
 
int main() { 
    pthread_t t1, t2; 
 
    printf("Main: Creating threads...\n"); 
 
    pthread_create(&t1, NULL, download, NULL); 
    pthread_create(&t2, NULL, process, NULL); 
 
    pthread_join(t1, NULL); 
    pthread_join(t2, NULL); 
 
    printf("Main: All tasks finished\n"); 
 
    return 0; 
}
