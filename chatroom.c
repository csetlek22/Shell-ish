 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/wait.h>

#define MAX_BUF 1024

int main(int argc, char *argv[]) {

    if (argc != 3) {
        fprintf(stderr, "Usage: %s <roomname> <username>\n", argv[0]);
        exit(1);
    }

    char *roomname = argv[1];
    char *username = argv[2];

    char room_path[256];
    char fifo_path[256];


    snprintf(room_path, sizeof(room_path), "/tmp/chatroom-%s", roomname);

    if (mkdir(room_path, 0777) == -1) {
        if (errno != EEXIST) {
            perror("mkdir failed");
            exit(1);
        }
    }


    snprintf(fifo_path, sizeof(fifo_path), "%s/%s", room_path, username);

    if (mkfifo(fifo_path, 0666) == -1) {
        if (errno != EEXIST) {
            perror("mkfifo failed");
            exit(1);
        }
    }

    printf("Welcome to %s!\n", roomname);


    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        exit(1);
    }

    if (pid == 0) {

        char buffer[MAX_BUF];

        while (1) {
            int fd = open(fifo_path, O_RDONLY);
            if (fd == -1) {
                perror("open for read failed");
                exit(1);
            }

            int n;
            while ((n = read(fd, buffer, sizeof(buffer) - 1)) > 0) {
                buffer[n] = '\0';
                printf("%s", buffer);
                fflush(stdout);
            }

            close(fd);
        }

        exit(0);
    }


    char input[MAX_BUF];
    char message[MAX_BUF];

    while (1) {

        printf("[%s] %s > ", roomname, username);
        fflush(stdout);

        if (fgets(input, sizeof(input), stdin) == NULL)
            break;


        input[strcspn(input, "\n")] = '\0';

        snprintf(message, sizeof(message),
                 "[%s] %s: %s\n",
                 roomname, username, input);

        DIR *dir = opendir(room_path);
        if (!dir) {
            perror("opendir failed");
            continue;
        }

        struct dirent *entry;

        while ((entry = readdir(dir)) != NULL) {

            if (strcmp(entry->d_name, ".") == 0 ||
                strcmp(entry->d_name, "..") == 0)
                continue;

            if (strcmp(entry->d_name, username) == 0)
                continue;

            char other_fifo[256];
            snprintf(other_fifo, sizeof(other_fifo),
                     "%s/%s", room_path, entry->d_name);

            pid_t send_pid = fork();

            if (send_pid == 0) {
                int fd = open(other_fifo, O_WRONLY | O_NONBLOCK);
                if (fd != -1) {
                    write(fd, message, strlen(message));
                    close(fd);
                }
                exit(0);
            }
        }

        closedir(dir);


        while (waitpid(-1, NULL, WNOHANG) > 0);
    }

    return 0;
}
