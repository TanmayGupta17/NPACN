#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define M 100

int main()
{
    int sockfd, retval;
    struct sockaddr_in serveraddr, clientaddr;
    int recedbytes, sendbytes;
    char buff[M];
    char name[M];
    int ch;
    char IP[M];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        printf("Error Creating Socket");
        exit(0);
    }

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(3000);
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    retval = connect(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    if (retval == -1)
    {
        printf("Connection Error");
        exit(0);
    }

    while (1)
    {
        memset(buff, '\0', sizeof(buff));
        memset(name, '\0', sizeof(name));
        memset(IP, '\0', sizeof(IP));

        printf("Enter Your Choice:\n1.Add Data to Server\n2.Get IP address\n3.Get Domain Name\n4.Exit\n");
        scanf("%d", &ch);
        getchar();
        printf("\n");
        sendbytes = send(sockfd, &ch, sizeof(ch), 0);
        if (sendbytes == -1)
        {
            printf("Error Sending Message");
            break;
        }
        switch (ch)
        {
        case 1:
            printf("Enter Domain Name: ");
            // fgets(name, sizeof(name), stdin);
            scanf("%s", &name);
            // getchar();
            printf("\n");
            sendbytes = send(sockfd, name, sizeof(name), 0);
            if (sendbytes == -1)
            {
                printf("Error Sending Message");
                break;
            }
            printf("Enter IP Address: ");
            // fgets(IP, sizeof(IP), stdin);
            scanf("%s", &IP);
            sendbytes = send(sockfd, IP, sizeof(IP), 0);
            if (sendbytes == -1)
            {
                printf("Error Sending Message");
                break;
            }
            break;
        case 2:
            printf("Enter Domain Name: ");
            scanf("%s", &name);
            // getchar();
            printf("\n");
            sendbytes = send(sockfd, name, sizeof(name), 0);
            if (sendbytes == -1)
            {
                printf("Error Sending Message");
                break;
            }
            break;
        case 3:
            printf("Enter IP Address: ");
            scanf("%s", &IP);
            sendbytes = send(sockfd, IP, sizeof(IP), 0);
            if (sendbytes == -1)
            {
                printf("Error Sending Message");
                break;
            }
            break;
        case 4:
            exit(0);
            break;
        }
    }
}
