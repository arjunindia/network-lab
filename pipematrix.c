/*
This C program demonstrates the use of inter-process communication (IPC) through pipes to perform matrix multiplication and then calculate the sum of the elements in each row and column of the resulting matrix. The program uses a series of forked processes to perform these tasks in a step-by-step manner. Here's a breakdown of the code with comments:
*/
#include <stdio.h>     // Include standard input/output library
#include <string.h>    // Include string manipulation library
#include <unistd.h>    // Include Unix standard library (for fork, pipe, etc.)
#include <sys/types.h> // Include system data types

// The main function where the program starts execution.
int main()
{
    // Declare two  20x20 integer matrices and variables for matrix dimensions and indices.
    int mat1[20][20], mat2[20][20];
    int a, b, x, y, i, j, k, s, t;

    // Declare file descriptors for four pipes for IPC.
    int fd1[2], fd2[2], fd3[2], fd4[2];

    // Declare process IDs for child processes.
    int p2, p3, p4;

    // Declare variables for sum and count used in the calculation.
    int sum, count;

    // Create four pipes for inter-process communication.
    pipe(fd1);
    pipe(fd2);
    pipe(fd3);
    pipe(fd4);

    // Print the process ID of the parent process.
    printf("Parent Process id : %d\n", getpid());

    // Prompt the user to enter the order of the two matrices.
    printf("Order of matrix  1 : ");
    scanf("%d %d", &a, &b);
    printf("Order of matrix  2 : ");
    scanf("%d %d", &x, &y);

    // Check if matrix multiplication is possible (the number of columns in the first matrix should be equal to the number of rows in the second matrix).
    if (b != x)
    {
        printf("Matrix multiplication not possible\n");
        return 0;
    }

    // Prompt the user to enter the elements of the two matrices.
    printf("Enter the elements of matrix  1 : ");
    for (i = 0; i < a; i++)
    {
        for (j = 0; j < b; j++)
        {
            scanf("%d", &mat1[i][j]);
        }
    }
    printf("Enter the elements of matrix  2 : ");
    for (i = 0; i < x; i++)
    {
        for (j = 0; j < y; j++)
        {
            scanf("%d", &mat2[i][j]);
        }
    }

    // Print the matrices.
    printf("Matrix  1 : \n");
    for (i = 0; i < a; i++)
    {
        for (j = 0; j < b; j++)
        {
            printf("%d ", mat1[i][j]);
        }
        printf("\n");
    }
    printf("Matrix  2 : \n");
    for (i = 0; i < x; i++)
    {
        for (j = 0; j < y; j++)
        {
            printf("%d ", mat2[i][j]);
        }
        printf("\n");
    }

    // Write the matrix dimensions and elements to the first pipe.
    write(fd1[1], &a, sizeof(a));
    write(fd1[1], &b, sizeof(b));
    write(fd1[1], &x, sizeof(x));
    write(fd1[1], &y, sizeof(y));
    write(fd1[1], mat1, sizeof(mat1));
    write(fd1[1], mat2, sizeof(mat2));
    close(fd1[1]);

    // Fork a new process.
    p2 = fork();
    if (p2 == 0)
    {
        // Child process code for matrix multiplication.
        int mat3[40][40];
        printf("Parent Process of p2 id : %d\n", getppid());
        printf("Child Process of p2 id : %d\n", getpid());
        read(fd1[0], &a, sizeof(a));
        read(fd1[0], &b, sizeof(b));
        read(fd1[0], &x, sizeof(x));
        read(fd1[0], &y, sizeof(y));
        read(fd1[0], mat1, sizeof(mat1));
        read(fd1[0], mat2, sizeof(mat2));
        close(fd1[0]);
        for (i = 0; i < a; i++)
        {
            for (k = 0; k < y; k++)
            {
                mat3[i][k] = 0;
                for (j = 0; j < b; j++)
                {
                    mat3[i][k] = mat3[i][k] + mat1[i][j] * mat2[j][k];
                }
            }
        }
        write(fd2[1], &a, sizeof(a));
        write(fd2[1], &y, sizeof(y));
        write(fd2[1], mat3, sizeof(mat3));
        close(fd2[1]);

        // Fork another process for calculating the sum of elements in each row and column.
        p3 = fork();
        if (p3 == 0)
        {
            int pdt[40][40];
            int trans[40][40];
            printf("Parent Process of p3 id : %d\n", getppid());
            printf("Child Process of p3 id : %d\n", getpid());
            read(fd2[0], &s, sizeof(s));
            read(fd2[0], &t, sizeof(t));
            read(fd2[0], pdt, sizeof(pdt));
            close(fd2[0]);
            printf("Product of the matrices : \n");
            for (i = 0; i < s; i++)
            {
                for (j = 0; j < t; j++)
                {
                    printf("%d ", pdt[i][j]);
                }
                printf("\n");
            }
            write(fd3[1], &s, sizeof(s));
            write(fd3[1], &t, sizeof(t));
            write(fd3[1], pdt, sizeof(pdt));
            close(fd3[1]);

            // Fork another process for calculating the sum of elements in each row and column.
            p4 = fork();
            if (p4 == 0)
            {
                printf("Parent Process of p4 id : %d\n", getppid());
                printf("Child Process of p4 id : %d\n", getpid());
                read(fd3[0], &s, sizeof(s));
                read(fd3[0], &t, sizeof(t));
                read(fd3[0], pdt, sizeof(pdt));
                close(fd3[0]);
                for (i = 0; i < s; i++)
                {
                    sum = 0;
                    count = 0;
                    for (j = 0; j < t; j++)
                    {
                        if (count < t)
                        {
                            sum = sum + pdt[i][j];
                            count++;
                        }
                    }
                    printf("Sum of the elements of row %d : %d\n", i + 1, sum);
                }
                for (i = 0; i < s; i++)
                {
                    for (j = 0; j < t; j++)
                    {
                        trans[i][j] = pdt[j][i];
                    }
                }
                for (i = 0; i < s; i++)
                {
                    sum = 0;
                    count = 0;
                    for (j = 0; j < t; j++)
                    {
                        if (count < t)
                        {
                            sum = sum + trans[i][j];
                            count++;
                        }
                    }
                    printf("Sum of the elements of column %d : %d\n", i + 1, sum);
                }
            }
        }
    }
    return 0;
}
