#include <stdio.h>
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netdb.h>

int main() {

    int status;
    struct addrinfo host_info;
    struct addrinfo *host_info_list;

    memset(&host_info, 0, sizeof host_info);

    host_info.ai_family = AF_UNSPEC;
    host_info.ai_socktype = SOCK_STREAM;

    status = getaddrinfo("127.0.0.1", "5555", &host_info, &host_info_list);
    if (status != 0) {
        std::cout << "getaddrinfo error" << gai_strerror(status);
        return status;
    }

    int sock = socket(host_info_list->ai_family, host_info_list->ai_socktype,
            host_info_list->ai_protocol);
    if (sock == -1) {
        printf("Socket error.\n");
    }

    status = connect(sock, host_info_list->ai_addr, host_info_list->ai_addrlen);
    if (status == -1) {
        printf("Connection error.\n");
    }

    char *msg = "GET / HTTP/1.1\nhost: www.google.com\n\n";
    int len;
    ssize_t bytes_sent;
    len = strlen(msg);
    bytes_sent = send(sock, msg, len, 0);

    ssize_t bytes_recieved;
    char incoming_data_buffer[1000];
    bytes_recieved = recv(sock, incoming_data_buffer,1000, 0);
    // If no data arrives, the program will just wait here until some data arrives.
    if (bytes_recieved == 0) std::cout << "host shut down." << std::endl ;
    if (bytes_recieved == -1)std::cout << "recieve error!" << std::endl ;
    std::cout << bytes_recieved << " bytes recieved :" << std::endl ;
    std::cout << incoming_data_buffer << std::endl;

    std::cout << "Receiving complete. Closing socket..." << std::endl;
    freeaddrinfo(host_info_list);
    close(sock);

}