#include "server.h"
#include <csignal>
#include <iostream>

ProxyServer* proxy = nullptr;

void handleSignal(int) {
    std::cout << "\nShutting down proxy...\n";
    exit(0);
}

int main() {
    signal(SIGINT, handleSignal);

    ProxyServer server;
    proxy = &server;

    server.loadBlockedDomains("config/blocked_domains.txt");
    server.start(8888);

    return 0;
}
