/**
 * @file pipestring.c
 * @brief This program demonstrates the use of pipes to sort and search strings.
 *
 * The program prompts the user to enter the number of strings and the strings themselves.
 * It then creates two pipes, `pf1` and `pf2`, and forks a child process `p2`.
 * The parent process writes the number of strings and the strings to `pf1`.
 * The child process reads the number of strings and the strings from `pf1`, sorts the strings in ascending order,
 * and writes the sorted strings to `pf2`.
 * The parent process prompts the user to enter a key to be searched and forks another child process `p3`.
 * The child process `p3` reads the number of strings and the sorted strings from `pf2`,
 * searches for the key in the sorted strings, and prints the result.
 *
 * @return 0 on success.
 */
#include <stdio.h>     // Include standard input/output library
#include <string.h>    // Include string manipulation library
#include <unistd.h>    // Include Unix standard library (for fork, pipe, etc.)
#include <sys/types.h> // Include system data types

int main()
{
    int p2, p3, n, i, a, j, flag = 0;          // Declare integer variables for process IDs, number of strings, loop counters, and flags
    char str[10][10], key[10];                 // Declare  2D array for strings and a character array for the key
    char temp[10], str1[10][10], str2[10][10]; // Declare temporary string and two  2D arrays for sorting and searching
    int pf1[2], pf2[2];                        // Declare two arrays for pipe file descriptors
    pipe(pf1);                                 // Create a pipe for inter-process communication
    pipe(pf2);                                 // Create another pipe for inter-process communication

    // Prompt the user for the number of strings and read them into the array
    printf("Enter the number of strings: ");
    scanf("%d", &n);
    printf("Enter the strings: ");
    for (i = 0; i < n; i++)
    {
        scanf("%s", str[i]);
    }

    // Display the entered strings
    printf("\nThe strings are: ");
    for (i = 0; i < n; i++)
    {
        printf("%s ", str[i]);
    }

    // Write the number of strings and the strings to the pipe for the child process to read
    write(pf1[1], &n, sizeof(n));
    write(pf1[1], str, sizeof(str));
    close(pf1[1]); // Close the write end of the pipe

    // Fork a new process
    p2 = fork();
    if (p2 == 0)
    { // If this is the child process
        // Read the number of strings and the strings from the pipe
        read(pf1[0], &n, sizeof(n));
        read(pf1[0], str1, sizeof(str));
        close(pf1[0]); // Close the read end of the pipe

        // Sort the strings in ascending order using bubble sort
        for (i = 0; i < n; i++)
        {
            for (j = i + 1; j <= n; j++)
            {
                if (strcmp(str1[i], str1[j]) > 0)
                {
                    strcpy(temp, str1[i]);
                    strcpy(str1[i], str1[j]);
                    strcpy(str1[j], temp);
                }
            }
        }

        // Display the sorted strings
        printf("\n\nThe sorted strings are: ");
        for (i = 0; i < n; i++)
        {
            printf("%s ", str1[i]);
        }

        // Write the number of strings and the sorted strings to the second pipe for the grandchild process to read
        write(pf2[1], &n, sizeof(n));
        write(pf2[1], str1, sizeof(str1));
        close(pf2[1]); // Close the write end of the second pipe
    }
    else
    { // If this is the parent process
        // Prompt the user for the key to be searched
        printf("\n\nEnter the key to be searched: ");
        getchar(); // Consume the newline character left in the input buffer by scanf
        scanf("%s", key);
        printf("The key is: %s", key);

        // Fork another new process
        p3 = fork();
        if (p3 == 0)
        {             // If this is the grandchild process
            sleep(5); // Wait for  5 seconds to simulate processing time

            // Read the number of strings and the sorted strings from the second pipe
            read(pf2[0], &a, sizeof(a));
            read(pf2[0], str2, sizeof(str2));
            close(pf2[0]); // Close the read end of the second pipe

            // Search for the key in the sorted strings
            for (i = 0; i < a; i++)
            {
                if (strcmp(str2[i], key) == 0)
                {
                    flag = 1; // Set the flag to indicate the key is found
                    break;
                }
            }

            // Display whether the key was found and its position
            if (flag == 1)
            {
                printf("\n\nThe key is found at position %d", i + 1);
            }
            else
            {
                printf("\n\nThe key is not found");
            }
        }
    }
}
