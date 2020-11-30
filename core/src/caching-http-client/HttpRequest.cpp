#include <caching-http-client/HttpRequest.h>

#include <utility>
#include <sstream>

namespace serio {
HttpRequest::operator std::shared_ptr<nativeformat::http::Request>() const {
    std::unordered_map<std::string, std::string> headersMap(headers.cbegin(), headers.cend());
    auto request = nativeformat::http::createRequest(url, headersMap);
    if (!method.empty()) {
        request->setMethod(method);
    }
    if (!body.empty()) {
        request->setData(reinterpret_cast<const unsigned char*>(body.c_str()), body.length() + 1);
    }
    return request;
}

HttpRequest::operator std::string() const {
    std::stringstream request;
    request << "Request{";
    request << "url=" << url << ", ";
    request << "method=" << method << ", ";
    request << "body=" << body << "}";
    return request.str();
}

bool HttpRequest::operator==(const HttpRequest &rhs) const {
    return url == rhs.url &&
           method == rhs.method &&
           headers == rhs.headers &&
           body == rhs.body;
}

bool HttpRequest::operator!=(const HttpRequest &rhs) const {
    return !(rhs == *this);
}
}