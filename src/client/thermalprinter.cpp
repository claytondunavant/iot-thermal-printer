#include "thermalprinter.h"
#include <cstdio>
#include <cstring>

Thermal_Printer::Thermal_Printer(std::string path) {

    mpath = path; 
    
    //TODO send initialization codes to printer
}

void Thermal_Printer::print(std::string str) {

    FILE * file = fopen(mpath.c_str(), "w");
    if ( file == NULL ) {
        perror("Failed to open serial file");
        return;
    }
    
    fprintf(file, str.c_str());
}

bool is_recognized_tag(std::string tag) {
    for ( auto legal_tag : LEGAL_TAGS ) {
        if ( strcmp(legal_tag, tag.c_str()) == 0 ) {
            return true;
        }
    }
    return false;
}

int get_index_of_next_close_carrot(unsigned int i, std::string str) {
    for ( ; i < str.size(); i++ ) {

        if (str[i] == '>') {
            return i;
        }
    }
    
    return -1;
}

// convert html to thermal printer escape sequences
std::string html_to_thermal_printer(std::string html) {
    
    
    std::string out; //output
    std::stack<std::string> s; //stack 
    unsigned int i; // index
    char c; // character buffer
    std::string buf; // string buffer
    int i_next_close_carrot; //index of next close carrot
    
    //loop through
    //when you see a <
    //if you recognize the tag, then add escape sequence to output and add text 
    //if you dont recognize the tag, consume until end of this tag at this level

    for ( i = 0; i < html.size(); i++ ) {
        c = html[i];
        
        if ( c == '<' && i < (html.size() - 1) ) {
            
            c = html[i+1];
            i_next_close_carrot = get_index_of_next_close_carrot(i, html);
            
            // if there isnt a close carrot
            if ( i_next_close_carrot == -1 )
                break;
            
            // if its a close tag
            if ( c == '/' ) {

                // get the substring, its the tag name
                buf = html.substr(i+2, i_next_close_carrot - (i+2));
                
                std::cout << "Close Tag BUF:" << std::endl;
                std::cout << buf << std::endl;
                
                // jump to after tag
                i = i_next_close_carrot;
                continue;

            // if its an open bracket
            } else {
                
                // get the substring, its the tag name
                buf = html.substr(i+1, i_next_close_carrot - (i+1));
                std::cout << "Open Tag BUF:" << std::endl;
                std::cout << buf << std::endl;
                
                // TODO handle unrecongized nested cases
                if ( is_recognized_tag(buf) ) {
                    
                    if ( buf == "h1" ) {
                        out.append(ALIGN_CENTER);
                        out.append(BOLD);
                    } else if ( buf == "p") {
                        out.append(ALIGN_LEFT);
                        out.append(UNBOLD);
                    }
                }
                
                // jump to after tag
                // TODO: why is this -1 
                i = i_next_close_carrot - 1;
                continue;
            }
        }
        
        out.push_back(c);

    }
    
    return out;
}