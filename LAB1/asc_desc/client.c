#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#define max 90

int main()
{
    int arr[max];
    char buff[max];
    int sockfd, retval;
    int recedbytes, sendbytes;
    struct sockaddr_in serveraddr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        printf("Socket Creation Error");
        exit(0);
    }

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(5000);
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    retval = connect(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    if (retval == -1)
    {
        printf("Connection Error");
        close(sockfd);
        exit(0);
    }

    while (1)
    {
        memset(buff, '\0', sizeof(buff));
        printf("Enter Array:");
        for (int i = 0; i < 5; i++)
        {
            scanf("%d", &arr[i]);
        }
        getchar();
        sendbytes = send(sockfd, arr, sizeof(arr), 0);
        if (sendbytes == -1)
        {
            printf("Error Sending message");
            close(sockfd);
        }
        for (int i = 0; i < 5; i++)
        {
            printf("%d ", arr[i]);
        }

        fgets(buff, sizeof(buff), stdin);
        int s = strlen(buff) * sizeof(char);
        sendbytes = send(sockfd, buff, s, 0);
        if (sendbytes == -1)
        {
            printf("Error Sending message");
            close(sockfd);
        }

        recedbytes = recv(sockfd, arr, sizeof(arr), 0);
        for (int i = 0; i < 5; i++)
        {
            printf("%d", arr[i]);
        }
    }
    close(sockfd);
    return 0;
}
