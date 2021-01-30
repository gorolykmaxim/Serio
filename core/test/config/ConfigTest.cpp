#include <gtest/gtest.h>
#include <SQLiteCpp/Database.h>
#include <NFHTTP/ResponseImplementation.h>
#include <HttpClientMock.h>
#include <config/Config.h>
#include <nlohmann/json.hpp>

class ConfigTest : public ::testing::Test {
protected:
    const serio::HttpClientConfig httpClientConfig{{"user-agent 1"}};
    const std::vector<serio::TvShowCrawlerConfig> tvShowCrawlerConfigs = std::vector({
        serio::TvShowCrawlerConfig{"Platform 1",
                                   std::chrono::milliseconds(180000),
                                   "tv show crawler code"},
        serio::TvShowCrawlerConfig{"Platform 2",
                                   std::chrono::milliseconds(380000),
                                   "tv show crawler code 2"}
    });
    const std::vector<serio::SearchCrawlerConfig> searchCrawlerConfigs = std::vector({
        serio::SearchCrawlerConfig{tvShowCrawlerConfigs[0].platformName,
                                   std::chrono::milliseconds(190000),
                                   3,
                                   "search tv show crawler"},
        serio::SearchCrawlerConfig{tvShowCrawlerConfigs[1].platformName,
                                   std::chrono::milliseconds(200000),
                                   1,
                                   "search tv show crawler 2"},
    });
    const std::vector<serio::CategoryCrawlerConfig> categoryCrawlerConfigs = std::vector({
        serio::CategoryCrawlerConfig{tvShowCrawlerConfigs[0].platformName,
                                     "category 1",
                                     std::chrono::milliseconds(150000),
                                     3,
                                     "category tv show crawler"},
        serio::CategoryCrawlerConfig{tvShowCrawlerConfigs[0].platformName,
                                     "category 2",
                                     std::chrono::milliseconds(150000),
                                     1,
                                     "category tv show crawler 2"},
        serio::CategoryCrawlerConfig{tvShowCrawlerConfigs[1].platformName,
                                     "category 1",
                                     std::chrono::milliseconds(100000),
                                     2,
                                     "category tv show crawler 3"}
    });
    const std::vector<serio::SuggestionsCrawlerConfig> suggestionsCrawlerConfigs = std::vector({
        serio::SuggestionsCrawlerConfig{tvShowCrawlerConfigs[0].platformName,
                                        std::chrono::milliseconds(300000),
                                        "suggestions crawler 1"},
        serio::SuggestionsCrawlerConfig{tvShowCrawlerConfigs[1].platformName,
                                        std::chrono::milliseconds(301000),
                                        "suggestions crawler 2"},
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
                                {"pages-per-load", categoryCrawlerConfigs[0].pagesPerLoad},
                                {"crawler", categoryCrawlerConfigs[0].crawler}
                            },
                            {
                                {"name", categoryCrawlerConfigs[1].categoryName},
                                {"crawler", categoryCrawlerConfigs[1].crawler}
                            }
                        }}
                    }},
                    {"suggestions", {
                        {"cache-ttl", suggestionsCrawlerConfigs[0].cacheTtl.count()},
                        {"crawler", suggestionsCrawlerConfigs[0].crawler}
                    }},
                    {"search", {
                       {"cache-ttl", searchCrawlerConfigs[0].cacheTtl.count()},
                       {"pages-per-load", searchCrawlerConfigs[0].pagesPerLoad},
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
                                {"pages-per-load", categoryCrawlerConfigs[2].pagesPerLoad},
                                {"crawler", categoryCrawlerConfigs[2].crawler}
                            }
                        }}
                    }},
                    {"suggestions", {
                        {"cache-ttl", suggestionsCrawlerConfigs[1].cacheTtl.count()},
                        {"crawler", suggestionsCrawlerConfigs[1].crawler}
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
    mocks::HttpClientMock httpClient;
    serio::Config config = serio::Config(database, httpClient);

    void mockClientResponse(const nlohmann::json& response) {
        const auto rawResponse = response.dump();
        const std::chrono::milliseconds cacheTtl = serio::ConfigSource::CACHE_TTL;
        const serio::HttpResponse httpResponse(rawResponse);
        EXPECT_CALL(httpClient, sendRequest(request, cacheTtl)).WillOnce(::testing::Return(httpResponse));
    }
    void mockClientError() {
        const std::chrono::milliseconds cacheTtl = serio::ConfigSource::CACHE_TTL;
        const serio::HttpResponse httpResponse([] () -> std::string { throw std::runtime_error(""); });
        EXPECT_CALL(httpClient, sendRequest(request, cacheTtl)).WillOnce(::testing::Return(httpResponse));
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

TEST_F(ConfigTest, shouldFailToGetHttpClientConfigSinceSourceUrlIsNotSet) {
    EXPECT_THROW(config.getHttpClientConfig(), serio::ConfigFetchError);
}

TEST_F(ConfigTest, shouldFailToGetHttpClientConfigDueToHttpError) {
    mockClientError();
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

TEST_F(ConfigTest, shouldFailToGetTvShowCrawlerConfigSinceSourceUrlIsNotSet) {
    EXPECT_THROW(config.getTvShowCrawlerConfigs(), serio::ConfigFetchError);
}

TEST_F(ConfigTest, shouldFailToGetTvShowCrawlerConfigDueToHttpError) {
    mockClientError();
    config.setSourceUrl(sourceUrl);
    EXPECT_THROW(config.getTvShowCrawlerConfigs(), serio::ConfigFetchError);
}

TEST_F(ConfigTest, shouldGetTvShowCrawlerConfig) {
    mockClientResponse(jsonResponse);
    config.setSourceUrl(sourceUrl);
    EXPECT_EQ(tvShowCrawlerConfigs, config.getTvShowCrawlerConfigs());
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

TEST_F(ConfigTest, shouldFailToGetSearchCrawlerConfigSinceSourceUrlIsNotSet) {
    EXPECT_THROW(config.getSearchCrawlerConfigs(), serio::ConfigFetchError);
}

TEST_F(ConfigTest, shouldFailToGetSearchCrawlerConfigDueToHttpError) {
    mockClientError();
    config.setSourceUrl(sourceUrl);
    EXPECT_THROW(config.getSearchCrawlerConfigs(), serio::ConfigFetchError);
}

TEST_F(ConfigTest, shouldGetSearchCrawlerConfig) {
    mockClientResponse(jsonResponse);
    config.setSourceUrl(sourceUrl);
    EXPECT_EQ(searchCrawlerConfigs, config.getSearchCrawlerConfigs());
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

TEST_F(ConfigTest, shouldFailToGetCategoryCrawlerConfigSinceSourceUrlIsNotSet) {
    EXPECT_THROW(config.getCategoryCrawlerConfigs(), serio::ConfigFetchError);
}

TEST_F(ConfigTest, shouldFailToGetCategoryCrawlerConfigDueToHttpError) {
    mockClientError();
    config.setSourceUrl(sourceUrl);
    EXPECT_THROW(config.getCategoryCrawlerConfigs(), serio::ConfigFetchError);
}

TEST_F(ConfigTest, shouldGetCategoryCrawlerConfig) {
    mockClientResponse(jsonResponse);
    config.setSourceUrl(sourceUrl);
    EXPECT_EQ(categoryCrawlerConfigs, config.getCategoryCrawlerConfigs());
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

TEST_F(ConfigTest, shouldFailToGetSuggestionsCrawlerConfigSinceSourceUrlIsNotSet) {
    EXPECT_THROW(config.getSuggestionsCrawlerConfigs(), serio::ConfigFetchError);
}

TEST_F(ConfigTest, shouldFailToGetSuggestionsCrawlerConfigDueToHttpError) {
    mockClientError();
    config.setSourceUrl(sourceUrl);
    EXPECT_THROW(config.getSuggestionsCrawlerConfigs(), serio::ConfigFetchError);
}

TEST_F(ConfigTest, shouldGetSuggestionsCrawlerConfig) {
    mockClientResponse(jsonResponse);
    config.setSourceUrl(sourceUrl);
    EXPECT_EQ(suggestionsCrawlerConfigs, config.getSuggestionsCrawlerConfigs());
}

TEST_F(ConfigTest, shouldGetEmptyVectorOfSuggestionsCrawlerConfigsSinceNoPlatformsAreConfigured) {
    mockClientResponse({});
    config.setSourceUrl(sourceUrl);
    EXPECT_TRUE(config.getSuggestionsCrawlerConfigs().empty());
}

TEST_F(ConfigTest, shouldSkipPlatformSuggestionsCrawlerConfigsThatLackFields) {
    mockClientResponse({{"platforms", {jsonResponse["platforms"][0], {}}}});
    config.setSourceUrl(sourceUrl);
    EXPECT_EQ(std::vector({suggestionsCrawlerConfigs[0]}), config.getSuggestionsCrawlerConfigs());
}