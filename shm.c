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
    int A[10][10], B[10][10], C[10][10];
    int *shm = (int *)shmat(shmid, 0, 0);
    a = *shm;
    shm++;
    b = *shm;
    shm++;
    for (int i = 0; i < a; i++)
    {
        for (int j = 0; j < b; j++)
        {
            A[i][j] = *shm;
            shm++;
        }
    }
    c = *shm;
    shm++;
    d = *shm;
    shm++;
    for (int i = 0; i < c; i++)
    {
        for (int j = 0; j < d; j++)
        {
            B[i][j] = *shm;
            shm++;
        }
    }
    if (b != c)
    {
        printf("Matrix multiplication not possible\n");
    }
    else
    {
        for (int i = 0; i < a; i++)
        {
            for (int j = 0; j < d; j++)
            {
                C[i][j] = 0;
                for (int k = 0; k < b; k++)
                {
                    C[i][j] += A[i][k] * B[k][j];
                }
            }
        }
        printf("The resultant matrix is : \n");
        for (int i = 0; i < a; i++)
        {
            for (int j = 0; j < d; j++)
            {
                printf("%d ", C[i][j]);
            }
            printf("\n");
        }
    }
}