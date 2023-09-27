#include "sockethelper.h"

bool str_end_is_double_crlf(std::string str) {
    return str.length() >= 4 && str.substr(str.length() - 4).compare("\r\n\r\n") == 0;
}

// need to make this smarter
// does not grab content
std::string skt_read_http_msg_header(int fd) {

    std::string header;
    unsigned char c;

    while( read(fd, &c, sizeof(c)) > 0 ) {
        
        header.push_back(c);

        // if its the end of a line
        if (str_end_is_double_crlf(header))
            break;
        
    }

    return header;
}

void skt_write(int fd, std::string str) {
    write(fd, str.c_str(), str.length());
}