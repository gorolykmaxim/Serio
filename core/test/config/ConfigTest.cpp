#include <gtest/gtest.h>
#include <SQLiteCpp/Database.h>
#include <NFHTTP/ResponseImplementation.h>
#include <CacheMock.h>
#include <NFClientMock.h>
#include <config/Config.h>
#include <nlohmann/json.hpp>

MATCHER_P(IsRequest, r, "") { return arg->url() == r->url(); }

class ConfigTest : public ::testing::Test {
protected:
    const std::string sourceUrl = "https://serio.com/config.json";
    const std::shared_ptr<nativeformat::http::Request> request = nativeformat::http::createRequest(sourceUrl, {});
    const serio::HttpClientConfig httpClientConfig = {{"user-agent 1"}};
    const nlohmann::json jsonResponse = {{"http-client", {{"user-agents", httpClientConfig.userAgents}}}};

    SQLite::Database database = SQLite::Database(":memory:", SQLite::OPEN_READWRITE);
    ::testing::NiceMock<mocks::CacheMock> cache;
    mocks::NFClientMock httpClient;
    serio::Config config = serio::Config(database, cache, httpClient);

    void mockClientResponse(const nlohmann::json& response) {
        const auto rawResponse = response.dump();
        const auto httpResponse = std::make_shared<nativeformat::http::ResponseImplementation>(
            request,
            reinterpret_cast<const unsigned char*>(rawResponse.c_str()),
            rawResponse.size() + 1,
            nativeformat::http::StatusCode::StatusCodeOK,
            false);
        EXPECT_CALL(httpClient, performRequestSynchronously(IsRequest(request)))
            .WillOnce(::testing::Return(httpResponse));
    }
};

TEST_F(ConfigTest, shouldSetSourceUrl) {
    EXPECT_FALSE(config.getSourceUrl());
    config.setSourceUrl(sourceUrl);
    EXPECT_EQ(sourceUrl, *config.getSourceUrl());
}

TEST_F(ConfigTest, shouldReplaceExistingSourceUrlWithNewOne) {
    config.setSourceUrl("");
    config.setSourceUrl(sourceUrl);
    EXPECT_EQ(sourceUrl, config.getSourceUrl());
}

TEST_F(ConfigTest, shouldKeepSourceUrlInDatabase) {
    serio::Config anotherConfig(database, cache, httpClient);
    anotherConfig.setSourceUrl(sourceUrl);
    EXPECT_EQ(sourceUrl, *config.getSourceUrl());
}

// HTTP Client Config

TEST_F(ConfigTest, shouldFailToGetHttpClientConfigSinceSourceUrlIsNotSet) {
    EXPECT_THROW(config.getHttpClientConfig(), serio::ConfigFetchError);
}

TEST_F(ConfigTest, shouldFailToGetHttpClientConfigDueToHttpError) {
    EXPECT_CALL(httpClient, performRequestSynchronously(IsRequest(request)))
        .WillOnce(::testing::Throw(std::runtime_error("")));
    config.setSourceUrl(sourceUrl);
    EXPECT_THROW(config.getHttpClientConfig(), serio::ConfigFetchError);
}

TEST_F(ConfigTest, shouldGetHttpClientConfig) {
    mockClientResponse(jsonResponse);
    config.setSourceUrl(sourceUrl);
    EXPECT_EQ(httpClientConfig, config.getHttpClientConfig());
}

TEST_F(ConfigTest, shouldGetHttpClientConfigWithoutUserAgentsSpecified) {
    mockClientResponse({{"http-client", {}}});
    config.setSourceUrl(sourceUrl);
    EXPECT_EQ(serio::HttpClientConfig(), config.getHttpClientConfig());
}

TEST_F(ConfigTest, shouldGetEmptyHttpClientConfig) {
    mockClientResponse({});
    config.setSourceUrl(sourceUrl);
    EXPECT_EQ(serio::HttpClientConfig(), config.getHttpClientConfig());
}

TEST_F(ConfigTest, shouldGetHttpClientConfigFromCache) {
    ON_CALL(cache, get(serio::ConfigSource::CACHE_ENTRY_NAME))
        .WillByDefault(::testing::Return(std::optional<std::string>(jsonResponse.dump())));
    config.setSourceUrl(sourceUrl);
    EXPECT_CALL(httpClient, performRequestSynchronously(::testing::_)).Times(0);
    EXPECT_EQ(httpClientConfig, config.getHttpClientConfig());
}

TEST_F(ConfigTest, shouldSaveHttpClientConfigToCache) {
    config.setSourceUrl(sourceUrl);
    mockClientResponse(jsonResponse);
    EXPECT_CALL(cache, put(
            serio::ConfigSource::CACHE_ENTRY_NAME,
            jsonResponse.dump(),
            std::chrono::duration_cast<std::chrono::milliseconds>(serio::ConfigSource::CACHE_TTL)));
    config.getHttpClientConfig();
}
