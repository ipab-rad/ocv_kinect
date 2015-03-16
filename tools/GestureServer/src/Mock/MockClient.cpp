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


// Returns the correct sockaddr for IPv4 or IPv6
void* get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &( ((struct sockaddr_in*)sa)->sin_addr );
    } else {
        return &( ((struct sockaddr_in6*)sa)->sin6_addr );
    }
}

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

    for(host=hosts; host!=NULL; host=host->ai_next) {
        sock = socket(host->ai_family, host->ai_socktype, host->ai_protocol);
        if (sock == -1) {
            printf("Socket error.\n");
            continue;
        };
        int bindstatus = bind(sock, host->ai_addr, host->ai_addrlen);
        if (bindstatus == -1) {
            printf("Bind error: %d, %s\n", errno, strerror(errno));
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

    while (true) {
        int numrecd = recvfrom(sock, buffer, buffer_size, 0,
            (struct sockaddr*) &server_addr, &addr_len);
        if (numrecd == -1) {
            printf("Error receiving.\n");
            exit(1);
        }

        printf("Received %d bytes: ", numrecd);
        printf("%s", buffer);
        gesture g = deserialize_gesture(buffer, buffer_size);
        printf("  Movement: (%.3f %.3f %.3f)\n", g.movement.x, g.movement.y, g.movement.z);
    }

    freeaddrinfo(hosts);
    close(sock);
    return 0;
}