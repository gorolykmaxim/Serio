#include <gtest/gtest.h>
#include <caching-http-client/HttpRequest.h>
#include <ConfigMock.h>
#include <crawler-http-client/CrawlerHttpClient.h>

class CrawlerHttpClientTest : public ::testing::Test {
protected:
    const std::string response = "response body";
    const std::chrono::milliseconds cacheTtl = std::chrono::milliseconds(2134567);
    const serio::HttpClientConfig httpClientConfig{{"User Agent 1", "User Agent 2"}};
    const serio::HttpRequest originalRequest{"", "", {{"some-header", "some header value"}}};
    
    mocks::CachingHttpClientMock cachingClient;
    ::testing::NiceMock<mocks::ConfigMock> config;
    serio::CrawlerHttpClient client = serio::CrawlerHttpClient(config, cachingClient);

    serio::HttpRequest executeAndCaptureRequest() {
        serio::HttpRequest actualRequest;
        serio::HttpResponse httpResponse(response);
        const auto captureRequest = ::testing::SaveArg<0>(&actualRequest);
        const auto returnResponse = ::testing::Return(httpResponse);
        EXPECT_CALL(cachingClient, sendRequest(::testing::_, cacheTtl))
            .WillOnce(::testing::DoAll(captureRequest, returnResponse));
        EXPECT_EQ(response, client.sendRequest(originalRequest, cacheTtl).get());
        return actualRequest;
    }
};

TEST_F(CrawlerHttpClientTest, shouldAppendRandomUserAgentFromConfigToRequestAndSendItToDelegate) {
    ON_CALL(config, getHttpClientConfig()).WillByDefault(::testing::Return(httpClientConfig));
    const auto existingAgents = httpClientConfig.userAgents;
    std::set<std::string> usedAgents;
    for (auto i = 0; i < 100; i++) {
        serio::HttpRequest actualRequest = executeAndCaptureRequest();
        usedAgents.emplace(actualRequest.headers["User-Agent"]);
        if (usedAgents.size() == existingAgents.size()) {
            EXPECT_NE(usedAgents.cend(), usedAgents.find(existingAgents[0]));
            EXPECT_NE(usedAgents.cend(), usedAgents.find(existingAgents[1]));
            return;
        }
    }
    GTEST_FATAL_FAILURE_("All existing user agents had to be used at least once");
}