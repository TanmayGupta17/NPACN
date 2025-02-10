#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#define MAX 100

struct Student
{
    char reg_no[10];
    char name[50];
    char address[100];
    char dept[50];
    int semester;
    char section[5];
    char courses[200];
    char subject_code[10];
    int marks;
};

// Sample data
struct Student students[] = {
    {"220953470", "Tanmay Gupta", "XYZ Street, City", "CCE", 5, "A", "DSA, OS, Networks", "DSA", 85},
    {"220953471", "Alex Kumar", "ABC Road, Town", "IT", 4, "B", "DBMS, CN, AI", "DBMS", 90},
    {"220953472", "Sanya Mehta", "LMN Avenue, Metro", "CSE", 3, "C", "ML, DL, Big Data", "ML", 88},
    {"220953473", "Rohan Sharma", "PQR Lane, Village", "ECE", 2, "D", "Circuits, Microprocessors, Signals", "Circuits", 82},
    {"220953474", "Priya Verma", "UVW Colony, City", "ME", 6, "A", "Thermodynamics, Mechanics, Fluid Dynamics", "Mechanics", 89},
    {"220953475", "Aryan Singh", "EFG Street, Town", "EEE", 4, "B", "Power Systems, Control Systems, Electronics", "Power Systems", 91},
    {"220953476", "Neha Kapoor", "RST Apartments, Metro", "CSE", 5, "C", "Web Dev, Cyber Security, AI", "Cyber Security", 87},
    {"220953477", "Rahul Nair", "JKL Nagar, City", "IT", 3, "D", "Data Science, Blockchain, IoT", "Blockchain", 80}};

int main()
{
    int sockfd, newsockfd;
    socklen_t actuallen;
    struct sockaddr_in serveraddr, clientaddr;
    int recedbytes, sendbytes;
    char buff[MAX];

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        perror("Error Creating Socket");
        exit(EXIT_FAILURE);
    }

    // Bind socket
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(3223);
    serveraddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) == -1)
    {
        perror("Binding Error");
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if (listen(sockfd, 5) == -1)
    {
        perror("Error Listening to the socket");
        exit(EXIT_FAILURE);
    }

    // printf("Server listening on port 5000...\n");

    while (1)
    {
        actuallen = sizeof(clientaddr);
        newsockfd = accept(sockfd, (struct sockaddr *)&clientaddr, &actuallen);
        if (newsockfd == -1)
        {
            perror("Accept Error");
            continue;
        }

        printf("Connected to client...\n");

        pid_t p = fork();
        if (p == 0)
        {                  // Child process
            close(sockfd); // Close server socket in child
            memset(buff, '\0', sizeof(buff));

            int recedbytes = recv(newsockfd, buff, sizeof(buff), 0);
            if (recedbytes == -1)
            {
                perror("Error Receiving Message");
                exit(EXIT_FAILURE);
            }

            int opt = buff[0] - '0'; // Extract option number
            char query[50];
            strcpy(query, buff + 1); // Extract actual input string
            int size = sizeof(students) / sizeof(students[0]);

            char response[1024] = {0}; // Response buffer
            int found = 0;             // Flag to check if record found

            for (int i = 0; i < size; i++)
            {
                switch (opt)
                {
                case 1: // Query by Registration Number
                    if (strcmp(query, students[i].reg_no) == 0)
                    {
                        snprintf(response, sizeof(response), "Name: %s\nAddress: %s\nPID: %d",
                                 students[i].name, students[i].address, getpid());
                        found = 1;
                    }
                    break;

                case 2: // Query by Name
                    if (strcmp(query, students[i].name) == 0)
                    {
                        snprintf(response, sizeof(response), "Dept: %s\nSemester: %d\nSection: %s\nCourses: %s\nPID: %d",
                                 students[i].dept, students[i].semester, students[i].section, students[i].courses, getpid());
                        found = 1;
                    }
                    break;

                case 3: // Query by Subject Code
                    if (strcmp(query, students[i].subject_code) == 0)
                    {
                        snprintf(response, sizeof(response), "Marks: %d\nPID: %d",
                                 students[i].marks, getpid());
                        found = 1;
                    }
                    break;

                default:
                    snprintf(response, sizeof(response), "Invalid Option");
                    found = 1;
                    break;
                }

                if (found)
                {
                    sendbytes = send(newsockfd, response, sizeof(response), 0);
                    if (sendbytes == -1)
                    {
                        printf("Error Sending Message");
                        break;
                    }
                    break; // Stop loop once data is found
                }
            }

            if (!found)
            {
                snprintf(response, sizeof(response), "Record Not Found");
            }

            // Send response to client
            if (send(newsockfd, response, strlen(response), 0) == -1)
            {
                perror("Error Sending Message");
            }

            close(newsockfd);
            exit(0); // Exit child process
        }
        else
        {
            close(newsockfd); // Close client socket in parent
        }
    }

    return 0;
}
