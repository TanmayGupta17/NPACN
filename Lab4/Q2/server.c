#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

#define M 100

typedef struct
{
    char name[M];
    char ip[M];
} DNS;

int main()
{
    int sockfd, retval;
    socklen_t actuallen;
    int sendbytes, recedbytes;
    struct sockaddr_in serveraddr, clientaddr;
    char buff[M];
    char name[M];
    char ipaddr[M];
    int choice;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        printf("Socket Creation Error\n");
        exit(EXIT_FAILURE);
    }

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(3000);
    serveraddr.sin_addr.s_addr = INADDR_ANY;

    retval = bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    if (retval == -1)
    {
        printf("Binding Error\n");
        exit(EXIT_FAILURE);
    }

    retval = listen(sockfd, 1);
    if (retval == -1)
    {
        printf("Error Listening ...\n");
        exit(EXIT_FAILURE);
    }

    actuallen = sizeof(clientaddr);
    int newsockfd = accept(sockfd, (struct sockaddr *)&clientaddr, &actuallen);
    if (newsockfd == -1)
    {
        printf("Error Accepting Connection...\n");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        memset(buff, '\0', sizeof(buff));
        choice = 0;
        recedbytes = recv(newsockfd, &choice, sizeof(choice), 0);
        if (recedbytes <= 0)
        {
            printf("Error Receiving Message or Client Disconnected\n");
            break;
        }

        FILE *fp;
        DNS dns;

        switch (choice)
        {
        case 1:
            // Add Info
            recedbytes = recv(newsockfd, name, sizeof(name), 0);
            if (recedbytes <= 0)
            {
                printf("Error Receiving Name\n");
                break;
            }
            recedbytes = recv(newsockfd, ipaddr, sizeof(ipaddr), 0);
            if (recedbytes <= 0)
            {
                printf("Error Receiving IP\n");
                break;
            }

            strcpy(dns.name, name);
            strcpy(dns.ip, ipaddr);

            fp = fopen("DNS.txt", "a");
            if (!fp)
            {
                printf("File not found!!\n");
                break;
            }

            fprintf(fp, "%s %s\n", dns.name, dns.ip);
            printf("Data Added Successfully: %s -> %s\n", dns.name, dns.ip);
            fclose(fp);
            break;

        case 2:
            // Get IP by name
            recedbytes = recv(newsockfd, buff, sizeof(buff), 0);
            if (recedbytes <= 0)
            {
                printf("Error Receiving Name for Lookup\n");
                break;
            }

            fp = fopen("DNS.txt", "r");
            if (!fp)
            {
                printf("File not found!!\n");
                break;
            }

            int found = 0;
            while (fscanf(fp, "%s %s", dns.name, dns.ip) != EOF)
            {
                if (strcmp(dns.name, buff) == 0)
                {
                    send(newsockfd, dns.ip, sizeof(dns.ip), 0);
                    printf("Found: %s -> %s\n", dns.name, dns.ip);
                    found = 1;
                    break;
                }
            }
            if (!found)
            {
                char not_found[] = "Not Found";
                send(newsockfd, not_found, sizeof(not_found), 0);
                printf("Name not found in DNS\n");
            }

            fclose(fp);
            break;

        case 3:
            // Get Name by IP
            recedbytes = recv(newsockfd, buff, sizeof(buff), 0);
            if (recedbytes <= 0)
            {
                printf("Error Receiving IP for Lookup\n");
                break;
            }

            fp = fopen("DNS.txt", "r");
            if (!fp)
            {
                printf("File not found!!\n");
                break;
            }

            found = 0;
            while (fscanf(fp, "%s %s", dns.name, dns.ip) != EOF)
            {
                if (strcmp(dns.ip, buff) == 0)
                {
                    send(newsockfd, dns.name, sizeof(dns.name), 0);
                    printf("Found: %s -> %s\n", dns.ip, dns.name);
                    found = 1;
                    break;
                }
            }
            if (!found)
            {
                char not_found[] = "Not Found";
                send(newsockfd, not_found, sizeof(not_found), 0);
                printf("IP not found in DNS\n");
            }

            fclose(fp);
            break;

        default:
            printf("Enter a valid choice\n");
            break;
        }
    }

    close(newsockfd);
    close(sockfd);
    return 0;
}
