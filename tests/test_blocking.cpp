#include <iostream>
#include "server.h"

int main() {
    ProxyServer proxy;
    proxy.loadBlockedDomains("config/blocked_domains.txt");

    std::string test1 = "facebook.com";
    std::string test2 = "google.com";

    std::cout << test1 << " blocked? "
              << (proxy.isBlocked(test1) ? "YES" : "NO") << std::endl;

    std::cout << test2 << " blocked? "
              << (proxy.isBlocked(test2) ? "YES" : "NO") << std::endl;

    return 0;
}
