#include <cstdio>
#include <sys/socket.h>
#include <netdb.h>
#include <cstring>
#include <unistd.h>
#include <stdlib.h>

#define SERVER "localhost"
#define PORT 16662
#define PORT_STR "16662"
#define BUFFERSIZE 1024

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
    
    char buf[BUFFERSIZE];
    int charsRead;
    while ( (charsRead = recv(sockfd, &buf, BUFFERSIZE, 0)) > 0 ) {
        printf("%s\n", buf);
        
        // clean the buffer
        memset(&buf, 0, BUFFERSIZE);
    }

    close(sockfd);
}