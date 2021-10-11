//Server implementation in C++
//Used for a client server chant program

//Server Libraries
#include <iostream>
#include <string>
#include <fstream>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <netdb.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

//Server Definitions
#define BUFFER_LEN   1500
#define MAX_REQUESTS 5

//Server Side 
int main(int argc, char *argv[]) {
    
    //Get the server port number
    if(argc != 2) {
        cerr << "Usage: port" << endl;
        exit(0);
    }

    //Get the port number
    int port = atoi(argv[1]);
    //Create message buffer
    char msg[BUFFER_LEN];    

    //Create the socket and the connection tools 
    sockaddr_in servAddr;
    bzero((char*)&servAddr, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr= htonl(INADDR_ANY);
    servAddr.sin_port = htons(port);

    //Open stream socket with internet address (keep track of socket descriptor)
    int serverSd = socket(AF_INET, SOCK_STREAM, 0);
    if(serverSd < 0) {
        cerr << "Error establishing the server socket..." << endl;
        exit(0);
    }

    //Bind socket to local address
    int bindStatus = bind(serverSd, (struct sockaddr*)&servAddr, sizeof(servAddr));
    if(bindStatus < 0) {
        cerr << "Error binding the socket to the local address..." << endl;
        exit(0);
    }

    //Otherwise the socket was created successfully
    cout << "Waiting for client connection..." << endl;

    //Listen for 5 requests at a time 
    listen(serverSd, MAX_REQUESTS);

    //Recieve a request from the client using accept 
    //New address to connect with the client
    sockaddr_in newSockAddr;
    socklen_t newSockAddrSize = sizeof(newSockAddr);

    //Accept the request and create a new socket descriptor to handle new connection with client 
    int newSd = accept(serverSd, (sockaddr*)&newSockAddr, &newSockAddrSize);
    if(newSd < 0) {
        cerr << "Error accepting request from client..." << endl;
        exit(0);
    }
    
    //Else the connection was made 
    cout << "Connected..." << endl;

    //Keep track of session time 
    struct timeval start1, end1;
    gettimeofday(&start1, NULL);

    //Keep track of the amount of data
    int bytesRead, bytesWritten = 0;

    while(1) {
        //Listen for a message from the client
        cout << "Awaiting client response..." << endl;

        memset(&msg, 0, sizeof(msg)); //clear the buffer
        //Recieve message from client
        bytesRead += recv(newSd, (char*)&msg, sizeof(msg), 0); //Read number if bytes read

        //See if client requested to terminate session
        if(!strcmp(msg, "exit")) {
            cout << "Client terminated session..." << endl;
            break;
        }

        //Else print the client message 
        cout << "Client: " << msg << endl;

        //Message to send to client
        cout << ">";
        string data;
        getline(cin, data);

        //Clear the buffer
        memset(&msg, 0, sizeof(msg));
        strcpy(msg, data.c_str()); //Copy the data into the buffer

        if(data == "exit") {    
            //Server terminates session
            //Send to client that the server has terminated the session
            send(newSd, (char*)&msg, strlen(msg), 0);
            break;
        }

        //Send message to client
        bytesWritten += send(newSd, (char*)&msg, strlen(msg), 0);
    }

    //Close all socket descriptors when finished
    gettimeofday(&end1, NULL);
    close(newSd);
    close(serverSd);
    cout << "********** Session **********" << endl;
    cout << "Bytes Written: " << bytesWritten << endl;
    cout << "Bytes Read: " << bytesRead << endl;
    cout << "Elapsed Time: " << (end1.tv_sec - start1.tv_sec) << " seconds" << endl;
    cout << "Connection closed..." << endl;

    return 0;
}   