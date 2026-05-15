#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

void handle_sigint(int sig) {
    printf("\nSIGINT received! Exiting program safely...\n");
    exit(0);
}

int main() {
    signal(SIGINT, SIG_IGN);

    for(int i = 1; i <= 10; i++) {
        printf("Iteration %d\n", i);
        sleep(1);

        if(i == 5) {
            printf("\nNow SIGINT (Ctrl + C) will be handled.\n");
            signal(SIGINT, handle_sigint);
        }
    }

    printf("Program completed normally.\n");
    return 0;
}
