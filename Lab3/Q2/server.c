#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define max 100

void swap(char *x, char *y)
{
    char temp;
    temp = *x;
    *x = *y;
    *y = temp;
}

void permute(char *a, int l, int r)
{
    int i;
    if (l == r) // Base case: we have a complete permutation
    {
        printf("%s\n", a); // Print the permutation when the full string is formed
    }
    else
    {
        for (i = l; i <= r; i++)
        {
            // Swap the current character with the next
            swap((a + l), (a + i));

            // Recurse for the next character
            permute(a, l + 1, r);

            // Backtrack: Undo the swap to explore other possibilities
            swap((a + l), (a + i));
        }
    }
}

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
        printf("Error Listeneing..");
        exit(0);
    }

    actuallen = sizeof(clientaddr);
    int newsockfd;
    newsockfd = accept(sockfd, (struct sockaddr *)&clientaddr, &actuallen);
    if (newsockfd == -1)
    {
        printf("Error Accepting Connection");
        exit(0);
    }

    while (1)
    {
        memset(buff, '\0', sizeof(buff));
        // printf("Enter Message: ");
        // fgets(buff,sizeof(buff),stdin);
        recedbytes = recv(newsockfd, buff, sizeof(buff), 0);
        if (recedbytes == -1)
        {
            printf("Error Recieving Message");
            break;
        }

        printf("Recieved Message : %s", buff);
        int len = 0;
        for (; buff[len] != '\0'; len++)
            ;
        permute(buff, 0, len - 1);
    }
    exit(0);
    return 0;
}
