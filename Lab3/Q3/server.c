#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>

#define max 100

#include <stdio.h>
#include <string.h>

void ascending(char *c)
{
    int len = strlen(c);
    for (int i = 0; i < len - 1; i++)
    {
        for (int j = 0; j < len - i - 1; j++) // Corrected loop condition
        {
            if (c[j] > c[j + 1]) // Compare adjacent elements
            {
                char temp = c[j];
                c[j] = c[j + 1];
                c[j + 1] = temp;
            }
        }
    }
}

void descending(char *c)
{
    int len = strlen(c);
    for (int i = 0; i < len - 1; i++)
    {
        for (int j = 0; j < len - i - 1; j++) // Corrected loop condition
        {
            if (c[j] < c[j + 1]) // Swap for descending order
            {
                char temp = c[j];
                c[j] = c[j + 1];
                c[j + 1] = temp;
            }
        }
    }
}

int main()
{
    int sockfd, retval, newsockfd;
    int recedbytes, sendbytes;
    socklen_t actuallen;
    struct sockaddr_in serveraddr, clientaddr;
    char alphabets[max];
    char numbers[max];
    char buff[max];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        printf("Error creating Socket");
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

    retval = listen(sockfd, 1);
    if (retval == -1)
    {
        printf("Error Listening..");
        exit(0);
    }
    actuallen = sizeof(clientaddr);
    newsockfd = accept(sockfd, (struct sockaddr *)&clientaddr, &actuallen);
    if (newsockfd == -1)
    {
        printf("Error Accepting connection..");
        exit(0);
    }

    while (1)
    {
        memset(buff, '\0', sizeof(buff));
        memset(alphabets, '\0', sizeof(alphabets));
        memset(numbers, '\0', sizeof(numbers));

        recedbytes = recv(newsockfd, buff, sizeof(buff), 0);
        if (recedbytes == -1)
        {
            printf("Error Receiving Message...");
            break;
        }

        int j = 0;
        int k = 0;
        for (int i = 0; i < sizeof(buff); i++)
        {
            if (buff[i] >= '0' && buff[i] <= '9')
            {
                numbers[j++] = buff[i];
            }
            else if (buff[i] >= 'a' && buff[i] <= 'z')
            {
                alphabets[k++] = buff[i];
            }
        }

        pid_t p = fork();
        if (p == 0)
        {
            int cid = getpid();
            printf("Child Process: %d\n", cid);
            sendbytes = send(newsockfd, &cid, sizeof(cid), 0);
            if (sendbytes == -1)
            {
                printf("Error Sending Message");
                break;
            }
            ascending(numbers);
            sendbytes = send(newsockfd, numbers, sizeof(numbers), 0);
            if (sendbytes == -1)
            {
                printf("Error Sending Message");
                break;
            }
        }
        else if (p > 0)
        {
            int pid = getppid();
            printf("Parent Process: %d\n", pid);
            sendbytes = send(newsockfd, &pid, sizeof(pid), 0);
            if (sendbytes == -1)
            {
                printf("Error Sending Message");
                break;
            }
            descending(alphabets);
            sendbytes = send(newsockfd, alphabets, sizeof(alphabets), 0);
            if (sendbytes == -1)
            {
                printf("Error Sending Message");
                break;
            }
        }
    }
    exit(0);
    return 0;
}
