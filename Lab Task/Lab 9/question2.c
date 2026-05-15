#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

void sigchld_handler(int sig) {
    int status;
    pid_t pid;

    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        printf("Child process with PID %d terminated\n", pid);
    }
}

int main() {
    signal(SIGCHLD, sigchld_handler);

    for (int i = 0; i < 3; i++) {
        pid_t pid = fork();

        if (pid == 0) {
            printf("Child %d started with PID %d\n", i + 1, getpid());
            sleep(3 + i * 2);
            printf("Child %d (PID %d) exiting\n", i + 1, getpid());
            exit(0);
        }
    }

    while (1) {
        printf("Server running and monitoring workers...\n");
        sleep(2);
    }

    return 0;
}
