Marks.c:
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main() {
    int fd;
    int marks[5];
    char buffer[100];
    int total = 0;

    fd = open("marks.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd < 0) {
        perror("Error creating file");
        exit(1);
    }

    printf("Enter marks for 5 subjects:\n");
    for (int i = 0; i < 5; i++) {
        printf("Subject %d: ", i + 1);
        scanf("%d", &marks[i]);
        sprintf(buffer, "%d\n", marks[i]);
        write(fd, buffer, strlen(buffer));
    }

    close(fd);

    fd = open("marks.txt", O_RDONLY);
    if (fd < 0) {
        perror("Error opening file");
        exit(1);
    }

    char readBuffer[100];
    int bytesRead = read(fd, readBuffer, sizeof(readBuffer) - 1);
    if (bytesRead < 0) {
        perror("Error reading file");
        close(fd);
        exit(1);
    }

    readBuffer[bytesRead] = '\0';

    char *token = strtok(readBuffer, "\n");
    while (token != NULL) {
        total += atoi(token);
        token = strtok(NULL, "\n");
    }

    printf("\nTotal Marks = %d\n", total);

    close(fd);

    return 0;
}
