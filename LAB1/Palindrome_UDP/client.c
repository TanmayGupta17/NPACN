#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdlib.h>

#define max 90

int main()
{
    int recedbytes, sendbytes;
    int retval;
    int sockfd;
    struct sockaddr_in serveraddr;
    socklen_t actuallen;
    char buff[max];
    int val = 0;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1)
    {
        printf("Error Creating Socket");
        exit(0);
    }

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(3223);
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    actuallen = sizeof(serveraddr);
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

        sendbytes = sendto(sockfd, buff, strlen(buff) + 1, 0, (struct sockaddr *)&serveraddr, actuallen); // ✅ Send only required bytes
        if (sendbytes == -1)
        {
            printf("Error Sending Message\n");
            break;
        }

        recedbytes = recvfrom(sockfd, buff, sizeof(buff), 0, (struct sockaddr *)&serveraddr, &actuallen);
        if (recedbytes == -1)
        {
            printf("Error Receiving Message\n");
            break;
        }
        printf("%s\n", buff);

        recedbytes = recvfrom(sockfd, &val, sizeof(val), 0, (struct sockaddr *)&serveraddr, &actuallen); // ✅ Pass pointer
        if (recedbytes == -1)
        {
            printf("Error Receiving Message\n");
            break;
        }
        printf("Vowel Count: %d\n", val);
    }
}