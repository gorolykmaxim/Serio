#ifndef SERIO_CACHINGHTTPCLIENT_H
#define SERIO_CACHINGHTTPCLIENT_H

#include <future>
#include <NFHTTP/Client.h>
#include <cache/Cache.h>
#include "HttpRequest.h"

namespace serio {
class CachingHttpClient {
public:
    CachingHttpClient(nativeformat::http::Client& client, Cache& cache);
    virtual std::future<std::string> sendRequest(const HttpRequest& request, const std::chrono::milliseconds& cacheTtl);
private:
    nativeformat::http::Client& client;
    Cache& cache;

    void writeResponseToPromise(const std::shared_ptr<nativeformat::http::Response>& response,
                                const std::shared_ptr<std::promise<std::string>>& promise,
                                const HttpRequest& request,
                                const std::chrono::milliseconds& cacheTtl);
    std::string readBodyFromResponse(const std::shared_ptr<nativeformat::http::Response>& response);
};

class HttpResponseError : std::runtime_error {
public:
    HttpResponseError(const std::string& url, int code, const std::string& body);
};
}

#endif //SERIO_CACHINGHTTPCLIENT_H
