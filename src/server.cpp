/*
Main Source: https://beej.us/guide/bgnet/html/
*/

#include <arpa/inet.h>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

// TODO: make port str constant
#define DOMAIN "localhost"
#define PORT 1985
#define BACKLOG 5

/*
getaddrinfo()
socket()
bind()
listen()
accept()
*/

int main () {
    
    int status;
    struct addrinfo hints; // hints to give to getaddrinfo
    struct addrinfo * server_info, * p; // array of addrinfo from getaddrinfo
    struct sockaddr_in client_addr;
    socklen_t addr_size; 
    int sockfd; // socket file desciptor
    int clientfd; // client file descriptor
    
    // Set up hints to provide to getaddrinfo
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET; // ipv4
    hints.ai_socktype = SOCK_STREAM; // TCP 
    hints.ai_flags = AI_PASSIVE; // let getaddrinfo get my ip address
    

    // save an array of addrinfo to server_info given my hints
    status = getaddrinfo(DOMAIN, "1985", &hints, &server_info);
    if ( status != 0 ) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
    }
    
    // iterate through server_info till we get a bind
    for ( p = server_info ; p != NULL; p = p->ai_next ) {

        // create a socket using res of getaddrinfo
        sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if ( sockfd == -1 ) {
            perror("server: socket");
        }
        
        // bind socket file descriptor to a port
        status = bind(sockfd, p->ai_addr, p->ai_addrlen);
        if ( status == -1 ) {
            close(sockfd);
            perror("server:bind");
            continue;
        }

        break;
    }

    // free server_info
    freeaddrinfo(server_info);

    if ( p == NULL ) {
        fprintf(stderr, "server: failed to bind\n");
        exit(1);
    }
    
    
    // listen
    status = listen(sockfd, BACKLOG);
    if ( status == -1 ) {
        perror("listen");
        exit(1);
    }

    printf("Waiting for connections...\n");
    
    addr_size = sizeof(client_addr);
    clientfd = accept(sockfd, (struct sockaddr *)&client_addr,  &addr_size);
    
    return 0;   
}