#ifndef SERIO_HTTPREQUEST_H
#define SERIO_HTTPREQUEST_H

#include <string>
#include <map>
#include <NFHTTP/NFHTTP.h>

namespace serio {
struct HttpRequest {
    std::string url;
    std::string method;
    std::map<std::string, std::string> headers;
    std::string body;

    operator std::shared_ptr<nativeformat::http::Request>() const;
    operator std::string() const;
    bool operator==(const HttpRequest &rhs) const;
    bool operator!=(const HttpRequest &rhs) const;
};
}

#endif //SERIO_HTTPREQUEST_H
