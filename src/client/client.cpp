#include "../sockethelper/sockethelper.h"
#include "thermalprinter.h"
#include <cstdio>
#include <ctime>
#include <iostream>
#include <mutex>
#include <string>
#include <unistd.h>
#include <pthread.h>

/*
Usage:
client localhost /dev/ttyS0
*/

unsigned int uid;
unsigned int time_at_last_heart_send;
unsigned int last_recv_heartbeat_n;
Thermal_Printer tp = Thermal_Printer("REPLACE"); // TODO: fix
std::mutex mutex;

int connect_to_server (char * hostname) {
    int status;
    struct addrinfo hints;
    struct addrinfo *server_info, *p;
    int sockfd;
    

    // set up hints
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET; // ipv4
    hints.ai_socktype = SOCK_STREAM; // TCP 
                                    
    status = getaddrinfo(hostname, PORT_STR, &hints, &server_info) ;
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

    return sockfd;
}

void * read_from_server(void *arg) {
    int sockfd = *(int*)arg;
    
    for (;;) {
        Message new_msg = skt_read_msg(sockfd);
        
        if ( msg_is_empty(new_msg) ) {
            //socket_mutex.unlock();
            continue;
        }
        
        print_message(new_msg);
        
        if ( new_msg.header == HEARTBEAT_HEADER ) {
            Heartbeat heartbeat = heart_msg_read(new_msg);
            
            mutex.lock();
            last_recv_heartbeat_n = heartbeat.n;
            mutex.unlock();
            
            // if its the first heartbeat, register the uid
            if ( heartbeat.n == FIRST_HEARTBEAT_N_FROM_SERVER ) {
                mutex.lock();
                uid = heartbeat.uid;
                mutex.unlock();
            }

            std::cout << "Received HEART " + std::to_string(heartbeat.n) + " from Server at " + std::to_string(time_since_unix_epoch()) << std::endl;

        } else if ( new_msg.header == PRINT_HEADER ) {
            tp.print(body_to_thermal_printer(new_msg.body));
        }
    }
    
    return nullptr;
}

// Send heartbeats
void * write_to_server(void * arg) {

    unsigned int curr_time; 

    int sockfd = *(int*)arg;
    
    // send heart 0
    heart_msg_write(sockfd, 0);
    mutex.lock();
    time_at_last_heart_send = time_since_unix_epoch();
    mutex.unlock();
    
    for (;;) {

        curr_time = time_since_unix_epoch(); 

        // send heartbeat 
        mutex.lock();
        if ( curr_time - time_at_last_heart_send > HEARTBEAT_SEC ) {
            time_at_last_heart_send = curr_time;
            heart_msg_write(sockfd, last_recv_heartbeat_n + 1);
        }
        mutex.unlock();
    }

    return nullptr;
}

int main (int argc, char *argv[]) {
    
    // handle command line arguments
    if ( argc != 3 ) {
        fprintf(stderr, "Usage: client hostname /path/to/serialDev\n");
        exit(1);
    }
    
    int sockfd = connect_to_server(argv[1]);
    tp = Thermal_Printer(argv[2]);
    
    pthread_t reader, writer;
    pthread_create(&reader, nullptr, read_from_server, &sockfd);
    pthread_create(&writer, nullptr, write_to_server, &sockfd);
    
    pthread_join(reader, nullptr);
    pthread_join(writer, nullptr);
    
}