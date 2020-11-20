#include <config/ConfigSource.h>

namespace serio {
serio::ConfigSource::ConfigSource(SQLite::Database &database, Cache &cache, nativeformat::http::Client &httpClient)
        : database(database),
          cache(cache),
          httpClient(httpClient) {
    database.exec("CREATE TABLE IF NOT EXISTS CONFIG(KEY TEXT PRIMARY KEY, VALUE TEXT)");
}

void ConfigSource::setUrl(const std::string &url) {
    SQLite::Statement saveSourceUrl(database, "INSERT OR REPLACE INTO CONFIG VALUES(?, ?)");
    saveSourceUrl.bind(1, SOURCE_URL_PARAM);
    saveSourceUrl.bind(2, url);
    saveSourceUrl.exec();
}

std::optional<std::string> ConfigSource::getUrl() {
    SQLite::Statement getSourceUrl(database, "SELECT VALUE FROM CONFIG WHERE KEY = ?");
    getSourceUrl.bind(1, SOURCE_URL_PARAM);
    if (getSourceUrl.executeStep()) {
        return getSourceUrl.getColumn(0).getString();
    } else {
        return std::optional<std::string>();
    }
}

ConfigStructure ConfigSource::fetchConfig() {
    try {
        const auto cachedResponse = cache.get(CACHE_ENTRY_NAME);
        if (cachedResponse) {
            return ConfigStructure(nlohmann::json::parse(*cachedResponse));
        } else {
            const auto rawResponse = fetchConfigFrom(getUrlOrFail());
            cache.put(CACHE_ENTRY_NAME, rawResponse, CACHE_TTL);
            return ConfigStructure(nlohmann::json::parse(rawResponse));
        }
    } catch (std::runtime_error& e) {
        throw ConfigFetchError(e.what());
    }
}

std::string ConfigSource::getUrlOrFail() {
    const auto url = getUrl();
    if (url) {
        return *url;
    } else {
        throw ConfigSourceNotSpecifiedError();
    }
}

std::string ConfigSource::fetchConfigFrom(const std::string &url) {
    const auto request = nativeformat::http::createRequest(getUrlOrFail(), {});
    const auto response = httpClient.performRequestSynchronously(request);
    size_t size;
    return reinterpret_cast<const char *>(response->data(size));
}

ConfigSourceNotSpecifiedError::ConfigSourceNotSpecifiedError()
    : std::runtime_error("Source URL to fetch the configuration from has not been specified") {}

ConfigFetchError::ConfigFetchError(const std::string &reason) : std::runtime_error(reason) {}
}
