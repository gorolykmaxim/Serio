#include <caching-http-client/CachingHttpClient.h>
#include <iconvlite.h>

namespace serio {
CachingHttpClient::CachingHttpClient(nativeformat::http::Client &client, Cache &cache)
    : client(client),
      cache(cache) {}

HttpResponse CachingHttpClient::sendRequest(const HttpRequest& request, const std::chrono::milliseconds& cacheTtl) {
    const auto cachedResponse = cache.get(request);
    if (cachedResponse) {
        return HttpResponse(*cachedResponse);
    } else {
        const auto responsePromise = std::make_shared<std::promise<std::shared_ptr<nativeformat::http::Response>>>();
        client.performRequest(request, [responsePromise] (const auto& response) {responsePromise->set_value(response);});
        return HttpResponse([this, responsePromise, request, cacheTtl] () {
            return readBodyFromResponse(responsePromise->get_future().get(), request, cacheTtl);
        });
    }
}

std::string CachingHttpClient::readBodyFromResponse(const std::shared_ptr<nativeformat::http::Response> &response,
                                                    const HttpRequest& request,
                                                    const std::chrono::milliseconds &cacheTtl) {
    auto responseBody = readBodyFromResponse(response);
    if (response->statusCode() >= nativeformat::http::StatusCode::StatusCodeBadRequest) {
        const auto expiredCachedResponse = cache.get(request, true);
        if (expiredCachedResponse) {
            return *expiredCachedResponse;
        } else {
            throw HttpResponseError(request.url, response->statusCode(), responseBody);
        }
    } else {
        cache.put(request, responseBody, cacheTtl);
        return responseBody;
    }
}

std::string CachingHttpClient::readBodyFromResponse(const std::shared_ptr<nativeformat::http::Response>& response) {
    size_t size;
    const auto data = reinterpret_cast<const char*>(response->data(size));
    if (size == 0) {
        return "";
    }
    return isWindows1251(response) ? iconvlite::cp2utf(data) : data;
}

bool CachingHttpClient::isWindows1251(const std::shared_ptr<nativeformat::http::Response> &response) {
    auto contentType = response->operator[]("Content-Type");
    std::transform(contentType.begin(), contentType.end(), contentType.begin(), std::tolower);
    return contentType.rfind("windows-1251") != std::string::npos;
}

HttpResponseError::HttpResponseError(const std::string &url, int code, const std::string &body)
    : std::runtime_error("Request to " + url + " failed: " + std::to_string(code) + " " + body) {}
}