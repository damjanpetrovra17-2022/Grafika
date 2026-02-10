#include "../Header/Log.h"

void Log::init()
{
    
    
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
