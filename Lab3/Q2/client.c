#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define max 100

int main()
{
    int sockfd, retval;
    int sendbytes;
    socklen_t actuallen;
    struct sockaddr_in serveraddr, clientaddr;
    char buff[max];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        printf("Error Creating Socket");
        exit(0);
    }

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(3223);
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    retval = connect(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    if (retval == -1)
    {
        printf("Error Connecting to server..");
        exit(0);
    }

    while (1)
    {
        memset(buff, '\0', sizeof(buff));
        printf("Enter String: ");
        fgets(buff, sizeof(buff), stdin);
        if (strcmp(buff, "stop") == 0)
        {
            printf("Closing Connection..");
            exit(0);
        }
        sendbytes = send(sockfd, buff, sizeof(buff), 0);
        if (sendbytes == -1)
        {
            printf("Error Sending message");
            break;
        }
    }
    exit(0);
    return 0;
}
