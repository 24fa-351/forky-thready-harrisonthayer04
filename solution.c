#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

void create_processes(int pattern, int current, int max) {
    if (pattern == 1) {
        for (int ix = 1; ix <= max; ix++) {
            pid_t pid = fork();
            if (pid == 0) {
                printf("Process %d (%d) beginning\n", ix, getpid());
                sleep(2);
                printf("Process %d (%d) exiting\n", ix, getpid());
                exit(0);
            } else {
                printf("Process 0 (%d) created Process %d (%d)\n", getpid(), ix,
                       pid);
            }
        }
        for (int iy = 0; iy < max; iy++) {
            wait(NULL);
        }
        printf("Process 0 (%d) exiting\n", getpid());
    } else if (pattern == 2) {
        printf("Process %d (%d) beginning\n", current, getpid());
        sleep(2);
        if (current < max) {
            pid_t pid = fork();
            if (pid == 0) {
                printf("Process %d (%d) created Process %d (%d)\n", current,
                       getppid(), current + 1, getpid());
                create_processes(pattern, current + 1, max);
                exit(0);
            } else {
                wait(NULL);
                printf("Process %d (%d) exiting\n", current, getpid());
            }
        } else {
            printf("Process %d (%d) exiting\n", current, getpid());
        }
    } else if (pattern == 3) {
        printf("Process %d (%d) beginning\n", current, getpid());
        sleep(2);
        if (current < max) {
            pid_t pid1 = fork();
            if (pid1 == 0) {
                printf("Process %d (%d) created Process %d (%d)\n", current,
                       getppid(), current + 1, getpid());
                create_processes(pattern, current + 1, max);
                exit(0);
            }
            if (current + 1 < max) {
                pid_t pid2 = fork();
                if (pid2 == 0) {
                    printf("Process %d (%d) created Process %d (%d)\n", current,
                           getppid(), current + 2, getpid());
                    create_processes(pattern, current + 2, max);
                    exit(0);
                }
            }
            wait(NULL);
            wait(NULL);
        }
        printf("Process %d (%d) exiting\n", current, getpid());
    } else {
        printf("Invalid pattern number.\n");
    }
}

int main(int argc, char *argv[]) {
    int num = atoi(argv[1]);
    int pattern = atoi(argv[2]);
    srand(getpid());  // Seed random number generator

    create_processes(pattern, 1, num);

    return 0;
}
