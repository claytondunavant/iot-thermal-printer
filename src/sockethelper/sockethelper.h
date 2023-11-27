#include <cstdio>
#include <sys/socket.h>
#include <netdb.h>
#include <cstring>
#include <unistd.h>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <mutex>

#define PORT 16662
#define PORT_STR "16662"
#define BACKLOG 5
#define BUFFERSIZE 1024

#define HEARTBEAT_SEC 1
#define FIRST_HEARTBEAT_N_FROM_SERVER 1

#define HEARTBEAT_HEADER "HEART\n\n"
#define PRINT_HEADER "PRINT\n\n"

struct Message {
    std::string header;
    std::string body;
};

struct Connection {
    int fd;
    unsigned int unix_epoch_last_received_heartbeat;
    unsigned int heartbeat_n;
};

// struct of a parsed hearbeat message body
struct Heartbeat {
    unsigned int n;
    unsigned int uid;
};

Message skt_read_msg(int fd);

void skt_write(int fd, std::string str);

Heartbeat heart_msg_read(Message msg);

void heart_msg_write(int fd, int n, unsigned int uid = 0);

bool msg_is_empty(Message msg);

unsigned int time_since_unix_epoch();

void print_message(Message msg);