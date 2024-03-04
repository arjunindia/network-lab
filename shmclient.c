#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

int main()
{
    key_t key = ftok("shm", 65);
    int shmid = shmget(key, 1024, 0666 | IPC_CREAT);
    int a, b;
    int c, d;
    int *shm = (int *)shmat(shmid, 0, 0);
    printf("Enter the order of first matrix : ");
    scanf("%d %d", &a, &b);
    *shm = a;
    shm++;
    *shm = b;
    shm++;
    printf("\nEnter the elements of first matrix : ");
    for (int i = 0; i < a; i++)
    {
        for (int j = 0; j < b; j++)
        {
            scanf("%d", shm);
            shm++;
        }
    }
    printf("\nEnter the order of second matrix : ");
    scanf("%d %d", &c, &d);
    *shm = c;
    shm++;
    *shm = d;
    shm++;
    printf("\nEnter the elements of second matrix : ");
    for (int i = 0; i < c; i++)
    {
        for (int j = 0; j < d; j++)
        {
            scanf("%d", shm);
            shm++;
        }
    }

    return 0;
}