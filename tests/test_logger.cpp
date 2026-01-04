#include "logger.h"
#include <thread>

void logTask(int id) {
    Logger::log("Log message from thread " + std::to_string(id));
}

int main() {
    std::thread t1(logTask, 1);
    std::thread t2(logTask, 2);
    std::thread t3(logTask, 3);
    std::thread t4(logTask, 4);

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    return 0;
}
