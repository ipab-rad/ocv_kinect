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

#include <gesture.hpp>


int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: MockClient server_ip port\n");
        exit(1);
    }

    int status;
    struct addrinfo hints;
    struct addrinfo* hosts;
    struct addrinfo* host;
    int sock;
    bool socket_bound = false;
    struct sockaddr_storage server_addr;
    socklen_t addr_len = sizeof server_addr;

    int buffer_size = gesture_size();
    char buffer[buffer_size];

    memset(&hints, 0, sizeof hints);

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;

    status = getaddrinfo(argv[1], argv[2], &hints, &hosts);

    for (host=hosts; host!=NULL; host=host->ai_next) {
        sock = socket(host->ai_family, host->ai_socktype, host->ai_protocol);
        if (sock == -1) continue;

        socket_bound = true;
        break;
    }

    if (!socket_bound) {
        printf("Error getting socket.\n");
        return 2;
    }

    while (true) {
        gesture g;
        g.movement = {0.25, 0.50, 0.75};
        char buffer[buffer_size];
        serialize_gesture(g, buffer, buffer_size);
        int sentbytes = sendto(sock, buffer, buffer_size, 0,
            host->ai_addr, host->ai_addrlen);
        if (sentbytes == -1) {
            printf("Error sending data.\n");
        } else {
            printf("Sent %d bytes: \n", sentbytes);
        }
    }

    freeaddrinfo(hosts);
    close(sock);
    return 0;
}