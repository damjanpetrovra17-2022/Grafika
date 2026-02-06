#pragma once

#include <iostream>
#include <string>

// Simple logging macros
#define LOG_INFO(msg) std::cout << "[INFO] " << msg << std::endl
#define LOG_WARNING(msg) std::cout << "[WARNING] " << msg << std::endl
#define LOG_ERROR(msg) std::cerr << "[ERROR] " << msg << std::endl

class Log
{
public:
    static void init();
    static void info(const std::string& message);
    static void warning(const std::string& message);
    static void error(const std::string& message);
};
