#include <string>
#include <cstdio>

class Thermal_Printer {
    public:
    Thermal_Printer(std::string path);

    void print(std::string str);
    
    private:
    std::string mpath;
};

std::string htmlToThermalPrinter(std::string html);
