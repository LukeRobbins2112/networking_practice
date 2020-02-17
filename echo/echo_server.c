#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

// (1) create socket object
// (1.5) configure socket to make debugging easier
// (2) bind to port (INADDR_ANY)
// (3) start listening (allow queue of 1024)
int open_listenfd(int port){

    // (1)
    int listenFd = socket(AF_INET, SOCK_STREAM, 0);

    // (1.5)
    int optval=1;
    if (setsockopt(listenFd, SOL_SOCKET, SO_REUSEADDR,(const void *)&optval , sizeof(int)) < 0){
        exit(1);
    }

    // (2)
    struct sockaddr_in servSocket;
    bzero((char*)&servSocket, sizeof(servSocket));
    servSocket.sin_family = AF_INET;
    servSocket.sin_addr.s_addr = htonl(INADDR_ANY);
    servSocket.sin_port = htons(port);

    if ((bind(listenFd, (struct sockaddr *)&servSocket, sizeof(servSocket))) < 0){
        printf("Error during bind\n");
        exit(1);
    }

    // (3)
    if ((listen(listenFd, 1024)) < 0){
        printf("Error during listen\n");
        exit(1);
    }

    return listenFd;
}

int main(int argc, char** argv){

    int serverFd = open_listenfd(atoi(argv[1]));

    // accept
    int clientFd;           // If connection opens, use this for read/write
    struct sockaddr_in clientInfo;  // On accept(), grabs info about client
    unsigned int clientLen = sizeof(clientInfo);

    if ((clientFd = accept(serverFd, (struct sockaddr*)&clientInfo, &clientLen)) < 0){
        printf("Accept err\n");
        return 1;
    }

    printf("Connected to client\n");

    printf("Client: %s\n", inet_ntoa(clientInfo.sin_addr));

    char buf[10];
    if ((read(clientFd, buf, 10)) < 6){
        printf("Expected 6 bytes\n");
        exit(1);
    }

    printf("%s\n", buf);


    return 0;


}
