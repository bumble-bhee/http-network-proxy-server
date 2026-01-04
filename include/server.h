#ifndef SERVER_H
#define SERVER_H

#include <string>
#include <unordered_set>

/*
 * ProxyServer: core proxy logic
 */
class ProxyServer {
public:
    ProxyServer();

    void loadBlockedDomains(const std::string& filename);

    // Start proxy server
    void start(int port);

private:
    std::unordered_set<std::string> blockedDomains;

    // Handle a single client
    void handleClient(int clientFd);

    // Helpers
    bool isBlocked(const std::string& host) const;
    int connectToServer(const std::string& host, int port);

    // Error responses
    void send403(int clientFd);
    void send503(int clientFd);
};

#endif
