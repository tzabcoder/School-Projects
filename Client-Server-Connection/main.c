/* --------------------------------------------------------------------------------
Trevor Zabilowicz

CET 440 - Computer Networking 
Server/Client Project - Phase 2
11/11/2021

FILE: main.c

DESCRIPTION:
This program access the server via a socket created on the client machine. The 
Client connects to the Server and sends command requests.
The Client recieves the responses from the server, based on the pre-defined protocol.
Client displays the server responses in a user-friendly format.
-------------------------------------------------------------------------------- */

//File Includes 
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include "FileOper.h"

//Program Definitions
#define RCVBUFSIZE 1024       //Size of receiving buffer
#define SNDBUFSIZE 1024       //Size of sending buffer

int main(int argc, char *argv[]) {

    //Program Variables 
    int servSock = 0;
    int n = 0;
    int i = 0;
    int info_count;
    char recvBuff[RCVBUFSIZE];    //Receiving buffer
    char sendBuff[SNDBUFSIZE];    //Sending buffer
    char command[20];
    struct sockaddr_in serv_addr;
    int quit = 0; 
    int mistake = 0;
    int bytesRcvd;
    char servIP[16];              //Server IP address 
    int port;                     //Port used to send data
    char temp_buff[255];
    char* MyInfoDetails[] = {"Firstname: ", "Lastname: ", "Age: ", "GPA: ", "IP Address: "};

    //User input for port number 
    printf("Please input the port to connect: ");
    scanf("%d", &port);

    strcpy(servIP, "158.83.254.100");      //Copy Draco1 IP address into servIP

    //Attempt to create a Socket
    if ((servSock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Error: Could not create socket... \n");
        return 1;
    }
    //Else, Socket was created successfully

    memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;                    //Internet Address family
    serv_addr.sin_addr.s_addr = inet_addr(servIP);     //Server IP Address
    serv_addr.sin_port = htons(port);                  //Server port

    //Attempt to connect to the server 
    if (connect(servSock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\n Error: Could not connect to the server... \n");
        return 1;
    }
    //Else, Connected to the server

    //Attempt to receive data from the server 
    if ((bytesRcvd = recv(servSock, recvBuff, RCVBUFSIZE + 1, 0)) <= 0) {
        printf("\n Error: recv() failed or connection closed prematurely...\n");
    }
    //Else, bytes recived 

    //Try to read data from the LogDate file
    strcpy(temp_buff, readFile());
    if (strlen(temp_buff) == 0) {
        printf("User has not logged in before... this is the fist time!\n");
    }
    else {
        printf("Last log on date: %s\n", temp_buff);
    }

    do {
        //Input Propmt
        if (strcmp(sendBuff, "MyInfo") == 0) {
            //Do nothing
        }
        else {
            //Else Print the receive buffer 
            printf("Server Echo: %s\n", recvBuff);
        }

        //Prompt for username if the command is to Register or Login
        if ((strcmp(sendBuff, "Login") == 0) || (strcmp(sendBuff, "Register") == 0)) {
            printf("Username: ");
            scanf("%s", &sendBuff);
        }
        //Otherwise prompt for command
        else {
            printf("Enter Command: ");
            scanf("%s", &sendBuff);
        }

        //Send to Server
        if (send(servSock, sendBuff, sizeof(sendBuff), 0) < 0) {
            printf("Disconnected from server...\n");
            bytesRcvd = 0;
        }
        else {
            memset(&recvBuff, '0', sizeof(recvBuff));
            if ((bytesRcvd = recv(servSock, recvBuff, RCVBUFSIZE + 1, 0)) <= 0) {
                printf("Error: recv() failed or connection closed prematurely...\n");
            }
        }   

        //If the entered command is Quit
        if (strcmp(sendBuff, "Quit") == 0) {
            bytesRcvd = -1;
            printf("%s\n", recvBuff);
            writeFile();
        }

        //Format output for MyInfo command
        if (strcmp(sendBuff, "MyInfo") == 0) {
            info_count = 0;
            for (i = 0; i < strlen(recvBuff)-1; i++){
                if (recvBuff[i] == 10) {      //Check if space
                    printf("\n");
                    printf("%s", MyInfoDetails[info_count]);
                    info_count++;
                }
                else {
                    printf("%c", recvBuff[i]);
                }
            }
            printf("\n\n");
        }

    } while (bytesRcvd > 0);

    close(servSock);     //Close the server socket
    return 0;
}
