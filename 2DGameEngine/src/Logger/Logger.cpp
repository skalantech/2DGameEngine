#define _CRT_SECURE_NO_DEPRECATE
#include "Logger.h"

#include <iostream>
#include <string>
#include <chrono>
#include <ctime>
#include <Windows.h>
#include <stdio.h>

std::vector<Log_entry> Logger::messages; // Anything declared static in the .h has to be defined in the .cpp

std::string Current_date_time_to_string() {
    std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::string output(30, '\0');
    std::strftime(&output[0], output.size(), "%d-%b-%Y %H:%M:%S", localtime(&now));
    return output;
}

void Logger::log(const std::string& message) {
    Log_entry log_entry;
    log_entry.type = LOG_INFO;
    log_entry.message = "LOG: [" + Current_date_time_to_string() + "]: " + message;
    std::cout << "\x1B[32m" << log_entry.message << "\033[0m" << std::endl;
    messages.push_back(log_entry);
}

void Logger::war(const std::string& message) {
    Log_entry log_entry;
    log_entry.type = LOG_INFO;
    log_entry.message = "WAR: [" + Current_date_time_to_string() + "]: " + message;
    std::cout << "\x1B[33m" << log_entry.message << "\033[0m" << std::endl;
    messages.push_back(log_entry);
}

void Logger::err(const std::string& message) {
    Log_entry log_entry;
    log_entry.type = LOG_ERROR;
    log_entry.message = "ERR: [" + Current_date_time_to_string() + "]: " + message;
    messages.push_back(log_entry);
    std::cerr << "\x1B[91m"<< log_entry.message << "\033[0m" << std::endl;
}

//\x1B[30m:Black
//\x1B[31m:Red
//\x1B[32m:Green
//\x1B[33m:Yellow
//\x1B[34m:Blue
//\x1B[35m:Magenta
//\x1B[36m:Cyan
//\x1B[37m:White
