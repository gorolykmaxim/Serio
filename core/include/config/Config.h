#ifndef SERIO_CONFIG_H
#define SERIO_CONFIG_H

#include <SQLiteCpp/Database.h>
#include <cache/Cache.h>
#include <NFHTTP/Client.h>
#include "HttpClientConfig.h"
#include "ConfigSource.h"
#include "ConfigStructure.h"
#include "EpisodeCrawlerConfig.h"
#include <nlohmann/json.hpp>

namespace serio {
class Config {
public:
    Config(SQLite::Database &database, CachingHttpClient& httpClient);
    virtual void setSourceUrl(const std::string& url);
    virtual std::optional<std::string> getSourceUrl();
    virtual HttpClientConfig getHttpClientConfig();
    virtual std::vector<EpisodeCrawlerConfig> getEpisodeCrawlerConfigs();
private:
    ConfigSource source;
};
}

#endif //SERIO_CONFIG_H
