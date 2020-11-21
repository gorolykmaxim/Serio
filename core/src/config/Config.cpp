#include <config/Config.h>

namespace serio {
Config::Config(SQLite::Database &database, Cache &cache, nativeformat::http::Client &httpClient)
    : source(database, cache, httpClient) {}

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
}