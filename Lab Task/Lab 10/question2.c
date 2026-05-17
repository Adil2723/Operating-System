#include <stdio.h> 
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <unistd.h> 
 
int main() { 
    key_t key = 1234; 
    int shmid = shmget(key, sizeof(int), 0666 | IPC_CREAT); 
    int *balance = (int*) shmat(shmid, NULL, 0); 
 
    *balance = 1025; 
 
    if (fork() == 0) { 
        sleep(1); 
        printf("%d\n", *balance); 
    } else { 
        *balance += 1575; 
        printf("%d\n", *balance); 
    } 
 
    return 0; 
}
