#ifndef GUARD_LOGGER_H
#define GUARD_LOGGER_H

#include <string>

using std::string;

enum log_level {DEBUG, INFO, WARN, ERROR};

class Logger {
    public:
        void set_level(log_level);
        void debug(string);
        void info(string);
        void warn(string);
        void error(string);
        void fatal(string);
    private:
        log_level level = DEBUG;
};

#endif
