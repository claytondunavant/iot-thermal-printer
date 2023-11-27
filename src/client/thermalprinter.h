#include <cstddef>
#include <string>
#include <cstdio>
#include <stack>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <cstring>
#include <string>
#include <fstream>

#define BOLD_TAG "b"
#define ALIGN_CENTER_TAG "ac"
#define ALIGN_LEFT_TAG "al"
#define ALIGN_RIGHT_TAG "ar"
#define LINE_FEED_TAG "lf"

#define ALIGN_LEFT_CODE "\x1B\x61\x00"
#define ALIGN_CENTER_CODE "\x1B\x61\x01"
#define ALIGN_RIGHT_CODE "\x1B\x61\x02"
#define BOLD_CODE "\x1B\x20\x01"
#define UNBOLD_CODE "\x1B\x20\x00"
#define PRINT_DATA_IN_BUFFER_CODE "\x0c"
#define LINE_FEED_CODE "\x1B\x64"

#define LINE_FEED_DEFAULT_VAL "\x01"

static std::unordered_map<std::string, std::string> legal_tags = {
    {BOLD_TAG, BOLD_CODE},
    {ALIGN_CENTER_TAG, ALIGN_CENTER_CODE},
    {ALIGN_LEFT_TAG, ALIGN_LEFT_CODE},
    {ALIGN_RIGHT_CODE, ALIGN_RIGHT_CODE},
    {LINE_FEED_TAG, LINE_FEED_TAG},
};

static std::unordered_map<std::string, std::string> default_tag_values = {
    {LINE_FEED_TAG, LINE_FEED_DEFAULT_VAL},
};

class Thermal_Printer {
    public:
    Thermal_Printer(std::string path);
    Thermal_Printer();

    void print(std::string str);
    
    private:
    std::string mpath;
};

bool is_recognized_tag(std::string tag);
std::string tag_to_esc_code(std::string tag);
int get_index_of_next_close_carrot(unsigned int i, std::string str);
std::string body_to_thermal_printer(std::string html);
