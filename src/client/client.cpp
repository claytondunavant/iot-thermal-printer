#include "../sockethelper/sockethelper.h"
#include "thermalprinter.h"
#include <iostream>

/*
Usage:
client localhost /dev/ttyS0
*/

int main (int argc, char *argv[]) {
    
    int status;
    struct addrinfo hints;
    struct addrinfo *server_info, *p;
    int sockfd;
    
    // handle command line arguments
    if ( argc != 3 ) {
        fprintf(stderr, "Usage: client hostname /path/to/serialDev\n");
        exit(1);
    }

    // set up hints
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET; // ipv4
    hints.ai_socktype = SOCK_STREAM; // TCP 
                                    
    status = getaddrinfo(argv[1], PORT_STR, &hints, &server_info) ;
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
    
    // create Thermal Printer object
    Thermal_Printer tp = Thermal_Printer(argv[2]);
    
    Message msg = skt_read_msg(sockfd);
    
    std::string to_print = body_to_thermal_printer(msg.body);
    
    tp.print(to_print);
    
    close(sockfd);
}