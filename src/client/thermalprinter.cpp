#include "thermalprinter.h"

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
    
    fprintf(file, (const char *) str.c_str());
}

bool is_recognized_tag(std::string tag) {
    return legal_tags.find(tag) != legal_tags.end();
}

std::string tag_to_esc_code(std::string tag) {
    return legal_tags[tag];
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
std::string body_to_thermal_printer(std::string html) {
    
    
    std::string out; //output
    std::stack<std::string> s; //stack 
    unsigned int i; // index
    char c; // character buffer
    std::string tag; // tag buffer
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
            
            // get the substring, its the tag name
            tag = html.substr(i+1, i_next_close_carrot - (i+1));
            
            if ( is_recognized_tag(tag) ) {
                out.append(tag_to_esc_code(tag));
                out.append(PRINT_DATA_IN_BUFFER); // flush the tag
            } else {
                out.append("Unrecongized tag: " + tag + "\n");
            }
            
            // jump to after tag
            i = i_next_close_carrot;
            continue;
        }
        
        out.push_back(c);

    }
    
    return out;
}