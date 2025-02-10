#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#define max 100

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
    int sockfd, retval;
    int recedbytes, sendbytes;
    struct sockaddr_in serveraddr, clientaddr;
    socklen_t actuallen;
    int opt;
    char buff[max];

    // Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1)
    {
        perror("Error Creating Socket");
        exit(EXIT_FAILURE);
    }

    // Bind socket
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
        recedbytes = recvfrom(sockfd, buff, sizeof(buff), 0, (struct sockaddr *)&clientaddr, &actuallen);
        if (recedbytes == -1)
        {
            printf("Error Recieving Message");
            break;
        }

        opt = buff[0] - '0';
        char query[max];
        strcpy(query, buff + 1);
        int size = sizeof(students) / sizeof(students[0]);
        char response[max];
        int found = 0;

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

                break; // Stop loop once data is found
            }
        }

        if (!found)
        {
            snprintf(response, sizeof(response), "Record Not Found");
        }

        sendbytes = sendto(sockfd, response, sizeof(response), 0, (struct sockaddr *)&clientaddr, actuallen);
        if (sendbytes == -1)
        {
            printf("Error Sending Message");
            break;
        }
    }
    exit(0);
    return 0;
}
