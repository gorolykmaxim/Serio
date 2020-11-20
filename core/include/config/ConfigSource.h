#ifndef SERIO_CONFIGSOURCE_H
#define SERIO_CONFIGSOURCE_H

#include <SQLiteCpp/Database.h>
#include <cache/Cache.h>
#include <NFHTTP/Client.h>
#include "ConfigStructure.h"

namespace serio {
class ConfigSource {
public:
    inline static const std::string CACHE_ENTRY_NAME = "config";
    inline static const std::chrono::hours CACHE_TTL = std::chrono::hours(24);

    ConfigSource(SQLite::Database &database, Cache &cache, nativeformat::http::Client &httpClient);
    void setUrl(const std::string& url);
    std::optional<std::string> getUrl();
    ConfigStructure fetchConfig();
private:
    inline static const std::string SOURCE_URL_PARAM = "source-url";
    SQLite::Database& database;
    Cache& cache;
    nativeformat::http::Client& httpClient;

    std::string getUrlOrFail();
    std::string fetchConfigFrom(const std::string& url);
};

class ConfigSourceNotSpecifiedError : public std::runtime_error {
public:
    ConfigSourceNotSpecifiedError();
};

class ConfigFetchError : public std::runtime_error {
public:
    explicit ConfigFetchError(const std::string &reason);
};
}

#endif //SERIO_CONFIGSOURCE_H
