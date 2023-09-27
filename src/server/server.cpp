/*
Main Source: https://beej.us/guide/bgnet/html/
*/

#include "../sockethelper/sockethelper.h"
#include <asm-generic/socket.h>
#include <string>
#include <sys/socket.h>

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
    int yes = 1;
    
    // Set up hints to provide to getaddrinfo
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET; // ipv4
    hints.ai_socktype = SOCK_STREAM; // TCP 
    hints.ai_flags = AI_PASSIVE; // let getaddrinfo get my ip address
    

    // save an array of addrinfo to server_info given my hints
    status = getaddrinfo(SERVER, PORT_STR, &hints, &server_info);
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
            str.append("HTTP/1.1 200 Document follows\r\n");
            str.append("Server: IoT Thermal Printer\r\n");
            str.append("Content-type: text/html\r\n");
            str.append("\r\n");
            str.append("<h1>Hello World</h1>\r\n");

            skt_read_http_msg_header(clientfd);
            skt_write(clientfd, str);

            close(clientfd);
            exit(0);
        }
        
        close(clientfd);
    
    }
    
    return 0;   
}