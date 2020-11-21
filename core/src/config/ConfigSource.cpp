#include <config/ConfigSource.h>

namespace serio {
serio::ConfigSource::ConfigSource(SQLite::Database &database, CachingHttpClient& client)
        : database(database),
          client(client) {
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
        const auto url = getUrl();
        if (!url) {
            throw ConfigSourceNotSpecifiedError();
        }
        HttpRequest request{*url};
        const auto response = client.sendRequest(request, CACHE_TTL).get();
        const auto jsonRoot = nlohmann::json::parse(response);
        return ConfigStructure(jsonRoot);
    } catch (std::runtime_error& e) {
        throw ConfigFetchError(e.what());
    }
}

ConfigSourceNotSpecifiedError::ConfigSourceNotSpecifiedError()
    : std::runtime_error("Source URL to fetch the configuration from has not been specified") {}

ConfigFetchError::ConfigFetchError(const std::string &reason) : std::runtime_error(reason) {}
}
