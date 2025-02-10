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
    socklen_t actuallen;
    char buff[MAX];
    char query[MAX];
    int option;

    // Create socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1)
    {
        perror("Error Creating Socket");
        exit(EXIT_FAILURE);
    }

    // Define server address
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(3223);
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Localhost

    actuallen = sizeof(serveraddr);

    // Infinite loop for continuous queries
    while (1)
    {
        // Input query option and string
        printf("\nEnter the query option (1 - Reg No, 2 - Name, 3 - Subject Code, 0 to Exit): ");
        if (scanf("%d", &option) != 1) // Prevent invalid input
        {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n')
                ; // Clear input buffer
            continue;
        }
        getchar(); // Consume newline character left by scanf

        if (option == 0)
        {
            printf("Exiting client...\n");
            break; // Exit the loop if user chooses to exit
        }

        printf("Enter the query string: ");
        fgets(query, sizeof(query), stdin);
        query[strcspn(query, "\n")] = 0; 

        snprintf(buff, sizeof(buff), "%d%s", option, query);
        if (sendto(sockfd, buff, strlen(buff) + 1, 0, (struct sockaddr *)&serveraddr, actuallen) == -1)
        {
            perror("Error Sending Message");
            close(sockfd);
            exit(EXIT_FAILURE);
        }

        // Receive the server's response
        int recedbytes = recvfrom(sockfd, buff, sizeof(buff) - 1, 0, (struct sockaddr *)&serveraddr, &actuallen);
        if (recedbytes == -1)
        {
            perror("Error Receiving Message");
            exit(EXIT_FAILURE);
        }

        buff[recedbytes] = '\0'; // Ensure null termination
        printf("Server Response: \n%s\n", buff);
    }

    // Close the socket
    close(sockfd);
    return 0;
}
