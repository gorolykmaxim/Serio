#ifndef SERIO_CONFIG_H
#define SERIO_CONFIG_H

#include <SQLiteCpp/Database.h>
#include <cache/Cache.h>
#include <NFHTTP/Client.h>

namespace serio {
struct HttpClientConfig {
    std::vector<std::string> userAgents;
};

class Config {
public:
    Config(SQLite::Database &database, Cache &cache, nativeformat::http::Client &httpClient);
    void setSourceUrl(const std::string& url);
    std::optional<std::string> getSourceUrl();
private:
    const std::string SOURCE_URL_PARAM = "source-url";
    SQLite::Database& database;
    Cache& cache;
    nativeformat::http::Client& httpClient;
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

#endif //SERIO_CONFIG_H
