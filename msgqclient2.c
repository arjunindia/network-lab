#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <string.h>
struct msgq
{
    long mtype;
    char mtext[100];
} msg;

int main()
{
    key_t key;
    int msgid;
    key = ftok("msgq", 65);
    msgid = msgget(key, 0666 | IPC_CREAT);
    msg.mtype = 2;
    do
    {
        printf("Enter the message to send : ");
        scanf("%s", msg.mtext);
        msgsnd(msgid, &msg, sizeof(msg), 0);
    } while (strcmp(msg.mtext, "exit") != 0);
    msgctl(msgid, IPC_RMID, NULL);
    return 0;
}