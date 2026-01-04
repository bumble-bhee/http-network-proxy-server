#include <iostream>
#include "http_parser.h"

int main() {
    std::string request =
        "GET http://example.com/page HTTP/1.1\r\n"
        "Host: example.com\r\n"
        "\r\n";

    HttpRequest req = HttpParser::parse(request);

    std::cout << "Method: " << req.method << std::endl;
    std::cout << "URI: " << req.uri << std::endl;
    std::cout << "Host: " << req.host << std::endl;
    std::cout << "Port: " << req.port << std::endl;

    return 0;
}
