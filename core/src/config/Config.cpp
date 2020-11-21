#include <config/Config.h>

namespace serio {
Config::Config(SQLite::Database &database, CachingHttpClient& httpClient)
    : source(database, httpClient) {}

void Config::setSourceUrl(const std::string &url) {
    source.setUrl(url);
}

std::optional<std::string> Config::getSourceUrl() {
    return source.getUrl();
}

HttpClientConfig Config::getHttpClientConfig() {
    const auto config = source.fetchConfig();
    HttpClientConfig httpClientConfig;
    const auto userAgents = config.getParameter({"http-client", "user-agents"});
    if (userAgents) {
        httpClientConfig.userAgents = userAgents->get<std::vector<std::string>>();
    }
    return httpClientConfig;
}

std::vector<EpisodeCrawlerConfig> Config::getEpisodeCrawlerConfigs() {
    const std::vector<std::vector<std::string>> fields({
        {"name"},
        {"episode", "cache-ttl"},
        {"episode", "tvShowCrawler"},
        {"episode", "episodeCrawler"}
    });
    return getPlatformConfigs<EpisodeCrawlerConfig>(fields, [](const std::vector<nlohmann::json> &values) {
        return EpisodeCrawlerConfig{
            values[0].get<std::string>(),
            std::chrono::milliseconds(values[1].get<long>()),
            values[2].get<std::string>(),
            values[3].get<std::string>()
        };
    });
}

std::vector<SearchCrawlerConfig> Config::getSearchCrawlerConfigs() {
    const std::vector<std::vector<std::string>> fields({
        {"name"},
        {"search", "cache-ttl"},
        {"search", "crawler"}
    });
    return getPlatformConfigs<SearchCrawlerConfig>(fields, [](const std::vector<nlohmann::json> &values) {
        return SearchCrawlerConfig{
            values[0].get<std::string>(),
            std::chrono::milliseconds(values[1].get<long>()),
            values[2].get<std::string>()
        };
    });
}

std::vector<CategoryCrawlerConfig> Config::getCategoryCrawlerConfigs() {
    const auto config = source.fetchConfig();
    std::vector<CategoryCrawlerConfig> categoryCrawlerConfigs;
    const auto platforms = config.getList({"platforms"});
    for (const auto& platform: platforms) {
        const auto categoryList = platform.getList({"categories", "category-list"});
        for (const auto& category: categoryList) {
            const auto platformName = platform.getParameter({"name"});
            const auto categoryName = category.getParameter({"name"});
            const auto cacheTtl = platform.getParameter({"categories", "cache-ttl"});
            const auto crawler = category.getParameter({"crawler"});
            if (platformName && cacheTtl && categoryName && crawler) {
                CategoryCrawlerConfig categoryCrawlerConfig{
                    platformName->get<std::string>(),
                    categoryName->get<std::string>(),
                    std::chrono::milliseconds(cacheTtl->get<long>()),
                    crawler->get<std::string>()
                };
                categoryCrawlerConfigs.push_back(std::move(categoryCrawlerConfig));
            }
        }
    }
    return categoryCrawlerConfigs;
}
}
