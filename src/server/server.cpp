/*
Main Source: https://beej.us/guide/bgnet/html/
*/

#include "../sockethelper/sockethelper.h"
#include <chrono>
#include <cstddef>
#include <pthread.h>
#include <string>
#include <utility>
#include <vector>

/*
getaddrinfo()
socket()
bind()
listen()
accept()
*/

std::unordered_map<unsigned int, Connection> connections;
unsigned int num_connections = 0;
std::mutex connections_mutex;

// returns new uid
int create_new_connection (int fd) {
                        
    Connection new_connection;

    new_connection.fd = fd;
    new_connection.unix_epoch_last_received_heartbeat = time_since_unix_epoch(); 
    new_connection.heartbeat_n = 0;
    
    connections_mutex.lock();
    connections.insert(std::make_pair(num_connections, new_connection));
    connections_mutex.unlock();
    
    return num_connections++;
}

// Read and Respond
void * handle_connection(void * arg) {
    int clientfd = *(int*)arg;

    for (;;) {

        Message new_msg = skt_read_msg(clientfd);
        
        if ( msg_is_empty(new_msg) ) 
            continue;
        
        //print_message(new_msg);
        
        if ( new_msg.header == HEARTBEAT_HEADER ) {

            Heartbeat heartbeat = heart_msg_read(new_msg);
            
            // if heart_n is 0, then assign usid and add to connections
            if ( heartbeat.n == 0 ) {
                heartbeat.uid = create_new_connection(clientfd);
            }

            std::cout << "Received HEART " + std::to_string(heartbeat.n) + " from Client " + std::to_string(heartbeat.uid) + " at " + std::to_string(time_since_unix_epoch()) << std::endl;

            heart_msg_write(clientfd, heartbeat.n + 1, heartbeat.uid);
        
        // if received a print, send to all connections
        } else if ( new_msg.header == PRINT_HEADER ) {
            for ( auto connection : connections ) {
                int fd = connection.second.fd;
                skt_write_msg(fd, new_msg);
            }
        }

    }
    
    // done with this file descriptor
    close(clientfd);

    return nullptr;
}

/*
Usage:
./server localhost 
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
    if ( argc != 2 ) {
        fprintf(stderr, "Usage: server hostname\n");
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
    std::vector<pthread_t> threads;
    while (1) {

        addr_size = sizeof(client_addr);
        clientfd = accept(sockfd, (struct sockaddr *)&client_addr,  &addr_size);
        
        // accept a new connection and make a thread
        pthread_t new_connection;
        threads.push_back(new_connection);
        pthread_create(&new_connection, nullptr, handle_connection, &clientfd);
    }
    
    for ( pthread_t thread : threads ) {
        pthread_join(thread, nullptr);
    }
    
    return 0;   
}