#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int main() {
    key_t key;
    int len, max;
    int msqid;
    char pathname[] = "11-3_2.c";

    struct clientmsgbuf {
        long mtype;
        struct {
            pid_t pid;
            float message;
        } info;
    } clientbuf;

    struct sermsgbuf {
        long mtype;
        struct {
            float message;
        } info;
    } serverbuf;

    if ((key = ftok(pathname, 0)) < 0) {
        printf("Can't generate key\n");
        exit(-1);
    }

    if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
        printf("Can't get msqid\n");
        exit(-1);
    }

    while (1) {
        max = sizeof(clientbuf.info);
        if (len = msgrcv(msqid, (struct clientmsgbuf *) &clientbuf, max, 1, 0) < 0) {
            printf("Can't receive message from queue\n");
            exit(-1);
        }
        printf("Pid = %d sends number = %f\n", clientbuf.info.pid, clientbuf.info.message);
     
        serverbuf.mtype = clientbuf.info.pid;
        
        serverbuf.info.message = clientbuf.info.message*clientbuf.info.message;
        len = sizeof(serverbuf.info);

        if (msgsnd(msqid, (struct sermsgbuf *) &serverbuf, len, 0) < 0) {
            printf("There is a problem with sending message to queue\n");
            msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
            exit(-1);
        }
        printf("Everything is alright\n");
    }
    return 0;
}
