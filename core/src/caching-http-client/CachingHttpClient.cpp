#include <caching-http-client/CachingHttpClient.h>

namespace serio {
CachingHttpClient::CachingHttpClient(nativeformat::http::Client &client, Cache &cache)
    : client(client),
      cache(cache) {}

std::future<std::string> CachingHttpClient::sendRequest(const std::shared_ptr<nativeformat::http::Request>& request,
                                                        const std::chrono::milliseconds &cacheTtl) {
    const auto responsePromise = std::make_shared<std::promise<std::string>>();
    const auto cachedResponse = cache.get(request->url());
    if (cachedResponse) {
        responsePromise->set_value(*cachedResponse);
    } else {
        client.performRequest(request, [this, responsePromise, cacheTtl] (const auto& response) {
            writeResponseToPromise(response, responsePromise, cacheTtl);
        });
    }
    return responsePromise->get_future();
}

void CachingHttpClient::writeResponseToPromise(const std::shared_ptr<nativeformat::http::Response> &response,
                                               const std::shared_ptr<std::promise<std::string>> &promise,
                                               const std::chrono::milliseconds &cacheTtl) {
    const auto url = response->request()->url();
    const auto responseBody = readBodyFromResponse(response);
    if (response->statusCode() >= nativeformat::http::StatusCode::StatusCodeBadRequest) {
        const auto expiredCachedResponse = cache.get(url, true);
        if (expiredCachedResponse) {
            promise->set_value(*expiredCachedResponse);
        } else {
            HttpResponseError responseError(url, response->statusCode(), responseBody);
            const auto exception = std::make_exception_ptr(responseError);
            promise->set_exception(exception);
        }
    } else {
        cache.put(url, responseBody, cacheTtl);
        promise->set_value(responseBody);
    }
}

std::string CachingHttpClient::readBodyFromResponse(const std::shared_ptr<nativeformat::http::Response>& response) {
    size_t size;
    const auto data = reinterpret_cast<const char*>(response->data(size));
    return size > 0 ? std::string(data) : "";
}

HttpResponseError::HttpResponseError(const std::string &url, int code, const std::string &body)
    : std::runtime_error("Request to " + url + " failed: " + std::to_string(code) + " " + body) {}
}