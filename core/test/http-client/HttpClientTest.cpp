#include <gtest/gtest.h>
#include <NFHTTP/NFHTTP.h>
#include <NFHTTP/ResponseImplementation.h>
#include <http-client/HttpClient.h>
#include <NFClientMock.h>
#include <CacheMock.h>
#include <iconvlite.h>

class HttpClientTest : public ::testing::Test {
protected:
    const std::string rawResponse = "response";
    const std::string utfResponse = "Поиск по сайту";
    const std::string url = "https://some.url.com/";
    const serio::HttpRequest request{url};
    const std::string requestCacheKey = request;
    const std::chrono::milliseconds cacheTtl = std::chrono::hours(24);
    const serio::HttpRequest complexRequest{url,
                                            "POST",
                                            {{"header 1", "value 1"}, {"header 2", "value 2"}},
                                            "body of the request"};
    const std::shared_ptr<nativeformat::http::Response> response = std::make_shared<nativeformat::http::ResponseImplementation>(
            request, reinterpret_cast<const unsigned char*>(rawResponse.c_str()), rawResponse.size() + 1,
            nativeformat::http::StatusCode::StatusCodeOK, false);
    const std::shared_ptr<nativeformat::http::Response> error = std::make_shared<nativeformat::http::ResponseImplementation>(
            request, nullptr, 0, nativeformat::http::StatusCode::StatusCodeBadRequest, false);

    ::testing::NiceMock<mocks::CacheMock> cache;
    mocks::NFClientMock client;
    serio::HttpClient httpClient = serio::HttpClient(client, cache);

    virtual void SetUp() {
        ON_CALL(cache, get(::testing::_, false)).WillByDefault(::testing::Return(std::optional<std::string>()));
    }

    void mockHttpClientResponse(const std::shared_ptr<nativeformat::http::Response>& res) {
        const auto invokeResponseCallback = ::testing::InvokeArgument<1>(res);
        const auto returnNull = ::testing::Return(std::shared_ptr<nativeformat::http::RequestToken>(nullptr));
        EXPECT_CALL(client, performRequest(IsRequest(static_cast<std::shared_ptr<nativeformat::http::Request>>(request)), ::testing::_))
            .WillOnce(::testing::DoAll(invokeResponseCallback, returnNull));
    }

    void mockHttpClientResponse(const std::string& content, const std::string& charset) {
        auto response = std::make_shared<nativeformat::http::ResponseImplementation>(
                request,
                reinterpret_cast<const unsigned char*>(content.c_str()),
                content.size() + 1,
                nativeformat::http::StatusCode::StatusCodeOK,
                false);
        response->operator[]("Content-Type") = "text/html; CHARSET=" + charset;
        mockHttpClientResponse(response);
    }
};

TEST_F(HttpClientTest, shouldGetResponseFromCache) {
    EXPECT_CALL(cache, get(requestCacheKey, false)).WillOnce(::testing::Return(std::optional(rawResponse)));
    EXPECT_CALL(client, performRequest(::testing::_, ::testing::_)).Times(0);
    EXPECT_EQ(rawResponse, httpClient.sendRequest(request, cacheTtl).get());
}

TEST_F(HttpClientTest, shouldMissCacheGetResponseFromNetworkAndCacheIt) {
    EXPECT_CALL(cache, get(requestCacheKey, false)).WillOnce(::testing::Return(std::optional<std::string>()));
    EXPECT_CALL(cache, put(requestCacheKey, rawResponse, cacheTtl));
    mockHttpClientResponse(response);
    EXPECT_EQ(rawResponse, httpClient.sendRequest(request, cacheTtl).get());
}

TEST_F(HttpClientTest, shouldCacheResponseOnCallingThread) {
    EXPECT_CALL(cache, get(requestCacheKey, false)).WillOnce(::testing::Return(std::optional<std::string>()));
    EXPECT_CALL(cache, put(requestCacheKey, rawResponse, cacheTtl))
        .Times(0);
    mockHttpClientResponse(response);
    auto responseFuture = httpClient.sendRequest(request, cacheTtl);
    EXPECT_CALL(cache, put(requestCacheKey, rawResponse, std::chrono::duration_cast<std::chrono::milliseconds>(cacheTtl)));
    responseFuture.get();
}

TEST_F(HttpClientTest, shouldMissCacheAndFailToGetResponseFromNetwork) {
    mockHttpClientResponse(error);
    EXPECT_THROW(httpClient.sendRequest(request, cacheTtl).get(), serio::HttpResponseError);
}

TEST_F(HttpClientTest, shouldMissCacheFailToGetResponseFromNetworkAndReturnExpiredEntryFromCache) {
    mockHttpClientResponse(error);
    EXPECT_CALL(cache, get(requestCacheKey, false)).WillOnce(::testing::Return(std::optional<std::string>()));
    EXPECT_CALL(cache, get(requestCacheKey, true)).WillOnce(::testing::Return(std::optional(rawResponse)));
    EXPECT_EQ(rawResponse, httpClient.sendRequest(request, cacheTtl).get());
}

TEST_F(HttpClientTest, shouldUseRequestUrlMethodAndBodyAsCacheKey) {
    EXPECT_CALL(cache, get("Request{url=" + url + ", method=POST, body=body of the request}", false))
        .WillOnce(::testing::Return(std::optional(rawResponse)));
    httpClient.sendRequest(complexRequest, cacheTtl);
}

TEST_F(HttpClientTest, shouldPassAllRequestAttributesToNFClient) {
    const auto expectedRequest = nativeformat::http::createRequest(
            complexRequest.url,
            std::unordered_map<std::string, std::string>(complexRequest.headers.cbegin(), complexRequest.headers.cend()));
    expectedRequest->setMethod(complexRequest.method);
    expectedRequest->setData(reinterpret_cast<const unsigned char *>(complexRequest.body.c_str()), complexRequest.body.length() + 1);
    EXPECT_CALL(client, performRequest(IsRequest(expectedRequest), ::testing::_));
    httpClient.sendRequest(complexRequest, cacheTtl);
}

TEST_F(HttpClientTest, shouldConvertWindows1251ResponseToUtf8WithContentTypeLowercase) {
    const auto winResponse = iconvlite::utf2cp(utfResponse);
    mockHttpClientResponse(winResponse, "windows-1251");
    EXPECT_EQ(utfResponse, httpClient.sendRequest(request, cacheTtl).get());
}

TEST_F(HttpClientTest, shouldConvertWindows1251ResponseToUtf8WithContentTypeUppercase) {
    const auto winResponse = iconvlite::utf2cp(utfResponse);
    mockHttpClientResponse(winResponse, "WINDOWS-1251");
    EXPECT_EQ(utfResponse, httpClient.sendRequest(request, cacheTtl).get());
}

TEST_F(HttpClientTest, shouldNotConvertResponseToUtf8IfItIsNotWindows1251) {
    const auto unknownResponse = iconvlite::utf2cp(utfResponse);
    mockHttpClientResponse(unknownResponse, "unknown-charset");
    EXPECT_EQ(unknownResponse, httpClient.sendRequest(request, cacheTtl).get());
}
