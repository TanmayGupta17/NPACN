#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
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
    int newsockfd;
    char buff[max];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        printf("Error Creating Socket");
        exit(0);
    }

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(3388);
    serveraddr.sin_addr.s_addr = INADDR_ANY;

    retval = bind(sockfd, (struct sockadrr *)&serveraddr, sizeof(serveraddr));
    if (retval == -1)
    {
        printf("Binding Error");
        exit(0);
    }

    retval = listen(sockfd, 1);
    if (retval == -1)
    {
        printf("Error Listening");
        exit(0);
    }
    actuallen = sizeof(clientaddr);
    newsockfd = accept(sockfd, (struct sockaddr *)&clientaddr, &actuallen);
    if (newsockfd == -1)
    {
        printf("Error Accepting Request");
        exit(0);
    }

    pid_t p = fork();
    if (p == 0)
    {
        printf("Child process: %d", getpid());
        printf("\n");
        while(1){
            memset(buff,'\0',sizeof(buff));
            recedbytes = recv(newsockfd,buff,sizeof(buff),0);
            if(recedbytes == -1){
                printf("Error REceiving message");
                break;
            }
            printf("%s",buff);
            printf("\n");
        }
        
    }
    else if(p>0){
        printf("Parent process: %d", getppid());
        while(1){
            memset(buff,'\0',sizeof(buff));
            printf("Enter Message: ");
            fgets(buff,sizeof(buff),stdin);
            if(strcmp(buff,"Halt") == 0){
                printf("Closing server....");
                exit(0);
            }
            sendbytes = send(newsockfd,buff,sizeof(buff),0);
            if(sendbytes == -1){
                printf("Error Sending Message");
                break;
            }
        }
    }

    close(sockfd);
    close(newsockfd);
    exit(0);
    return 0;
}
