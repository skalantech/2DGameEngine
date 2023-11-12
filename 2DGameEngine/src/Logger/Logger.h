#ifndef LOGGER_H
#define LOGGER_H

#include <vector>
#include <string>

enum Log_type {
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR
};

struct Log_entry {
    Log_type type;
    std::string message;
};

class Logger {
    public:
        static std::vector<Log_entry> messages; // One vector for the entire class
        // static so that it is not necessary to create instances of this class
        static void log(const std::string& message); // static methods
        static void war(const std::string& message);
        static void err(const std::string& message);
};

#endif
