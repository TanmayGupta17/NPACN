#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define max 90

int main()
{
    int recedbytes, sendbytes;
    int retval;
    char buff[max];
    int val = 0;
    struct sockaddr_in serveraddr;
    int sockfd;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        printf("Error creating socket\n");
        exit(0);
    }

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(3388);
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // ✅ Corrected

    retval = connect(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    if (retval == -1)
    {
        printf("Connection Error\n");
        exit(0);
    }

    while (1)
    {
        memset(buff, '\0', sizeof(buff));
        printf("Enter Message: ");
        fgets(buff, sizeof(buff), stdin);

        buff[strcspn(buff, "\n")] = '\0'; // ✅ Remove newline

        if (strcmp(buff, "Halt") == 0)
        {
            break;
        }

        sendbytes = send(sockfd, buff, strlen(buff) + 1, 0); // ✅ Send only required bytes
        if (sendbytes == -1)
        {
            printf("Error Sending Message\n");
            break;
        }

        recedbytes = recv(sockfd, buff, sizeof(buff), 0);
        if (recedbytes == -1)
        {
            printf("Error Receiving Message\n");
            break;
        }
        printf("%s\n", buff);

        recedbytes = recv(sockfd, &val, sizeof(val), 0); // ✅ Pass pointer
        if (recedbytes == -1)
        {
            printf("Error Receiving Message\n");
            break;
        }
        printf("Vowel Count: %d\n", val);
    }
    return 0;
}
