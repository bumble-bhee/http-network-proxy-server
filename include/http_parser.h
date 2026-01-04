#ifndef HTTP_PARSER_H
#define HTTP_PARSER_H

#include <string>

/*
 * Structure representing a parsed HTTP request
 */
struct HttpRequest {
    std::string method;
    std::string uri;
    std::string host;
    int port;
};

/*
 * HttpParser parses raw HTTP request strings
 */
class HttpParser {
public:
    static HttpRequest parse(const std::string& request);
};

#endif
