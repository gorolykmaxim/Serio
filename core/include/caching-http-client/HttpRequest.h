#ifndef SERIO_HTTPREQUEST_H
#define SERIO_HTTPREQUEST_H

#include <string>
#include <NFHTTP/NFHTTP.h>

namespace serio {
struct HttpRequest {
    const std::string url;

    operator std::shared_ptr<nativeformat::http::Request>() const;
    operator std::string() const;
    bool operator==(const HttpRequest &rhs) const;
    bool operator!=(const HttpRequest &rhs) const;
};
}

#endif //SERIO_HTTPREQUEST_H
