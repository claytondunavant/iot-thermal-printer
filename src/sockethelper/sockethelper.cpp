#include "sockethelper.h"
#include <bits/chrono.h>
#include <iostream>
#include <iterator>
#include <ostream>
#include <sstream>
#include <string>

bool  str_end_is_double_lf(std::string str) {
    return str.length() >= 2 && str.substr(str.length() - 2).compare("\n\n") == 0;
}

/*
Whether http requests have a message or not requires knowledge on the content of the message.
I don't want to deal with that.
So behold, my content-independent protocol for now:

<header info>\n
\n
<body info>\n
\n
*/

Message skt_read_msg(int fd) {

    Message msg; // output message
    unsigned char c; // character buffer
    bool is_header; 
    
    is_header = true;

    while( read(fd, &c, sizeof(c)) > 0 ) {
        
        if ( is_header ) {

            msg.header.push_back(c);
            
            if (str_end_is_double_lf(msg.header))
                is_header = false;

        } else {

            msg.body.push_back(c);

            if (str_end_is_double_lf(msg.body))
                break;
            
        }

    }

    return msg;
}

void skt_write(int fd, std::string str) {
    write(fd, str.c_str(), str.length());
}

void heart_msg_write(int fd, int n, unsigned int uid) {
    std::string heart_str = 
        "HEART\n\n" +
        std::to_string(n) +
        "\n"
        ;
    
    // if not the 0th heartbeat, add the uid
    if ( n > 0 ) {
        heart_str +=
            std::to_string(uid) +
            "\n";
    }
    
    heart_str +=
        "\n";
    
    skt_write(fd, heart_str);
}

// return the n in the heart msg
Heartbeat heart_msg_read(Message msg) {

    Heartbeat heart;
    std::istringstream ss (msg.body);
    std::string buf;
    int lines_read = 0;
    unsigned int curr_val;
    
    while ( std::getline( ss, buf, '\n') ) {

        if ( buf.length() == 0 ) 
            continue;

        curr_val = std::stoi(buf);

        switch (lines_read) {
            case 0:
                heart.n = curr_val;
                break;
            case 1:
                heart.uid = curr_val;
        }
        
        lines_read++;
    }

    return heart;    
}

bool msg_is_empty(Message msg) {
    return msg.header.length() == 0 && msg.body.length() == 0;
}

unsigned int time_since_unix_epoch() {
    return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

void print_message(Message m){
    std::cout << "HEADER: " << m.header << std::endl;
    std::cout << "BODY: " << m.body << std::endl;
}