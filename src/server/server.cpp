/*
Main Source: https://beej.us/guide/bgnet/html/
*/

#include "../sockethelper/sockethelper.h"

/*
getaddrinfo()
socket()
bind()
listen()
accept()
*/

/*
Usage:
./server localhost serverMessageFile
*/

int main (int argc, char *argv[]) {
    
    int status;
    struct addrinfo hints; // hints to give to getaddrinfo
    struct addrinfo * server_info, * p; // array of addrinfo from getaddrinfo
    struct sockaddr_in client_addr;
    socklen_t addr_size; 
    int sockfd; // socket file desciptor
    int clientfd; // client file descriptor
    int yes = 1;
    
    // handle command line arguments
    if ( argc != 3 ) {
        fprintf(stderr, "Usage: server hostname /path/to/message\n");
        exit(1);
    }
    
    // Set up hints to provide to getaddrinfo
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET; // ipv4
    hints.ai_socktype = SOCK_STREAM; // TCP 
    hints.ai_flags = AI_PASSIVE; // let getaddrinfo get my ip address
    

    // save an array of addrinfo to server_info given my hints
    status = getaddrinfo(argv[1], PORT_STR, &hints, &server_info);
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
        
        status = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
        if ( status == -1 ) {
            close(sockfd);
            exit(1);
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
    
    // accept loop
    while (1) {

        addr_size = sizeof(client_addr);
        clientfd = accept(sockfd, (struct sockaddr *)&client_addr,  &addr_size);
        
        if (!fork()) { //child
            close(sockfd);
            
            std::string str = std::string();

            std::fstream serverMessageFile(argv[2]);

            if ( serverMessageFile.is_open() ) {
                char c;
                while ( serverMessageFile.get(c) ) {
                    str.push_back(c);
                }
                serverMessageFile.close();
            }
            
            skt_write(clientfd, str);

            close(clientfd);
            exit(0);
        }
        
        close(clientfd);
    
    }
    
    return 0;   
}