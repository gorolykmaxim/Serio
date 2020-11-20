#ifndef SERIO_CONFIG_H
#define SERIO_CONFIG_H

#include <SQLiteCpp/Database.h>
#include <cache/Cache.h>
#include <NFHTTP/Client.h>
#include "HttpClientConfig.h"
#include "ConfigSource.h"
#include "ConfigStructure.h"
#include <nlohmann/json.hpp>

namespace serio {
class Config {
public:
    Config(SQLite::Database &database, Cache &cache, nativeformat::http::Client &httpClient);
    virtual void setSourceUrl(const std::string& url);
    virtual std::optional<std::string> getSourceUrl();
    virtual HttpClientConfig getHttpClientConfig();
private:
    ConfigSource source;
};
}

#endif //SERIO_CONFIG_H
