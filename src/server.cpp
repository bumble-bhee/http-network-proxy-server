#include "server.h"
#include "http_parser.h"
#include "logger.h"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <thread>
#include <cstring>

#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

ProxyServer::ProxyServer() {}

void ProxyServer::loadBlockedDomains(const std::string& filename) {
    std::ifstream file(filename);
    std::string domain;

    while (std::getline(file, domain)) {
        domain.erase(
            std::remove_if(domain.begin(), domain.end(), ::isspace),
            domain.end()
        );
        if (!domain.empty())
            blockedDomains.insert(domain);
    }

}

bool ProxyServer::isBlocked(const std::string& host) const {
    std::string h = host;

    // convert to lowercase
    std::transform(h.begin(), h.end(), h.begin(), ::tolower);

    // remove port if present (example.com:80)
    size_t colon = h.find(':');
    if (colon != std::string::npos) {
        h = h.substr(0, colon);
    }

    // trim whitespace
    h.erase(std::remove_if(h.begin(), h.end(), ::isspace), h.end());

    return blockedDomains.find(h) != blockedDomains.end();
}

void ProxyServer::send403(int clientFd) {
    const char* msg =
        "HTTP/1.1 403 Forbidden\r\n"
        "Content-Length: 0\r\n\r\n";
    send(clientFd, msg, strlen(msg), 0);
}

void ProxyServer::send503(int clientFd) {
    const char* msg =
        "HTTP/1.1 503 Service Unavailable\r\n"
        "Content-Length: 0\r\n\r\n";
    send(clientFd, msg, strlen(msg), 0);
}

int ProxyServer::connectToServer(const std::string& host, int port) {
    hostent* server = gethostbyname(host.c_str());
    if (!server) return -1;

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    memcpy(&addr.sin_addr.s_addr, server->h_addr, server->h_length);

    if (connect(sock, (sockaddr*)&addr, sizeof(addr)) < 0) {
        close(sock);
        return -1;
    }

    return sock;
}

void ProxyServer::handleClient(int clientFd) {
    char buffer[4096];
    memset(buffer, 0, sizeof(buffer));

    int bytes = recv(clientFd, buffer, sizeof(buffer) - 1, 0);
    if (bytes <= 0) {
        close(clientFd);
        return;
    }

    std::string request(buffer);
    HttpRequest req = HttpParser::parse(request);
Logger::log("Request for host: " + req.host);

if (isBlocked(req.host)) {
    std::cout << "[BLOCKED] Host: " << req.host << std::endl;
    Logger::log("Blocked host: " + req.host);
    send403(clientFd);
    close(clientFd);
    return;
}

    int serverFd = connectToServer(req.host, req.port);
    if (serverFd < 0) {
        send503(clientFd);
        close(clientFd);
        return;
    }

    // Fix proxy-form to origin-form
    size_t pos = request.find(req.uri);
    if (pos != std::string::npos) {
        size_t p = req.uri.find('/', 7);
        std::string path = (p == std::string::npos) ? "/" : req.uri.substr(p);
        request.replace(pos, req.uri.length(), path);
    }

    send(serverFd, request.c_str(), request.size(), 0);

    char response[4096];
    int n;
    while ((n = recv(serverFd, response, sizeof(response), 0)) > 0) {
        send(clientFd, response, n, 0);
    }

    close(serverFd);
    close(clientFd);
}

void ProxyServer::start(int port) {
    int serverFd = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(serverFd, (sockaddr*)&addr, sizeof(addr));
    listen(serverFd, 10);

    std::cout << "✓ Listening on port " << port << std::endl;

    while (true) {
        int clientFd = accept(serverFd, nullptr, nullptr);
        std::thread(&ProxyServer::handleClient, this, clientFd).detach();
    }
}
