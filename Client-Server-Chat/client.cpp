//Client implementaion in C++
//Used for the client server chat program

//Client Libraries
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

//Client Definitions 
#define BUFFER_LEN 1500  

//Client Side
int main(int argc, char *argv[]) {
    //Need IP address and source port
    if(argc != 3) {
        cerr << "Usage: ip_address port" << endl;
        exit(0);
    }

    //Get the IP address and port number
    char* serverIP = argv[1];
    int port = atoi(argv[2]);

    //Message buffer 
    char msg[BUFFER_LEN];

    //Create socket and connetion tools
    struct hostent* host = gethostbyname(serverIP);
    sockaddr_in sendSockAddr;
    bzero((char*)&sendSockAddr, sizeof(sendSockAddr));
    sendSockAddr.sin_family = AF_INET;
    sendSockAddr.sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr*)*host->h_addr_list));
    sendSockAddr.sin_port = htons(port);

    //Open client stream socket
    int clientSd = socket(AF_INET, SOCK_STREAM, 0);

    //Try to connect to the server
    int status = connect(clientSd, (sockaddr*)&sendSockAddr, sizeof(sendSockAddr));
    if(status < 0) {
        cerr << "Error connecting to the server..." << endl;
        exit(0);
    }

    //Otherwise a connection was made 
    cout << "Connection made..." << endl;

    //Keep track of the bytes read and the bytes written
    int bytesRead, bytesWritten = 0;

    //Keep track of session time
    struct timeval start1, end1;
    gettimeofday(&start1, NULL);

    while(1) {
        //Communications with the server 
        cout << ">";

        string data;
        getline(cin, data);
        memset(&msg, 0, sizeof(msg)); //Clear the buffer
        strcpy(msg, data.c_str());    //Copy data to the buffer

        if(data == "exit") {
            //Client terminates the session 
            //Send info to server stating that client terminated session
            send(clientSd, (char*)&msg, strlen(msg), 0);
            break;
        }

        //Send message to server
        bytesWritten += send(clientSd, (char*)&msg, strlen(msg), 0);
        cout << "Awaiting server response..." << endl;

        //Clear buffer
        memset(&msg, 0, sizeof(msg));

        //Get message send from server 
        bytesRead += recv(clientSd, (char*)&msg, sizeof(msg), 0);
        if(!strcmp(msg, "exit")) {
            cout << "Server has terminated session..." << endl;
            break;
        }

        //otherwise print the server message 
        cout << "Server: " << msg << endl;
    } 

    //Close socket descriptors 
    gettimeofday(&end1, NULL);
    close(clientSd);
    cout << "********** Session **********" << endl;
    cout << "Bytes Written: " << bytesWritten << endl;
    cout << "Bytes Read: " << bytesRead << endl;
    cout << "Elapsed Time: " << (end1.tv_sec - start1.tv_sec) << " seconds" << endl;
    cout << "Connection closed..." << endl;

    return 0;
}