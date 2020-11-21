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
            writeResponseTo(response, responsePromise, cacheTtl);
        });
    }
    return responsePromise->get_future();
}

void CachingHttpClient::writeResponseTo(const std::shared_ptr<nativeformat::http::Response> &response,
                                        const std::shared_ptr<std::promise<std::string>> &promise,
                                        const std::chrono::milliseconds &cacheTtl) {
    const auto responseBody = readBodyFromResponse(response);
    if (response->statusCode() >= nativeformat::http::StatusCode::StatusCodeBadRequest) {
        writeErrorResponseTo(response, promise, responseBody);
    } else {
        writeSuccessResponseTo(response, promise, responseBody, cacheTtl);
    }
}

std::string CachingHttpClient::readBodyFromResponse(const std::shared_ptr<nativeformat::http::Response>& response) {
    size_t size;
    const auto data = reinterpret_cast<const char*>(response->data(size));
    return size > 0 ? std::string(data) : "";
}

void CachingHttpClient::writeErrorResponseTo(const std::shared_ptr<nativeformat::http::Response>& response,
                                             const std::shared_ptr<std::promise<std::string>>& promise,
                                             const std::string& responseBody) {
    HttpResponseError responseError(response->request()->url(), response->statusCode(), responseBody);
    const auto exception = std::make_exception_ptr(responseError);
    promise->set_exception(exception);
}

void CachingHttpClient::writeSuccessResponseTo(const std::shared_ptr<nativeformat::http::Response> &response,
                                               const std::shared_ptr<std::promise<std::string>> &promise,
                                               const std::string &responseBody,
                                               const std::chrono::milliseconds& cacheTtl) {
    cache.put(response->request()->url(), responseBody, cacheTtl);
    promise->set_value(responseBody);
}

HttpResponseError::HttpResponseError(const std::string &url, int code, const std::string &body)
    : std::runtime_error("Request to " + url + " failed: " + std::to_string(code) + " " + body) {}
}