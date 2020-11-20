#include <config/Config.h>

namespace serio {
Config::Config(SQLite::Database &database, Cache &cache, nativeformat::http::Client &httpClient)
    : database(database),
      cache(cache),
      httpClient(httpClient) {
    database.exec("CREATE TABLE IF NOT EXISTS CONFIG(KEY TEXT PRIMARY KEY, VALUE TEXT)");
}

void Config::setSourceUrl(const std::string &url) {
    SQLite::Statement saveSourceUrl(database, "INSERT OR REPLACE INTO CONFIG VALUES(?, ?)");
    saveSourceUrl.bind(1, SOURCE_URL_PARAM);
    saveSourceUrl.bind(2, url);
    saveSourceUrl.exec();
}

std::optional<std::string> Config::getSourceUrl() {
    SQLite::Statement getSourceUrl(database, "SELECT VALUE FROM CONFIG WHERE KEY = ?");
    getSourceUrl.bind(1, SOURCE_URL_PARAM);
    if (getSourceUrl.executeStep()) {
        return getSourceUrl.getColumn(0);
    } else {
        return std::optional<std::string>();
    }
}

ConfigFetchError::ConfigFetchError(const std::string &reason) : std::runtime_error(reason) {}

ConfigSourceNotSpecifiedError::ConfigSourceNotSpecifiedError()
    : std::runtime_error("Source URL to fetch the configuration from has not been specified") {}
}
