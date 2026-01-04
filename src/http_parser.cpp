#include "http_parser.h"
#include <sstream>

HttpRequest HttpParser::parse(const std::string& request) {
    HttpRequest req;
    req.port = 80; // default HTTP port

    std::istringstream stream(request);
    std::string line;

    // Parse request line: METHOD URI HTTP/1.1
    if (std::getline(stream, line)) {
        std::istringstream lineStream(line);
        lineStream >> req.method >> req.uri;
    }

    // Parse headers
    while (std::getline(stream, line)) {
        if (line == "\r" || line.empty())
            break;

        if (line.find("Host:") == 0) {
            std::string hostLine = line.substr(5);
            while (!hostLine.empty() && (hostLine[0] == ' ' || hostLine[0] == '\t'))
                hostLine.erase(0, 1);

            // Remove CRLF
            if (!hostLine.empty() && hostLine.back() == '\r')
                hostLine.pop_back();

            // Check for port in Host header
            size_t colonPos = hostLine.find(':');
            if (colonPos != std::string::npos) {
                req.host = hostLine.substr(0, colonPos);
                req.port = std::stoi(hostLine.substr(colonPos + 1));
            } else {
                req.host = hostLine;
            }
        }
    }

    return req;
}
