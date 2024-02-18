/*
This C program is a simple TCP server that waits for a client to connect, receives a string from the client, sends a response back to the client, and continues this loop until the client sends the string "exit".
The server is designed to run on a local machine (localhost) and communicates with a client that should be running on the same machine. The code is structured in a way that is suitable for a beginner. Here are the comments to explain each part of the code:
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
    // Declare two structures to hold the server's and client's address information.
    struct sockaddr_in saddr, caddr;

    // Variables to hold the server socket descriptor, client socket descriptor, and other necessary information.
    int sid, acp;
    unsigned int n, port, namelen;

    // Buffers to hold the string to be sent and the received string.
    char buf[32], tmp[32];

    // Variables used for loops and temporary storage.
    int i, j, k, c, l, p;

    // Prompt the user to enter the port number to bind to.
    printf("Enter the port number: ");
    scanf("%d", &port);

    // Create a socket and store its descriptor in sid.
    sid = socket(AF_INET, SOCK_STREAM, 0);
    if (sid == -1)
    {
        // If the socket creation fails, print an error message and exit.
        perror("Socket error\n");
        return 0;
    }

    // Zero out the server address structure.
    memset(&saddr, 0, sizeof(saddr));

    // Set the address family to IPv4 and the port number.
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(port);

    // Set the server IP address to localhost (127.0.0.1).
    saddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Store the size of the address structure.
    namelen = sizeof(saddr);

    // Bind the socket to the specified address and port.
    if (bind(sid, (struct sockaddr *)&saddr, sizeof(saddr)) == -1)
    {
        // If the bind operation fails, print an error message and exit.
        perror("Bind error\n");
        return 0;
    }

    // Print a message to indicate that the server is waiting for a client.
    printf("Server is waiting for client\n");

    // Put the server socket in a passive mode, where it waits for the client to approach the server to make a connection.
    if (listen(sid, 10) == -1)
    {
        // If the listen operation fails, print an error message and exit.
        perror("Listen error\n");
        return 0;
    }

    // Accept a connection from a client.
    n = sizeof(caddr);
    acp = accept(sid, (struct sockaddr *)&caddr, &n);
    if (acp == -1)
    {
        // If the accept operation fails, print an error message and exit.
        perror("Accept error\n");
        return 0;
    }

    // Start an infinite loop to receive data from the client and send a response.
    do
    {
        // Clear the buffer that will hold the received string.
        strncpy(tmp, "", 32);

        // Receive the string from the client.
        l = recv(acp, tmp, sizeof(tmp), 0);
        if (l == -1)
        {
            // If the receive operation fails, print an error message and exit.
            perror("Receive error\n");
            return 0;
        }

        // Print the string that was received.
        printf("Received string: %s\n", tmp);

        // If the user sends "exit", break out of the loop.
        if (strcmp(tmp, "exit") == 0)
        {
            break;
        }

        // Clear the buffer that will hold the data to be sent to the client.
        strncpy(buf, "", 32);

        // Prompt the user to enter data to send to the client.
        printf("Enter data to client: ");
        scanf("%s", buf);

        // Send the data to the client.
        l = send(acp, buf, strlen(buf), 0);
        if (l == -1)
        {
            // If the send operation fails, print an error message and exit.
            perror("Send error\n");
            return 0;
        }

        // Print the data that was sent to the client.
        printf("Data sent to client: %s\n", buf);
    } while (1);

    // Shutdown the server socket for sending.
    shutdown(sid, 2);

    // Close the client socket.
    close(acp);

    // Close the server socket.
    close(sid);

    return 0;
}
