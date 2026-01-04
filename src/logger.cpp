#include "logger.h"
#include <fstream>
#include <ctime>
#include <iomanip>

std::mutex Logger::logMutex;

void Logger::log(const std::string& message) {
    std::lock_guard<std::mutex> lock(logMutex);

    std::ofstream logFile("logs/proxy.log", std::ios::app);
    if (!logFile.is_open()) {
        return;
    }

    // Get current time
    std::time_t now = std::time(nullptr);
    std::tm* localTime = std::localtime(&now);

    logFile << "[" 
            << std::put_time(localTime, "%Y-%m-%d %H:%M:%S")
            << "] "
            << message 
            << std::endl;

    logFile.close();
}
