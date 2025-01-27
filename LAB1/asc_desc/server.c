#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#define MAX 90
#define ARRAY_SIZE 5

// Function to sort array in ascending order
void ascending(int *arr)
{
    for (int i = 0; i < ARRAY_SIZE - 1; i++)
    {
        for (int j = i + 1; j < ARRAY_SIZE; j++)
        {
            if (arr[i] > arr[j])
            {
                int temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
    }
}

// Function to sort array in descending order
void descending(int *arr)
{
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        for (int j = i + 1; j < ARRAY_SIZE; j++)
        {
            if (arr[i] < arr[j])
            {
                int temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
    }
}

// Function to split array into even numbers
void even(int *arr, int *evenarr)
{
    int j = 0;
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        if (arr[i] % 2 == 0)
        {
            evenarr[j++] = arr[i];
        }
    }
}

// Function to split array into odd numbers
void odd(int *arr, int *oddarr)
{
    int j = 0;
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        if (arr[i] % 2 != 0)
        {
            oddarr[j++] = arr[i];
        }
    }
}

int main()
{
    int sockfd, newsockfd, retval;
    socklen_t actuallen;
    int recedbytes, sendbytes;
    struct sockaddr_in serveraddr, clientaddr;
    int arr[MAX];
    char buff[MAX];
    char command[MAX]; // to store the command sent by the client

    // Create the socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        printf("Socket Creation Error\n");
        exit(0);
    }

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(5000);
    serveraddr.sin_addr.s_addr = htons(INADDR_ANY);

    // Bind the socket to the port
    retval = bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    if (retval == -1)
    {
        printf("Binding Error\n");
        close(sockfd);
        exit(0);
    }

    // Listen for incoming connections
    retval = listen(sockfd, 1);
    if (retval == -1)
    {
        printf("Listen Error\n");
        close(sockfd);
        exit(0);
    }

    actuallen = sizeof(clientaddr);
    newsockfd = accept(sockfd, (struct sockaddr *)&clientaddr, &actuallen);
    if (newsockfd == -1)
    {
        printf("Accept Error\n");
        close(sockfd);
        exit(0);
    }

    while (1)
    {
        memset(buff, '\0', sizeof(buff));
        memset(command, '\0', sizeof(command));

        // Receive the array from the client
        recedbytes = recv(newsockfd, arr, sizeof(arr), 0);
        if (recedbytes == -1)
        {
            printf("Error Receiving Array\n");
            close(sockfd);
            close(newsockfd);
            break;
        }

        // Print received array
        printf("Received Array: ");
        for (int i = 0; i < ARRAY_SIZE; i++)
        {
            printf("%d ", arr[i]);
        }
        printf("\n");

        // Receive the command (ascending, descending, even, odd)
        recedbytes = recv(newsockfd, command, sizeof(command), 0);
        if (recedbytes == -1)
        {
            printf("Error Receiving Command\n");
            close(sockfd);
            close(newsockfd);
            break;
        }

        // Print the received command
        printf("Received Command: %s\n", command);

        // Process the command
        if (strncmp(command, "ASCENDING", 9) == 0)
        {
            ascending(arr);
        }
        else if (strncmp(command, "DESCENDING", 10) == 0)
        {
            descending(arr);
        }
        else if (strncmp(command, "EVEN", 4) == 0)
        {
            int evenarr[ARRAY_SIZE];
            even(arr, evenarr);
            for (int i = 0; i < 5; i++)
            {
                printf("%d ", arr[i]);
            }
            sendbytes = send(newsockfd, evenarr, sizeof(evenarr), 0);
            if (sendbytes == -1)
            {
                printf("Error Sending Even Array\n");
                close(sockfd);
                close(newsockfd);
                break;
            }
            printf("Sent Even Array\n");
            continue; // Skip sending the main array
        }
        else if (strncmp(command, "ODD", 3) == 0)
        {
            int oddarr[ARRAY_SIZE];
            odd(arr, oddarr);
            for (int i = 0; i < 5; i++)
            {
                printf("%d ", arr[i]);
            }
            sendbytes = send(newsockfd, oddarr, sizeof(oddarr), 0);
            if (sendbytes == -1)
            {
                printf("Error Sending Odd Array\n");
                close(sockfd);
                close(newsockfd);
                break;
            }
            printf("Sent Odd Array\n");
            continue; // Skip sending the main array
        }
        else
        {
            printf("Unknown Command\n");
            continue;
        }

        // Send the processed array back to the client
        sendbytes = send(newsockfd, arr, sizeof(arr), 0);
        if (sendbytes == -1)
        {
            printf("Error Sending Array\n");
            close(sockfd);
            close(newsockfd);
            break;
        }
    }

    close(newsockfd);
    close(sockfd);
    return 0;
}
