#include <string>
#include <cstdio>
#include <stack>
#include <vector>
#include <iostream>

#define LEGAL_TAGS {"h1", "p"}

#define ALIGN_LEFT "\x1B\x61\x00"
#define ALIGN_CENTER "\x1B\x61\x01"
#define ALIGN_RIGHT "\x1B\x61\x02"
#define BOLD "\x1B\x20\x01"
#define UNBOLD "\x1B\x20\x00"

class Thermal_Printer {
    public:
    Thermal_Printer(std::string path);

    void print(std::string str);
    
    private:
    std::string mpath;
};

bool is_recognized_tag(std::string tag);
int get_index_of_next_close_carrot(unsigned int i, std::string str);
std::string html_to_thermal_printer(std::string html);
