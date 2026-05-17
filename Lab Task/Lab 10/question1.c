#include <stdio.h> 
#include <unistd.h> 
#include <string.h> 
#include <ctype.h> 
 
int main() { 
    int fd[2]; 
    pid_t pid; 
    char msg[100]; 
 
    pipe(fd); 
    pid = fork(); 
 
    if (pid > 0) { 
        close(fd[0]); 
        fgets(msg, sizeof(msg), stdin); 
        write(fd[1], msg, strlen(msg) + 1); 
        close(fd[1]); 
    } else { 
        close(fd[1]); 
        read(fd[0], msg, sizeof(msg)); 
 
        for (int i = 0; msg[i]; i++) { 
            msg[i] = tolower(msg[i]); 
        } 
 
        printf("%s", msg); 
        close(fd[0]); 
    } 
 
    return 0; 
}
