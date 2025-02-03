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
    socklen_t actuallen;
    struct sockaddr_in serveraddr, clientaddr;
    int recedbytes, sendbytes;
    char buff[max];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        printf("Error Creating Socket");
        exit(0);
    }

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(3388);
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    retval = connect(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    if (retval == -1)
    {
        printf("Error Connecting to the port...");
        exit(0);
    }

    actuallen = sizeof(serveraddr);
    pid_t p = fork();
    if (p == 0)
    {
        printf("Child process: %d", getpid());
        while (1)
        {
            memset(buff, '\0', sizeof(buff));
            printf("Enter Message: ");
            fgets(buff, sizeof(buff), stdin);
            if (strcmp(buff, "Halt") == 0)
            {
                printf("Closing server....");
                exit(0);
            }
            sendbytes = send(sockfd, buff, sizeof(buff), 0);
            if (sendbytes == -1)
            {
                printf("Error Sending Message");
                break;
            }
        }
    }
    else if (p > 0)
    {
        printf("Parent process: %d", getppid());
        printf("\n");
        while (1)
        {
            memset(buff, '\0', sizeof(buff));
            recedbytes = recv(sockfd, buff, sizeof(buff), 0);
            if (recedbytes == -1)
            {
                printf("Error REceiving message");
                break;
            }
            printf("%s", buff);
            printf("\n");
        }
    }
    exit(0);
    return 0;
}
