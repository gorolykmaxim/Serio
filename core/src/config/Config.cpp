#include <config/Config.h>

namespace serio {
Config::Config(SQLite::Database &database, HttpClient& httpClient)
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

std::vector<TvShowCrawlerConfig> Config::getTvShowCrawlerConfigs() {
    const auto config = source.fetchConfig();
    std::vector<TvShowCrawlerConfig> tvShowCrawlerConfigs;
    const auto platforms = config.getList({"platforms"});
    tvShowCrawlerConfigs.reserve(platforms.size());
    for (const auto& platform: platforms) {
        const auto name = platform.getParameter({"name"});
        const auto cacheTtl = platform.getParameter({"tv-show", "cache-ttl"});
        const auto crawler = platform.getParameter({"tv-show", "crawler"});
        if (name && cacheTtl && crawler) {
            TvShowCrawlerConfig tvShowCrawlerConfig{
                    name->get<std::string>(),
                    std::chrono::milliseconds(cacheTtl->get<long>()),
                    crawler->get<std::string>()
            };
            tvShowCrawlerConfigs.push_back(std::move(tvShowCrawlerConfig));
        }
    }
    return tvShowCrawlerConfigs;
}

std::vector<SearchCrawlerConfig> Config::getSearchCrawlerConfigs() {
    const auto config = source.fetchConfig();
    std::vector<SearchCrawlerConfig> searchCrawlerConfigs;
    const auto platforms = config.getList({"platforms"});
    searchCrawlerConfigs.reserve(platforms.size());
    for (const auto& platform: platforms) {
        const auto name = platform.getParameter({"name"});
        const auto cacheTtl = platform.getParameter({"search", "cache-ttl"});
        const auto crawler = platform.getParameter({"search", "crawler"});
        if (name && cacheTtl && crawler) {
            SearchCrawlerConfig searchCrawlerConfig{
                    name->get<std::string>(),
                    std::chrono::milliseconds(cacheTtl->get<long>()),
                    crawler->get<std::string>()
            };
            searchCrawlerConfigs.push_back(std::move(searchCrawlerConfig));
        }
    }
    return searchCrawlerConfigs;
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

std::vector<SuggestionsCrawlerConfig> Config::getSuggestionsCrawlerConfigs() {
    const auto config = source.fetchConfig();
    std::vector<SuggestionsCrawlerConfig> suggestionsCrawlerConfigs;
    const auto platforms = config.getList({"platforms"});
    suggestionsCrawlerConfigs.reserve(platforms.size());
    for (const auto& platform: platforms) {
        const auto name = platform.getParameter({"name"});
        const auto cacheTtl = platform.getParameter({"suggestions", "cache-ttl"});
        const auto crawler = platform.getParameter({"suggestions", "crawler"});
        if (name && cacheTtl && crawler) {
            SuggestionsCrawlerConfig suggestionsCrawlerConfig{
                name->get<std::string>(),
                std::chrono::milliseconds(cacheTtl->get<long>()),
                crawler->get<std::string>()
            };
            suggestionsCrawlerConfigs.push_back(std::move(suggestionsCrawlerConfig));
        }
    }
    return suggestionsCrawlerConfigs;
}
}
