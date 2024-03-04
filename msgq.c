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
y:
    msgrcv(msgid, &msg, sizeof(msg), msg.mtype, 0);
    if (strcmp(msg.mtext, "exit") == 0)
    {
        goto x;
    }
    printf("\nMessage received is : %s", msg.mtext);
    if (msg.mtype == 1)
    {
        printf("\nString in reverse is : ");
        for (int i = strlen(msg.mtext) - 1; i >= 0; i--)
        {
            printf("%c", msg.mtext[i]);
        }
    }
    if (msg.mtype == 2)
    {
        printf("\nNo. of vowels in the string are : ");
        int count = 0;
        for (int i = 0; i < strlen(msg.mtext); i++)
        {
            if (msg.mtext[i] == 'a' || msg.mtext[i] == 'e' || msg.mtext[i] == 'i' || msg.mtext[i] == 'o' || msg.mtext[i] == 'u' || msg.mtext[i] == 'A' || msg.mtext[i] == 'E' || msg.mtext[i] == 'I' || msg.mtext[i] == 'O' || msg.mtext[i] == 'U')
            {
                count++;
            }
        }
        printf("%d", count);
    }
    goto y;
x:
    msgctl(msgid, IPC_RMID, NULL);
    return 0;
}
