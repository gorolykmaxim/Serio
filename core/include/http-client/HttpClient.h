#ifndef SERIO_HTTPCLIENT_H
#define SERIO_HTTPCLIENT_H

#include <future>
#include <NFHTTP/Client.h>
#include <cache/Cache.h>
#include "HttpRequest.h"
#include "HttpResponse.h"

namespace serio {
class HttpClient {
public:
    HttpClient(nativeformat::http::Client& client, Cache& cache);
    virtual HttpResponse sendRequest(const HttpRequest& request, const std::chrono::milliseconds& cacheTtl);
private:
    nativeformat::http::Client& client;
    Cache& cache;

    std::string readBodyFromResponse(const std::shared_ptr<nativeformat::http::Response>& response,
                                     const HttpRequest& request,
                                     const std::chrono::milliseconds& cacheTtl);
    std::string readBodyFromResponse(const std::shared_ptr<nativeformat::http::Response>& response);
    bool isWindows1251(const std::shared_ptr<nativeformat::http::Response>& response);
};

class HttpResponseError : public std::runtime_error {
public:
    HttpResponseError(const std::string& url, int code, const std::string& body);
};
}

#endif //SERIO_HTTPCLIENT_H
