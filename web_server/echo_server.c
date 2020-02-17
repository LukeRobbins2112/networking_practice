#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "utils.h"

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

void serveStatic(int clientFd, char* filename, int filesize){
    int docFd;
    char* documentPtr;
    char buf[MAX_LINE];

    sprintf(buf, "HTTP/1.1 200 OK\r\n");
    sprintf(buf, "%sServer: TINY web server\r\n", buf);
    sprintf(buf, "%sContent-length: %d\r\n", buf, filesize);
    sprintf(buf, "%sContent-type: html\r\n", buf);
    sprintf(buf, "%s\r\n", buf);

    int responseLen = strlen(buf);
    if (write(clientFd, buf, responseLen) < responseLen){
        printf("Error on writing response header");
    }

    // Response body
    if ((docFd = open(filename, O_RDONLY, 0)) < 0){
        printf("Error opening file");
        return;
    }

    documentPtr = mmap(0, filesize, PROT_READ, MAP_PRIVATE, docFd, 0);
    close(docFd);
    if (write(clientFd, documentPtr, filesize) < filesize){
        printf("Error writing file contents");
        return;
    }
    munmap(documentPtr, filesize);

}

void serviceClient(int clientFd){

    char buf[MAX_LINE], method[MAX_LINE], uri[MAX_LINE], version[MAX_LINE], filename[MAX_LINE];

    // Get the request line
    if ((read(clientFd, buf, MAX_LINE)) < 0){
        printf("Read error");
    }

    sscanf(buf, "%s %s %s", method, uri, version);

    // printf("method: %s\n", method);
    // printf("uri: %s\n", uri);
    // printf("version: %s\n", version);

    if (strncasecmp(method, "GET", 3)){
        printf("Error, we can only use the GET method right now");
        return;
    }

    // File name and info
    struct stat filestats;
    strcpy(filename, ".");
    strcat(filename, uri);
    //printf("relative file path: %s\n", filename);

    if (stat(filename, &filestats) < 0){
        printf("Error getting file stats");
        return;
    }

    //printf("File size: %lld\n", filestats.st_size);

    serveStatic(clientFd, filename, filestats.st_size);

}

int main(int argc, char** argv){

    int serverFd = open_listenfd(atoi(argv[1]));

    // accept
    int clientFd;           // If connection opens, use this for read/write
    struct sockaddr_in clientInfo;  // On accept(), grabs info about client
    unsigned int clientLen = sizeof(clientInfo);

    while(1){
        if ((clientFd = accept(serverFd, (struct sockaddr*)&clientInfo, &clientLen)) < 0){
            printf("Accept err\n");
            return 1;
        }
        printf("Connected to client\n");
        printf("Client: %s\n", inet_ntoa(clientInfo.sin_addr));

        serviceClient(clientFd);
        close(clientFd);
    }

    return 0;
}
