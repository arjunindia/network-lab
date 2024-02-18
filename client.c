/**
 * This C program is a simple TCP client that connects to a server, sends a string, and receives a response.
 * It's designed to run on a local machine (localhost) and communicates with a server that should be running on the same machine.
 * The code is well-structured for a beginner, but I'll add comments to explain each part of the code.
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

// The main function where the program starts execution.
int main()
{
    // Declare a structure to hold the server's address information.
    struct sockaddr_in caddr;

    // Variables to hold the socket descriptor and other necessary information.
    int rec, cid;
    unsigned int n, port, namelen;

    // Buffers to hold the string to be sent and the received string.
    char buf[32], tmp[32];

    // Variables used for loops and temporary storage.
    int i, j, k, c, l, s;

    // Prompt the user to enter the port number to connect to.
    printf("Enter the port number: ");
    scanf("%d", &port);

    // Create a socket and store its descriptor in cid.
    cid = socket(AF_INET, SOCK_STREAM, 0);
    if (cid == -1)
    {
        // If the socket creation fails, print an error message and exit.
        perror("Socket error\n");
        return 0;
    }

    // Zero out the server address structure.
    memset(&caddr, 0, sizeof(caddr));

    // Set the address family to IPv4 and the port number.
    caddr.sin_family = AF_INET;
    caddr.sin_port = htons(port);

    // Set the server IP address to localhost (127.0.0.1).
    caddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Store the size of the address structure.
    namelen = sizeof(caddr);

    // Connect to the server.
    if (connect(cid, (struct sockaddr *)&caddr, sizeof(caddr)) == -1)
    {
        // If the connection fails, print an error message and exit.
        perror("Connect error\n");
        return 0;
    }

    // Start an infinite loop to send and receive data.
    do
    {
        // Prompt the user to enter a string to send to the server.
        printf("Enter the string: ");
        scanf("%s", tmp);

        // Send the string to the server.
        rec = send(cid, tmp, sizeof(tmp), 0);
        if (rec == -1)
        {
            // If the send operation fails, print an error message and exit.
            perror("Send error\n");
            return 0;
        }

        // If the user enters "exit", break out of the loop.
        if (strcmp(tmp, "exit") == 0)
        {
            break;
        }

        // Print the string that was sent.
        printf("Sent string: %s\n", tmp);

        // Clear the buffer that will hold the received string.
        strncpy(buf, "", 32);

        // Receive the string from the server.
        rec = recv(cid, buf, sizeof(buf), 0);
        if (rec == -1)
        {
            // If the receive operation fails, print an error message and exit.
            perror("Receive error\n");
            return 0;
        }

        // Print the string that was received.
        printf("Received string: %s\n", buf);

    } while (1);

    // Shutdown the socket for sending.
    shutdown(cid, 1);

    // Close the socket.
    close(cid);
}
