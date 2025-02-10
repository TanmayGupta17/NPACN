#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX 100

int main()
{
    int sockfd;
    struct sockaddr_in serveraddr;
    char buff[MAX];
    char query[MAX];
    int option;

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        perror("Error Creating Socket");
        exit(EXIT_FAILURE);
    }

    // Define server address
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(3223);
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Localhost

    // Connect to the server
    if (connect(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) == -1)
    {
        perror("Error Connecting to Server");
        exit(EXIT_FAILURE);
    }

    // Infinite loop for continuous queries
    while (1)
    {
        // Input query option and string
        printf("\nEnter the query option (1 - Reg No, 2 - Name, 3 - Subject Code, 0 to Exit): ");
        scanf("%d", &option);
        getchar(); // Consume newline character left by scanf

        if (option == 0)
        {
            printf("Exiting client...\n");
            break; // Exit the loop if user chooses to exit
        }

        printf("Enter the query string: ");
        fgets(query, sizeof(query), stdin);
        query[strcspn(query, "\n")] = 0; // Remove newline character

        // Send query to server
        snprintf(buff, sizeof(buff), "%d%s", option, query);
        if (send(sockfd, buff, strlen(buff), 0) == -1)
        {
            perror("Error Sending Message");
            close(sockfd);
            exit(EXIT_FAILURE);
        }

        // Receive the server's response
        int recedbytes = recv(sockfd, buff, sizeof(buff), 0);
        if (recedbytes == -1)
        {
            perror("Error Receiving Message");
            exit(EXIT_FAILURE);
        }

        // Print the response from server
        printf("Server Response: \n%s\n", buff);
    }

    // Close the socket
    close(sockfd);

    return 0;
}
