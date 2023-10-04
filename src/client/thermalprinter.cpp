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
    
    fprintf(file, str.c_str());
}

// convert html to thermal printer escape sequences
std::string htmlToThermalPrinter(std::string html) {
    return std::string();
}