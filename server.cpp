#include <arpa/inet.h>
#include <cstdio>
#include <cstring>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>

// TODO: make port str constant
#define DOMAIN "localhost"
#define PORT 1985

int main () {
    
    int status;
    struct addrinfo hints; // hints to give to getaddrinfo
    struct addrinfo * serverInfo; // array of addrinfo from getaddrinfo
    
    // Set up hints to provide to getaddrinfo
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET; // ipv4
    hints.ai_socktype = SOCK_STREAM; // TCP 
    hints.ai_flags = AI_PASSIVE; // let getaddrinfo get my ip address
    

    // save an array of addrinfo to serverInfo given my hints
    status = getaddrinfo(DOMAIN, NULL, &hints, &serverInfo);
    if ( status != 0 ) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
    }
    
    struct addrinfo *p;
    for ( p = serverInfo; p != NULL; p = p->ai_next) {
        char ip[INET_ADDRSTRLEN]; // space to hold ipv4 string
                                
        struct sockaddr_in * ipv4 = (struct sockaddr_in *) p->ai_addr;
        inet_ntop(AF_INET, &(ipv4->sin_addr), ip, INET_ADDRSTRLEN);

        printf("%s\n", ip);

    }
    
    freeaddrinfo(serverInfo);

    return 0;   
}