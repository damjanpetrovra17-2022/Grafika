#include "../Header/Log.h"

void Log::init()
{
    // Placeholder for future logging initialization
    // Could set up log files, configure output streams, etc.
}

void Log::info(const std::string& message)
{
    std::cout << "[INFO] " << message << std::endl;
}

void Log::warning(const std::string& message)
{
    std::cout << "[WARNING] " << message << std::endl;
}

void Log::error(const std::string& message)
{
    std::cerr << "[ERROR] " << message << std::endl;
}
