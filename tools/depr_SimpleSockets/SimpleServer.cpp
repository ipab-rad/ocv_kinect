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


int main() {
    int status;
    struct addrinfo hints;
    struct addrinfo* hosts;
    struct addrinfo* host;
    int sock;
    bool socket_bound = false;

    memset(&hints, 0, sizeof hints);

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;

    status = getaddrinfo(NULL, PORTNUM, &hints, &hosts);

    for(host=hosts; host!=NULL; host=host->ai_next) {
        sock = socket(host->ai_family, host->ai_socktype, host->ai_protocol);
        if (sock == -1) continue;
        socket_bound = true;
        break;
    }

    if (!socket_bound) {
        printf("Error getting socket.\n");
        return 2;
    }

    const char* message = "Recognition code 927, I am a potato.";
    while (true) {
        int numsent = sendto(sock, message, strlen(message), 0,
                 host->ai_addr, host->ai_addrlen);

        if (numsent == -1) {
            printf("Send error.\n");
        } else {
            printf("Server sent %d bytes: %s\n", numsent, message);
        }
    }


    freeaddrinfo(hosts);
    close(sock);
    return 0;
}