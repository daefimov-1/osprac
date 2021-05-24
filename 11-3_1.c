#include <time.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int main() {

    key_t key;
    int len, max;
    int msqid;
    char pathname[] = "11-3_1.c";

    struct clientmsgbuf {
        long mtype;
        struct {
            pid_t pid;
            float message;
        } info;
    } clientbuf;

    struct servermsgbuf {
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

    clientbuf.mtype = 1;
    clientbuf.info.pid = getpid();

    printf("Float number = ?\n");
    float a;
    scanf("%f", &a);

    len = sizeof(clientbuf.info);
    clientbuf.info.message = a;

    printf("Pid = %d sends number: %f\n", clientbuf.info.pid, clientbuf.info.message);
    if (msgsnd(msqid, (struct clientmsgbuf *) &clientbuf, len, 0) < 0) {
        printf("There is a problem with sending message to queue\n");
        msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
        exit(-1);
    }

    printf("Wait for answer\n");

    max = sizeof(serverbuf.info);
    if (len = msgrcv(msqid, &serverbuf, max, getpid(), 0) < 0) {
        printf("There is a problem with receiving message from queue\n");
        exit(-1);
    }
    printf("Squared number: %f\n", serverbuf.info.message);

    return 0;

}
