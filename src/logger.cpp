#include "logger.h"

#include <iostream>
#include <string>
#include <cstdlib>

using std::cerr;
using std::endl;

void Logger::set_level(log_level new_level) {
    this->level = new_level;
}

void Logger::debug(string msg) {
    if (this->level <= DEBUG)
        cerr << "DEBUG: " << msg << endl;
}

void Logger::info(string msg) {
    if (this->level <= INFO)
        cerr << "INFO: " << msg << endl;
}

void Logger::warn(string msg) {
    if (this->level <= WARN)
        cerr << "WARN: " << msg << endl;
}

void Logger::error(string msg) {
    if (this->level <= ERROR)
        cerr << "ERROR: " << msg << endl;
}

void Logger::fatal(string msg) {
    cerr << "FATAL: " << msg << endl;
    exit(1);
}

