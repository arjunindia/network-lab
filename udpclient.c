#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>

int main()
{
    struct sockaddr_in saddr, sin, cin;
    char buf[32], echobuf[32];
    int cid, n, m, i, byrc, s, p1, port, r;
    printf("Enter port number: ");
    scanf("%d", &port);
    cid = socket(AF_INET, SOCK_DGRAM, 0);
    if (cid < 0)
    {
        perror("Socket not created\n");
        exit(0);
    }
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(port);
    saddr.sin_addr.s_addr = INADDR_ANY; // or inet_addr("127.0.0.1"); for localhost
    cin.sin_family = AF_INET;
    cin.sin_port = htons(port);
    cin.sin_addr.s_addr = INADDR_ANY;
    // r = bind(cid, (struct sockaddr *)&cin, sizeof(cin));
    // if (r < 0)
    // {
    //     perror("Bind failed\n");
    //     exit(0);
    // }
    n = sizeof(cin);
    printf("Client is running\n");
    do
    {
        strncmp(buf, "", 32);
        printf("Enter message to send: ");
        scanf("%s", buf);
        sendto(cid, buf, 32, 0, (struct sockaddr *)&saddr, sizeof(saddr));
        if (strcmp(buf, "exit") == 0 || strcmp(echobuf, "exit") == 0)
        {
            close(cid);
            exit(0);
        }
        strncpy(echobuf, "", 32);
        printf("Waiting for server\n");
        if (byrc = recvfrom(cid, echobuf, 32, 0, (struct sockaddr *)&saddr, &n) <= 0)
        {
            perror("Receive failed\n");
            exit(0);
        }
        printf("Received message: %s\n", echobuf);
        if (strcmp(buf, "exit") == 0 || strcmp(echobuf, "exit") == 0)
        {
            close(cid);
            exit(0);
        }
    } while (1);
    close(cid);
}