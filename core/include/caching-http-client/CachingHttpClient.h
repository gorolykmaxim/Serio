#ifndef SERIO_CACHINGHTTPCLIENT_H
#define SERIO_CACHINGHTTPCLIENT_H

#include <future>
#include <NFHTTP/Client.h>
#include <cache/Cache.h>

namespace serio {
class CachingHttpClient {
public:
    CachingHttpClient(nativeformat::http::Client& client, Cache& cache);
    std::future<std::string> sendRequest(const std::shared_ptr<nativeformat::http::Request>& request,
                                         const std::chrono::milliseconds& cacheTtl);
private:
    nativeformat::http::Client& client;
    Cache& cache;

    void writeResponseTo(const std::shared_ptr<nativeformat::http::Response>& response,
                         const std::shared_ptr<std::promise<std::string>>& promise,
                         const std::chrono::milliseconds& cacheTtl);
    std::string readBodyFromResponse(const std::shared_ptr<nativeformat::http::Response>& response);
    void writeErrorResponseTo(const std::shared_ptr<nativeformat::http::Response>& response,
                              const std::shared_ptr<std::promise<std::string>>& promise,
                              const std::string& responseBody);
    void writeSuccessResponseTo(const std::shared_ptr<nativeformat::http::Response>& response,
                                const std::shared_ptr<std::promise<std::string>>& promise,
                                const std::string& responseBody,
                                const std::chrono::milliseconds& cacheTtl);
};

class HttpResponseError : std::runtime_error {
public:
    HttpResponseError(const std::string& url, int code, const std::string& body);
};
}

#endif //SERIO_CACHINGHTTPCLIENT_H
