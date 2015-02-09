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
    host_info.ai_flags = AI_PASSIVE; 
    status = getaddrinfo(NULL, "5555", &host_info, &host_info_list);

    int sock = socket(host_info_list->ai_family, host_info_list->ai_socktype,
            host_info_list->ai_protocol);
    if (sock == -1) {
        printf("Socket error.\n");
    }

    int yes = 1;
    status = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
    status = bind(sock, host_info_list->ai_addr, host_info_list->ai_addrlen);
    if (status == -1)  printf("Binding error.\n");

    status =  listen(sock, 5);
    if (status == -1) {
        printf("Listen error.\n");
    }

    int new_sd;
    struct sockaddr_storage their_addr;
    socklen_t addr_size = sizeof(their_addr);
    new_sd = accept(sock, (struct sockaddr *)&their_addr, &addr_size);
    if (new_sd == -1)
    {
        std::cout << "listen error" << std::endl ;
    }
    else
    {
        std::cout << "Connection accepted. Using new sock : "  <<  new_sd << std::endl;
    }


    std::cout << "Waiting to recieve data..."  << std::endl;
    ssize_t bytes_recieved;
    char incomming_data_buffer[1000];
    bytes_recieved = recv(new_sd, incomming_data_buffer,1000, 0);
    // If no data arrives, the program will just wait here until some data arrives.
    if (bytes_recieved == 0) std::cout << "host shut down." << std::endl ;
    if (bytes_recieved == -1)std::cout << "recieve error!" << std::endl ;
    std::cout << bytes_recieved << " bytes recieved :" << std::endl ;
    incomming_data_buffer[bytes_recieved] = '\0';
    std::cout << incomming_data_buffer << std::endl;


    std::cout << "send()ing back a message..."  << std::endl;
    char *msg = "-0.947 -1.000 -0.545";
    int len;
    ssize_t bytes_sent;
    len = strlen(msg);
    bytes_sent = send(new_sd, msg, len, 0);

    std::cout << "Stopping server..." << std::endl;
    freeaddrinfo(host_info_list);
    close(new_sd);
    close(sock);
}