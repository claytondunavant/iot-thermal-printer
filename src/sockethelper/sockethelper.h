#include <cstdio>
#include <sys/socket.h>
#include <netdb.h>
#include <cstring>
#include <unistd.h>
#include <stdlib.h>
#include <string>

#define SERVER "localhost"
#define PORT 16662
#define PORT_STR "16662"
#define BACKLOG 5
#define BUFFERSIZE 1024

struct Message {
    std::string header;
    std::string body;
};

Message skt_read_msg(int fd);

void skt_write(int fd, std::string str);