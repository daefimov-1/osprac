#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>

int main()
{    
    struct  sembuf mybuf;         
    char    pathname[] = "task3.c";
    size_t  size;
    char    resstring[15];
    int     fd[2], result;
    int     communications_count; 
    key_t   key;                  
    int     semid;               
    

    scanf("%d", &communications_count);

    if (pipe(fd) < 0) {
        printf("Can\'t open pipe\n");
        exit(-1);
    }

    if ((key = ftok(pathname, 0)) < 0) {
        printf("Cannot generate key\n");
        exit(-1);
    }

    if ((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0) {
        printf("Cannot create semaphore\n");
        exit(-1);
    }

    mybuf.sem_num = 0;
    mybuf.sem_op = 1; //Semaphore is initialized with 1
    mybuf.sem_flg = 0;

    if (semop(semid, &mybuf, 1) < 0) {
        printf("Cannot initialize semaphore with 1\n");
        exit(-1);
    }

    result = fork();

    if (result < 0) {
        printf("Cannoy fork the child\n");
        exit(-1);
    }
    else if (result > 0) {


        /* Parent process */
	mybuf.sem_num = 0;
        mybuf.sem_op = -1;
        mybuf.sem_flg = 0;
        if (semop(semid, &mybuf, 1) < 0) {
            printf("Cannot reduce value of semaphore by 1\n");
            exit(-1);
        }

        for (int i = 0; i < communications_count; ++i) {
            size = write(fd[1], "Hello, child!", 14);

            if (size != 14) {
                printf("Parent: cannot write all chars to the pipe.\n");
                exit(-1);
            }

            mybuf.sem_num = 0;
            mybuf.sem_op = 2;
            mybuf.sem_flg = 0;
            if (semop(semid, &mybuf, 1) < 0) {
                printf("Cannot rightly enter the critical section in program A\n");
                exit(-1);
            }

            mybuf.sem_num = 0;
            mybuf.sem_op = 0;
            mybuf.sem_flg = 0;
            if (semop(semid, &mybuf, 1) < 0) {
                printf("Cannot rightly enter the critical section in program A.\n");
                exit(-1);
            }

	    mybuf.sem_num = 0;
            mybuf.sem_op = -1;
            mybuf.sem_flg = 0;
            if (semop(semid, &mybuf, 1) < 0) {
                printf("Cannot rightly enter the critical section in program A\n");
                exit(-1);
            }

            size = read(fd[0], resstring, 15);
            if (size < 0) {
                printf("Can\'t read string from pipe\n");
                exit(-1);
            }

            printf("Parent: reading needable string: %s\n", resstring);
        }


        printf("Parent has finished\n");

    }
    else {

        /* Child process */
        for (int i = 0; i < communications_count; ++i) {

            mybuf.sem_num = 0;
            mybuf.sem_op = -2;
            mybuf.sem_flg = 0;

            if (semop(semid, &mybuf, 1) < 0) {
                printf("Cannot rightly enter the critical section in program\n");
                exit(-1);
            }

            size = read(fd[0], resstring, 14);

            if (size < 0) {
                printf("Child: Cannot read string from the pipe.\n");
                exit(-1);
            }

            printf("Child: Read the string: %s\n", resstring);

            size = write(fd[1], "Hello, parent!", 15);

            if (size != 15) {
                printf("Child: Cannot write all needable chars to the pipe.\n");
                exit(-1);
            }    

            mybuf.sem_num = 0;
            mybuf.sem_op = 1;
            mybuf.sem_flg = 0;

            if (semop(semid, &mybuf, 1) < 0) {
                printf("Cannot rightly enter the critical section in program\n");
                exit(-1);
            }
        }
        if (close(fd[0]) < 0) {
            printf("Child: Cannot close pipe for reading\n"); 
            exit(-1);
        }
        if (close(fd[1]) < 0) {
            printf("Child: Cannot close pipe for writing\n"); 
            exit(-1);
        }
    }

    return 0;
}
