#include <string>
#include <cstdio>
#include <stack>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <cstring>
#include <string>

#define ALIGN_LEFT "\x1B\x61\x00"
#define ALIGN_CENTER "\x1B\x61\x01"
#define ALIGN_RIGHT "\x1B\x61\x02"
#define BOLD "\x1B\x20\x01"
#define UNBOLD "\x1B\x20\x00"
#define PRINT_DATA_IN_BUFFER "\x0c"

static std::unordered_map<std::string, std::string> legal_tags = {
    {"b", BOLD},
    {"ac", ALIGN_CENTER},
    {"al", ALIGN_LEFT},
    {"ar", ALIGN_RIGHT},
};

class Thermal_Printer {
    public:
    Thermal_Printer(std::string path);

    void print(std::string str);
    
    private:
    std::string mpath;
};

bool is_recognized_tag(std::string tag);
std::string tag_to_esc_code(std::string tag);
int get_index_of_next_close_carrot(unsigned int i, std::string str);
std::string body_to_thermal_printer(std::string html);
