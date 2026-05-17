#include <stdio.h> 
#include <signal.h> 
#include <unistd.h> 
#include <sys/wait.h> 
 
void handle_sigint(int sig) { 
    printf("\nCtrl+C pressed! Not exiting...\n"); 
} 
 
void handle_sigchld(int sig) { 
    printf("Child process terminated!\n"); 
} 
 
int main() { 
    signal(SIGINT, handle_sigint); 
    signal(SIGCHLD, handle_sigchld); 
 
    pid_t pid = fork(); 
 
    if (pid == 0) { 
        printf("Child running...\n"); 
        sleep(2); 
        printf("Child exiting...\n"); 
    } else { 
        while (1) { 
            printf("Parent running...\n"); 
            sleep(1); 
        } 
    } 
 
    return 0; 
}
