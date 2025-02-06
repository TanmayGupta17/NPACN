#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define max 100

int main()
{
    int sockfd, retval;
    int recedbytes, sendbytes;
    socklen_t actuallen;
    struct sockaddr_in serveraddr, clientaddr;
    char buff[max];
    int id = 0;

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
        printf("Error Accepting Connection");
        exit(0);
    }

    while (1)
    {
        memset(buff, '\0', sizeof(buff));
        printf("Enter Message: ");
        fgets(buff, sizeof(buff), stdin);
        sendbytes = send(sockfd, buff, sizeof(buff), 0);
        if (sendbytes == -1)
        {
            printf("Error Sending Message");
            break;
        }

        recedbytes = recv(sockfd, &id, sizeof(id), 0);
        if (recedbytes == -1)
        {
            printf("Error recieving message");
            break;
            ;
        }
        printf("Process id: %d\n", id);

        recedbytes = recv(sockfd, buff, sizeof(buff), 0);
        if (recedbytes == -1)
        {
            printf("Error recieving message");
            break;
        }
        printf("%s\n", buff);

        recedbytes = recv(sockfd, &id, sizeof(id), 0);
        if (recedbytes == -1)
        {
            printf("Error recieving message");
            break;
        }
        printf("Process id: %d\n", id);

        recedbytes = recv(sockfd, buff, sizeof(buff), 0);
        if (recedbytes == -1)
        {
            printf("Error recieving message");
            break;
        }
        printf("%s\n", buff);
    }
    exit(0);
    return 0;
}
