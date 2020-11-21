#include <gtest/gtest.h>
#include <NFHTTP/NFHTTP.h>
#include <NFHTTP/ResponseImplementation.h>
#include <caching-http-client/CachingHttpClient.h>
#include <NFClientMock.h>
#include <CacheMock.h>

class CachingHttpClientTest : public ::testing::Test {
protected:
    const std::string rawResponse = "response";
    const std::string url = "https://some.url.com/";
    const std::shared_ptr<nativeformat::http::Request> request = nativeformat::http::createRequest(url, {});
    const std::chrono::hours cacheTtl = std::chrono::hours(24);
    const std::shared_ptr<nativeformat::http::Response> response = std::make_shared<nativeformat::http::ResponseImplementation>(
            request, reinterpret_cast<const unsigned char*>(rawResponse.c_str()), rawResponse.size() + 1,
            nativeformat::http::StatusCode::StatusCodeOK, false);

    ::testing::NiceMock<mocks::CacheMock> cache;
    mocks::NFClientMock client;
    serio::CachingHttpClient cachingClient = serio::CachingHttpClient(client, cache);

    void mockHttpClientResponse(const std::shared_ptr<nativeformat::http::Response>& res) {
        const auto invokeResponseCallback = ::testing::InvokeArgument<1>(res);
        const auto returnNull = ::testing::Return(std::shared_ptr<nativeformat::http::RequestToken>(nullptr));
        EXPECT_CALL(client, performRequest(IsRequest(request), ::testing::_))
            .WillOnce(::testing::DoAll(invokeResponseCallback, returnNull));
    }
};

TEST_F(CachingHttpClientTest, shouldGetResponseFromCache) {
    EXPECT_CALL(cache, get(url)).WillOnce(::testing::Return(std::optional(rawResponse)));
    EXPECT_CALL(client, performRequest(::testing::_, ::testing::_)).Times(0);
    EXPECT_EQ(rawResponse, cachingClient.sendRequest(request, cacheTtl).get());
}

TEST_F(CachingHttpClientTest, shouldMissCacheGetResponseFromNetworkAndCacheIt) {
    EXPECT_CALL(cache, get(url)).WillOnce(::testing::Return(std::optional<std::string>()));
    EXPECT_CALL(cache, put(url, rawResponse, std::chrono::duration_cast<std::chrono::milliseconds>(cacheTtl)));
    mockHttpClientResponse(response);
    EXPECT_EQ(rawResponse, cachingClient.sendRequest(request, cacheTtl).get());
}

TEST_F(CachingHttpClientTest, shouldMissCacheAndFailToGetResponseFromNetwork) {
    const auto error = std::make_shared<nativeformat::http::ResponseImplementation>(request,
                                                                                    nullptr,
                                                                                    0,
                                                                                    nativeformat::http::StatusCode::StatusCodeBadRequest,
                                                                                    false);
    mockHttpClientResponse(error);
    EXPECT_THROW(cachingClient.sendRequest(request, cacheTtl).get(), serio::HttpResponseError);
}
