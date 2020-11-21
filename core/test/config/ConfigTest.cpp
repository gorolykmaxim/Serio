#include <gtest/gtest.h>
#include <SQLiteCpp/Database.h>
#include <NFHTTP/ResponseImplementation.h>
#include <CacheMock.h>
#include <NFClientMock.h>
#include <config/Config.h>
#include <nlohmann/json.hpp>

MATCHER_P(IsRequest, r, "") { return arg->url() == r->url(); }

class ConfigTest : public ::testing::Test {
public:
    inline static const serio::HttpClientConfig httpClientConfig{{"user-agent 1"}};
    inline static const std::vector<serio::EpisodeCrawlerConfig> episodeCrawlerConfigs = std::vector({
        serio::EpisodeCrawlerConfig{"Platform 1",
                                    std::chrono::milliseconds(180000),
                                    "tv show crawler code",
                                    "episode crawler code"},
        serio::EpisodeCrawlerConfig{"Platform 2",
                                    std::chrono::milliseconds(380000),
                                    "tv show crawler code 2",
                                    "episode crawler code 2"}
    });
    const std::string sourceUrl = "https://serio.com/config.json";
    const std::shared_ptr<nativeformat::http::Request> request = nativeformat::http::createRequest(sourceUrl, {});
    const nlohmann::json jsonResponse = {
            {"platforms", {
                {
                    {"name", episodeCrawlerConfigs[0].platformName},
                    {"episode", {
                        {"cache-ttl", episodeCrawlerConfigs[0].cacheTtl.count()},
                        {"tvShowCrawler", episodeCrawlerConfigs[0].tvShowCrawler},
                        {"episodeCrawler", episodeCrawlerConfigs[0].episodeCrawler}
                    }}
                },
                {
                    {"name", episodeCrawlerConfigs[1].platformName},
                    {"episode", {
                            {"cache-ttl", episodeCrawlerConfigs[1].cacheTtl.count()},
                            {"tvShowCrawler", episodeCrawlerConfigs[1].tvShowCrawler},
                            {"episodeCrawler", episodeCrawlerConfigs[1].episodeCrawler}
                    }}
                }
            }},
            {"http-client", {
                {"user-agents", httpClientConfig.userAgents}
            }}
    };

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

TEST_F(ConfigTest, shouldGetEmptyVectorOfEpisodeCrawlerConfigsSinceNoPlatformsAreConfigured) {
    mockClientResponse({});
    config.setSourceUrl(sourceUrl);
    EXPECT_TRUE(config.getEpisodeCrawlerConfigs().empty());
}

TEST_F(ConfigTest, shouldSkipPlatformEpisodeCrawlerConfigsThatLackFields) {
    mockClientResponse({{"platforms", {jsonResponse["platforms"][0], {}}}});
    config.setSourceUrl(sourceUrl);
    EXPECT_EQ(std::vector({episodeCrawlerConfigs[0]}), config.getEpisodeCrawlerConfigs());
}

struct FetchAsserts {
    std::function<void(serio::Config&)> callGetter;
    std::function<void(serio::Config&)> compareResults;
};

class ConfigFetchTest : public ConfigTest, public ::testing::WithParamInterface<FetchAsserts> {};

TEST_P(ConfigFetchTest, shouldFailToGetConfigSinceSourceUrlIsNotSet) {
    EXPECT_THROW(GetParam().callGetter(config), serio::ConfigFetchError);
}

TEST_P(ConfigFetchTest, shouldFailToGetConfigDueToHttpError) {
    EXPECT_CALL(httpClient, performRequestSynchronously(IsRequest(request)))
        .WillOnce(::testing::Throw(std::runtime_error("")));
    config.setSourceUrl(sourceUrl);
    EXPECT_THROW(GetParam().callGetter(config), serio::ConfigFetchError);
}

TEST_P(ConfigFetchTest, shouldGetConfig) {
    mockClientResponse(jsonResponse);
    config.setSourceUrl(sourceUrl);
    GetParam().compareResults(config);
}

TEST_P(ConfigFetchTest, shouldGetConfigFromCache) {
    ON_CALL(cache, get(serio::ConfigSource::CACHE_ENTRY_NAME))
        .WillByDefault(::testing::Return(std::optional<std::string>(jsonResponse.dump())));
    config.setSourceUrl(sourceUrl);
    EXPECT_CALL(httpClient, performRequestSynchronously(::testing::_)).Times(0);
    GetParam().compareResults(config);
}

TEST_P(ConfigFetchTest, shouldSaveConfigToCache) {
    config.setSourceUrl(sourceUrl);
    mockClientResponse(jsonResponse);
    EXPECT_CALL(cache, put(
            serio::ConfigSource::CACHE_ENTRY_NAME,
            jsonResponse.dump(),
            std::chrono::duration_cast<std::chrono::milliseconds>(serio::ConfigSource::CACHE_TTL)));
    GetParam().callGetter(config);
}

INSTANTIATE_TEST_SUITE_P(HttpClientConfigFetchTest,
                         ConfigFetchTest,
                         ::testing::Values(FetchAsserts{
                             [] (serio::Config& config) { config.getHttpClientConfig(); },
                             [] (serio::Config& config) { EXPECT_EQ(ConfigTest::httpClientConfig, config.getHttpClientConfig()); }
                         }));

INSTANTIATE_TEST_SUITE_P(EpisodeCrawlerConfigFetchTest,
                         ConfigFetchTest,
                         ::testing::Values(FetchAsserts{
                            [] (serio::Config& config) { config.getEpisodeCrawlerConfigs(); },
                            [] (serio::Config& config) { EXPECT_EQ(ConfigTest::episodeCrawlerConfigs, config.getEpisodeCrawlerConfigs()); }
                        }));