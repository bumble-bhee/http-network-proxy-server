#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <mutex>

/*
 * Thread-safe Logger class
 */
class Logger {
public:
    // Log a message to file with timestamp
    static void log(const std::string& message);

private:
    static std::mutex logMutex;
};

#endif
