#include "sockethelper.h"

bool str_end_is_double_lf(std::string str) {
    return str.length() >= 2 && str.substr(str.length() - 2).compare("\n\n") == 0;
}

/*
Whether http requests have a message or not requires knowledge on the content of the message.
I don't want to deal with that.
So behold, my content-independent protocol for now:

<header info>\n
\n
<body info>\r\n
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