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


// (1) create socket
// (2) get hostentry info
// (3) use hostent to populate sockaddr_in struct
// (4) finish opening socket with sockaddr_in of server
int open_clientfd(char* hostname, int port){

    // (1)
    int clientFd = socket(AF_INET, SOCK_STREAM, 0);

    // (2)
    struct hostent* hostp = gethostbyname(hostname);
    if (!hostp){
        printf("error getting host entry");
        exit(1);
    }

    // (3)
    struct sockaddr_in serverAddr;
    bzero((char *)&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    bcopy((char *)hostp->h_addr_list[0],
          (char *)&serverAddr.sin_addr,
          hostp->h_length);
    serverAddr.sin_port = htons(port);

    // (4)
    if ((connect(clientFd, (struct sockaddr*)&serverAddr, sizeof(serverAddr))) < 0){
        printf("Error opening connection\n");
        exit(1);
    }

    return clientFd;
}


int main(int argc, char** argv){

    if (argc < 2){
        printf("Err, need args\n");
        return 1;
    }

    int clientFd = open_clientfd(argv[1], atoi(argv[2]));

    printf("Connected to server\n");

    char* buf = "hello";
    if (write(clientFd, buf, 6) < 6){
        printf("Error writing to server");
    }

    return 0;

}
