#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <stdio.h>

int main()
{
    size_t  size;
    int     semid;
    struct sembuf mybuf;
    
    key_t   key;
    char    pathname[] = "task3.c";

    char    resstring[14];
    int     fd[2], result;

    if (pipe(fd) < 0) {
        printf("Can\'t open pipe\n");
        exit(-1);
    }

    if ((key = ftok(pathname, 0)) < 0){
        printf("Can\'t generate key\n");
        exit(-1);
    }

    if ((semid = semget(key, 1, 0666)) < 0) {
        printf("There is no semaphore.\n");
        if ((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0) {
            printf("Can\'t get semid\n");
            exit(-1);
        }
        printf("Semaphore is successfully created.\n");
    }

    int n;
    scanf("%d", &n);

    result = fork();

    if (result < 0) {
        printf("Can\'t fork child\n");
        exit(-1);
    } else if (result > 0) {

        for (int i = 0; i < n; ++i) {
            size = write(fd[1], "Hello, world!", 14);

            if (size != 14) {
                printf("Can\'t write all string to pipe\n");
                exit(-1);
            } else {
                printf("Parent wrote message.\n");
            }

            if (close(fd[1]) < 0) {
                printf("Parent: Can\'t close writing side of pipe\n");
                exit(-1);
            }

            mybuf.sem_op = -1;
            mybuf.sem_flg = 0;
            mybuf.sem_num = 0;

            semop(semid, &mybuf, 1);

            size = read(fd[0], resstring, 14);
            if (size < 0) {
                printf("Parent can\'t read string\n");
                exit(-1);
            }

            printf("Parent read: %s\n", resstring);

            if (close(fd[0]) < 0) {
                printf("Parent: Can\'t close reading side of pipe\n");
                exit(-1);
            }

            printf("Parent exit\n");
        }
    } else {
        
        for (int i = 0; i < n; ++i){
            size = read(fd[0], resstring, 14);
            if (size < 0) {
                printf("Can\'t read string\n");
                exit(-1);
            }
            printf("Child read: %s\n", resstring);

            if (close(fd[0]) < 0) {
                printf("Child: Can\'t close reading side of pipe\n"); exit(-1);
            }

            for (int i = 0; i < 13 / 2; i++) {
                char t = resstring[i];
                resstring[i] = resstring[13 - i - 1];
                resstring[13 - i - 1] = t;
            }

            size = write(fd[1], resstring, 14);

            mybuf.sem_op = 1;
            mybuf.sem_flg = 0;
            mybuf.sem_num = 0;

            semop(semid, &mybuf, 1);

            if (size != 14) {
                printf("Child can\'t write all string\n");
                exit(-1);
            } else {
                printf("Child wrote message.\n");
            }

            if (close(fd[1]) < 0) {
                printf("Child: Can\'t close writing side of pipe\n"); exit(-1);
            }

            printf("Child exit.\n");
        }
    }

    return 0;
}
