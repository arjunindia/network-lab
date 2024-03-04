#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    struct sockaddr_in caddr, cin;
    char buf[32], echobuf[32];
    int cid, n, m, i, byrc, s, p2, port, r;
    printf("Enter port number: ");
    scanf("%d", &port);
    cid = socket(AF_INET, SOCK_DGRAM, 0);
    if (cid < 0)
    {
        perror("Socket not created\n");
        exit(0);
    }
    caddr.sin_family = AF_INET;
    caddr.sin_port = htons(port);
    caddr.sin_addr.s_addr = INADDR_ANY; // or inet_addr("127.0.0.1"); for localhost
    r = bind(cid, (struct sockaddr *)&caddr, sizeof(caddr));
    if (r < 0)
    {
        perror("Bind failed\n");
        exit(0);
    }
    n = sizeof(caddr);
    printf("Server is running\n");
    do
    {
        strncpy(buf, "", 32);
        printf("Waiting for client\n");
        if (byrc = recvfrom(cid, buf, 32, 0, (struct sockaddr *)&caddr, &n) <= 0)
        {
            perror("Receive failed\n");
            exit(0);
        }
        printf("Received message: %s\n", buf);
        if (strcmp(echobuf, "exit") == 0 || strcmp(buf, "exit") == 0)
        {
            close(cid);
            exit(0);
        }
        strncpy(echobuf, "", 32);
        printf("Enter message to send: ");
        scanf("%s", echobuf);
        sendto(cid, echobuf, 32, 0, (struct sockaddr *)&caddr, n);
        if (strcmp(echobuf, "exit") == 0 || strcmp(buf, "exit") == 0)
        {
            close(cid);
            exit(0);
        }
    } while (1);
    close(cid);
}