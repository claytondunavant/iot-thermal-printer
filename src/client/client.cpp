#include "../sockethelper/sockethelper.h"
#include <iostream>

#define SERIAL "/dev/ttyS0"


int main () {
    
    int status;
    struct addrinfo hints;
    struct addrinfo *server_info, *p;
    int sockfd;
    
    // set up hints
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET; // ipv4
    hints.ai_socktype = SOCK_STREAM; // TCP 
                                    
    status = getaddrinfo(SERVER, PORT_STR, &hints, &server_info) ;
    if ( status != 0 ) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
    }
    
    // iterate through server_info till we get a connection
    for ( p = server_info ; p != NULL; p = p->ai_next ) {

        // create a socket using res of getaddrinfo
        sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if ( sockfd == -1 ) {
            perror("server: socket");
        }
        
        // attempt to connect to socket
        status = connect(sockfd, p->ai_addr, p->ai_addrlen);
        if ( status == -1 ) {
            close(sockfd);
            perror("client: connect");
            continue;
        }

        break;
    }
    
    // free server_info
    freeaddrinfo(server_info);

    if ( p == NULL ) {
        fprintf(stderr, "client: failed to connect\n");
        exit(1);
    }
    
    skt_write(sockfd, "\r\n\r\n");
    std::string msg = skt_read_http_msg_header(sockfd);
    
    close(sockfd);
}