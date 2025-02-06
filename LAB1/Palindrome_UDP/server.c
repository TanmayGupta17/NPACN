#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>

#define max 90

bool isPalindrome(char *c)
{
    int len = strlen(c);
    for (int i = 0; i < len / 2; i++)
    {
        if (c[i] != c[len - i - 1])
            return false;
    }
    return true;
}

int Vowels(char *c)
{
    int count = 0;
    int len = strlen(c);
    for (int i = 0; i < len; i++)
    {
        if (c[i] == 'a' || c[i] == 'e' || c[i] == 'i' || c[i] == 'o' || c[i] == 'u')
            count++;
    }
    return count;
}

int main()
{
    int recedbytes, sendbytes;
    int retval, sockfd;
    struct sockaddr_in serveraddr, clientaddr;
    socklen_t actuallen;
    char buff[max];
    char command[max];

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1)
    {
        printf("Error Creating Socket");
        exit(0);
    }

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(3223);
    serveraddr.sin_addr.s_addr = INADDR_ANY;

    retval = bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    if (retval == -1)
    {
        printf("Binding Error");
        exit(0);
    }

    actuallen = sizeof(clientaddr);

    while (1)
    {
        memset(buff, '\0', sizeof(buff));
        memset(command, '\0', sizeof(command));

        recedbytes = recvfrom(sockfd, buff, sizeof(buff), 0,(struct sockaddr*)&clientaddr,&actuallen);
        if (recedbytes == -1)
        {
            printf("Error Receving message");
            break;
        }
        printf("Message Received: ");
        printf("%s", buff);

        // recedbytes = recv(newsockfd,command,sizeof(command),0);
        // if(recedbytes == -1){
        //     printf("Error Receving message");
        //     break;
        // }
        // printf("Command received");
        bool flag = isPalindrome(buff);
        if (flag)
        {
            strcpy(command, "true");
        }
        else
        {
            strcpy(command, "false");
        }

        sendbytes = sendto(sockfd, command, sizeof(command), 0, (struct sockaddr*)&clientaddr,actuallen);
        if (sendbytes == -1)
        {
            printf("Error sending message");
            break;
        }
        memset(command, '\0', sizeof(command));
        int noofvowels = Vowels(buff);
        sendbytes = sendto(sockfd, &noofvowels, sizeof(noofvowels), 0, (struct sockaddr*)&clientaddr,actuallen);
        if (sendbytes == -1)
        {
            printf("Error Sending message");
            break;
        }
    }
}