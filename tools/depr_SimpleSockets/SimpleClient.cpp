#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define PORTNUM "6360"
#define BUFFERSIZE 100


// Returns the correct sockaddr for IPv4 or IPv6
void* get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &( ((struct sockaddr_in*)sa)->sin_addr );
    } else {
        return &( ((struct sockaddr_in6*)sa)->sin6_addr );
    }
}

int main() {
    int status;
    struct addrinfo hints;
    struct addrinfo* hosts;
    struct addrinfo* host;
    int sock;
    bool socket_bound = false;
    struct sockaddr_storage server_addr;
    socklen_t addr_len = sizeof server_addr;
    char buffer[BUFFERSIZE];

    memset(&hints, 0, sizeof hints);

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;

    status = getaddrinfo(NULL, PORTNUM, &hints, &hosts);

    for(host=hosts; host!=NULL; host=host->ai_next) {
        sock = socket(host->ai_family, host->ai_socktype, host->ai_protocol);
        if (sock == -1) continue;
        int bindstatus = bind(sock, host->ai_addr, host->ai_addrlen);
        if (bindstatus == -1) {
            close(sock);
            continue;
        }
        socket_bound = true;
        break;
    }

    if (!socket_bound) {
        printf("Error binding socket.\n");
        return 2;
    }

    printf("Waiting for recognition code: \n");
    int numrecd = recvfrom(sock, buffer, BUFFERSIZE-1, 0,
        (struct sockaddr*) &server_addr, &addr_len);
    if (numrecd == -1) {
        printf("Error receiving code.\n");
        exit(1);
    }

    printf("Received recognition code, %d bytes.\n", numrecd);
    buffer[numrecd] = '\0';
    printf("==> %s\n", buffer);

    freeaddrinfo(hosts);
    close(sock);
    return 0;
}