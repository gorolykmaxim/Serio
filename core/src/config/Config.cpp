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
    return getConfigs<EpisodeCrawlerConfig>(fields, [] (const std::vector<nlohmann::json>& values) {
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
    return getConfigs<SearchCrawlerConfig>(fields, [] (const std::vector<nlohmann::json>& values) {
        return SearchCrawlerConfig{
            values[0].get<std::string>(),
            std::chrono::milliseconds(values[1].get<long>()),
            values[2].get<std::string>()
        };
    });
}
}
