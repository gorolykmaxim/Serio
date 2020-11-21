#include <caching-http-client/HttpRequest.h>

#include <utility>

namespace serio {
HttpRequest::operator std::shared_ptr<nativeformat::http::Request>() const {
    return nativeformat::http::createRequest(url, {});
}

HttpRequest::operator std::string() const {
    return url;
}
}