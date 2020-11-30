#include <gtest/gtest.h>
#include <SQLiteCpp/Database.h>
#include <NFHTTP/ResponseImplementation.h>
#include <CachingHttpClientMock.h>
#include <config/Config.h>
#include <nlohmann/json.hpp>

class ConfigTest : public ::testing::Test {
public:
    inline static const serio::HttpClientConfig httpClientConfig{{"user-agent 1"}};
    inline static const std::vector<serio::TvShowCrawlerConfig> tvShowCrawlerConfigs = std::vector({
        serio::TvShowCrawlerConfig{"Platform 1",
                                   std::chrono::milliseconds(180000),
                                   "tv show crawler code"},
        serio::TvShowCrawlerConfig{"Platform 2",
                                   std::chrono::milliseconds(380000),
                                   "tv show crawler code 2"}
    });
    inline static const std::vector<serio::SearchCrawlerConfig> searchCrawlerConfigs = std::vector({
        serio::SearchCrawlerConfig{tvShowCrawlerConfigs[0].platformName,
                                   std::chrono::milliseconds(190000),
                                   "search tv show crawler"},
        serio::SearchCrawlerConfig{tvShowCrawlerConfigs[1].platformName,
                                   std::chrono::milliseconds(200000),
                                   "search tv show crawler 2"},
    });
    inline static const std::vector<serio::CategoryCrawlerConfig> categoryCrawlerConfigs = std::vector({
        serio::CategoryCrawlerConfig{tvShowCrawlerConfigs[0].platformName,
                                     "category 1",
                                     std::chrono::milliseconds(150000),
                                     "category tv show crawler"},
        serio::CategoryCrawlerConfig{tvShowCrawlerConfigs[0].platformName,
                                     "category 2",
                                     std::chrono::milliseconds(150000),
                                     "category tv show crawler 2"},
        serio::CategoryCrawlerConfig{tvShowCrawlerConfigs[1].platformName,
                                     "category 1",
                                     std::chrono::milliseconds(100000),
                                     "category tv show crawler 3"}
    });
    const std::string sourceUrl = "https://serio.com/config.json";
    const serio::HttpRequest request{sourceUrl};
    const nlohmann::json jsonResponse = {
            {"platforms", {
                {
                    {"name", tvShowCrawlerConfigs[0].platformName},
                    {"categories", {
                        {"cache-ttl", categoryCrawlerConfigs[0].cacheTtl.count()},
                        {"category-list", {
                            {
                                {"name", categoryCrawlerConfigs[0].categoryName},
                                {"crawler", categoryCrawlerConfigs[0].crawler}
                            },
                            {
                                {"name", categoryCrawlerConfigs[1].categoryName},
                                {"crawler", categoryCrawlerConfigs[1].crawler}
                            }
                        }}
                    }},
                    {"search", {
                       {"cache-ttl", searchCrawlerConfigs[0].cacheTtl.count()},
                       {"crawler", searchCrawlerConfigs[0].crawler}
                    }},
                    {"tv-show", {
                        {"cache-ttl", tvShowCrawlerConfigs[0].cacheTtl.count()},
                        {"crawler", tvShowCrawlerConfigs[0].crawler},
                    }}
                },
                {
                    {"name", tvShowCrawlerConfigs[1].platformName},
                    {"categories", {
                        {"cache-ttl", categoryCrawlerConfigs[2].cacheTtl.count()},
                        {"category-list", {
                            {
                                {"name", categoryCrawlerConfigs[2].categoryName},
                                {"crawler", categoryCrawlerConfigs[2].crawler}
                            }
                        }}
                    }},
                    {"search", {
                        {"cache-ttl", searchCrawlerConfigs[1].cacheTtl.count()},
                        {"crawler", searchCrawlerConfigs[1].crawler}
                    }},
                    {"tv-show", {
                            {"cache-ttl", tvShowCrawlerConfigs[1].cacheTtl.count()},
                            {"crawler", tvShowCrawlerConfigs[1].crawler},
                    }}
                }
            }},
            {"http-client", {
                {"user-agents", httpClientConfig.userAgents}
            }}
    };

    SQLite::Database database = SQLite::Database(":memory:", SQLite::OPEN_READWRITE);
    mocks::CachingHttpClientMock httpClient;
    serio::Config config = serio::Config(database, httpClient);

    void mockClientResponse(const nlohmann::json& response) {
        const auto rawResponse = response.dump();
        std::promise<std::string> res;
        res.set_value(rawResponse);
        const std::chrono::milliseconds cacheTtl = serio::ConfigSource::CACHE_TTL;
        EXPECT_CALL(httpClient, sendRequest(request, cacheTtl))
            .WillOnce(::testing::Return(::testing::ByMove(res.get_future())));
    }
    void mockClientError() {
        const auto exception = std::make_exception_ptr(std::runtime_error(""));
        std::promise<std::string> response;
        response.set_exception(exception);
        const std::chrono::milliseconds cacheTtl = serio::ConfigSource::CACHE_TTL;
        EXPECT_CALL(httpClient, sendRequest(request, cacheTtl))
                .WillOnce(::testing::Return(::testing::ByMove(response.get_future())));
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
    serio::Config anotherConfig(database, httpClient);
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

TEST_F(ConfigTest, shouldGetEmptyVectorOfTvShowCrawlerConfigsSinceNoPlatformsAreConfigured) {
    mockClientResponse({});
    config.setSourceUrl(sourceUrl);
    EXPECT_TRUE(config.getTvShowCrawlerConfigs().empty());
}

TEST_F(ConfigTest, shouldSkipPlatformTvShowCrawlerConfigsThatLackFields) {
    mockClientResponse({{"platforms", {jsonResponse["platforms"][0], {}}}});
    config.setSourceUrl(sourceUrl);
    EXPECT_EQ(std::vector({tvShowCrawlerConfigs[0]}), config.getTvShowCrawlerConfigs());
}

TEST_F(ConfigTest, shouldGetEmptyVectorOfSearchCrawlerConfigsSinceNoPlatformsAreConfigured) {
    mockClientResponse({});
    config.setSourceUrl(sourceUrl);
    EXPECT_TRUE(config.getSearchCrawlerConfigs().empty());
}

TEST_F(ConfigTest, shouldSkipPlatformSearchCrawlerConfigsThatLackFields) {
    mockClientResponse({{"platforms", {jsonResponse["platforms"][0], {}}}});
    config.setSourceUrl(sourceUrl);
    EXPECT_EQ(std::vector({searchCrawlerConfigs[0]}), config.getSearchCrawlerConfigs());
}

TEST_F(ConfigTest, shouldGetEmptyVectorOfCategoryCrawlerConfigsSinceNoPlatformsAreConfigured) {
    mockClientResponse({});
    config.setSourceUrl(sourceUrl);
    EXPECT_TRUE(config.getCategoryCrawlerConfigs().empty());
}

TEST_F(ConfigTest, shouldSkipPlatformCategoryCrawlerConfigsThatLackFields) {
    mockClientResponse({
        {"platforms", {
            {
                {"categories", jsonResponse["platforms"][0]["categories"]}
            },
            {
                {"name", jsonResponse["platforms"][0]["name"]}
            },
            {
                {"name", jsonResponse["platforms"][0]["name"]},
                {"categories", {
                    {"category-list", jsonResponse["platforms"][0]["categories"]["category-list"]}
                }}
            },
            {
                {"name", jsonResponse["platforms"][0]["name"]},
                {"categories", {
                    {"cache-ttl", jsonResponse["platforms"][0]["categories"]["cache-ttl"]},
                    {"category-list", {
                        {
                            {"crawler", jsonResponse["platforms"][0]["categories"]["category-list"][0]["crawler"]}
                        },
                        {
                            {"name", jsonResponse["platforms"][0]["categories"]["category-list"][0]["name"]}
                        }
                    }}
                }}
            },
            jsonResponse["platforms"][1]
        }}
    });
    config.setSourceUrl(sourceUrl);
    EXPECT_EQ(std::vector({categoryCrawlerConfigs[2]}), config.getCategoryCrawlerConfigs());
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
    mockClientError();
    config.setSourceUrl(sourceUrl);
    EXPECT_THROW(GetParam().callGetter(config), serio::ConfigFetchError);
}

TEST_P(ConfigFetchTest, shouldGetConfig) {
    mockClientResponse(jsonResponse);
    config.setSourceUrl(sourceUrl);
    GetParam().compareResults(config);
}

const FetchAsserts httpClientConfigAsserts{
    [] (serio::Config& config) { config.getHttpClientConfig(); },
    [] (serio::Config& config) { EXPECT_EQ(ConfigTest::httpClientConfig, config.getHttpClientConfig()); }
};
const FetchAsserts tvShowCrawlerConfigAsserts{
    [] (serio::Config& config) { config.getTvShowCrawlerConfigs(); },
    [] (serio::Config& config) { EXPECT_EQ(ConfigTest::tvShowCrawlerConfigs, config.getTvShowCrawlerConfigs()); }
};
const FetchAsserts searchCrawlerConfigAsserts{
    [] (serio::Config& config) { config.getSearchCrawlerConfigs(); },
    [] (serio::Config& config) { EXPECT_EQ(ConfigTest::searchCrawlerConfigs, config.getSearchCrawlerConfigs()); }
};
const FetchAsserts categoryCrawlerConfigAsserts{
    [] (serio::Config& config) { config.getCategoryCrawlerConfigs(); },
    [] (serio::Config& config) { EXPECT_EQ(ConfigTest::categoryCrawlerConfigs, config.getCategoryCrawlerConfigs()); }
};
INSTANTIATE_TEST_SUITE_P(ConfigFetchTestInstantiation, ConfigFetchTest, ::testing::Values(
        httpClientConfigAsserts, tvShowCrawlerConfigAsserts, searchCrawlerConfigAsserts, categoryCrawlerConfigAsserts));
