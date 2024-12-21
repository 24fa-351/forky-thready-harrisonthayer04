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
    }
}

void create_processes_pattern_two(int current, int max) {
    if (current > max) {
        return;
    }
    printf("Process %d (%d) beginning\n", current, (int)getpid());
    sleep(1);
    if (current < max) {
        pid_t pid = fork();
        if (pid == 0) {
            printf("Process %d (%d) created Process %d (%d)\n", current,
                   (int)getppid(), current + 1, (int)getpid());
            create_processes_pattern_two(current + 1, max);
            exit(EXIT_SUCCESS);
        } else {
            wait(NULL);
        }
        printf("Process %d (%d) exiting\n", current, (int)getpid());
    }
}


void create_processes_pattern_three(int current, int max)
{
    if (current > max) {
        return;
    }

    printf("Process %d (%d) beginning\n", current, (int)getpid());

    pid_t pid_left = -1, pid_right = -1;
    if (2*current <= max) {
        pid_left = fork();
        if (pid_left < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        if (pid_left == 0) {
            printf("Process %d (%d) created Process %d (%d)\n",
                   current, (int)getppid(), 2*current, (int)getpid());
            create_processes_pattern_three(2*current, max);
            exit(0);
        }
    }
    if (2*current + 1 <= max) {
        pid_right = fork();
        if (pid_right < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        if (pid_right == 0) {
            printf("Process %d (%d) created Process %d (%d)\n",
                   current, (int)getppid(), 2*current+1, (int)getpid());
            create_processes_pattern_three(2*current+1, max);
            exit(0);
        }
    }

    if (pid_left > 0) {
        waitpid(pid_left, NULL, 0);
    }
    if (pid_right > 0) {
        waitpid(pid_right, NULL, 0);
    }

    printf("Process %d (%d) exiting\n", current, (int)getpid());
}
 

int main(int argc, char *argv[]) {
    srand(getpid());  // Seed random number generator
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <num> <pattern>\n", argv[0]);
        return 1;
    }
    int num = atoi(argv[1]);
    int pattern = atoi(argv[2]);
    if (pattern == 1) {
        create_processes(1, 1, num);
    } else if (pattern == 2) {
        create_processes_pattern_two(1, num);
    } else if (pattern == 3) {
        create_processes_pattern_three(1, num);
    } else {
        fprintf(stderr,
                "Invalid pattern number. Valid pattern numbers: 1,2,3\n");
    }
    return 0;
}
