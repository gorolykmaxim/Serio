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
    const auto config = source.fetchConfig();
    std::vector<EpisodeCrawlerConfig> episodeCrawlerConfigs;
    const auto platforms = config.getPlatforms();
    episodeCrawlerConfigs.reserve(platforms.size());
    for (const auto& platform: platforms) {
        const auto name = platform.getParameter({"name"});
        const auto cacheTtl = platform.getParameter({"episode", "cache-ttl"});
        const auto tvShowCrawler = platform.getParameter({"episode", "tvShowCrawler"});
        const auto episodeCrawler = platform.getParameter({"episode", "episodeCrawler"});
        if (name && cacheTtl && tvShowCrawler && episodeCrawler) {
            EpisodeCrawlerConfig episodeCrawlerConfig{
                    name->get<std::string>(),
                    std::chrono::milliseconds(cacheTtl->get<long>()),
                    tvShowCrawler->get<std::string>(),
                    episodeCrawler->get<std::string>()
            };
            episodeCrawlerConfigs.push_back(std::move(episodeCrawlerConfig));
        }
    }
    return episodeCrawlerConfigs;
}

std::vector<SearchCrawlerConfig> Config::getSearchCrawlerConfigs() {
    const auto config = source.fetchConfig();
    std::vector<SearchCrawlerConfig> searchCrawlerConfigs;
    const auto platforms = config.getPlatforms();
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
}
