#ifndef SERIO_CONFIG_H
#define SERIO_CONFIG_H

#include <SQLiteCpp/Database.h>
#include <cache/Cache.h>
#include <NFHTTP/Client.h>
#include "HttpClientConfig.h"
#include "ConfigSource.h"
#include "ConfigStructure.h"
#include "TvShowCrawlerConfig.h"
#include "SearchCrawlerConfig.h"
#include "CategoryCrawlerConfig.h"
#include "SuggestionsCrawlerConfig.h"
#include <nlohmann/json.hpp>

namespace serio {
class Config {
public:
    Config(SQLite::Database &database, HttpClient& httpClient);
    virtual void setSourceUrl(const std::string& url);
    virtual std::optional<std::string> getSourceUrl();
    virtual void setProperty(const std::string& name, const std::string& value);
    virtual std::optional<std::string> getProperty(const std::string& name);
    virtual HttpClientConfig getHttpClientConfig();
    virtual std::vector<TvShowCrawlerConfig> getTvShowCrawlerConfigs();
    virtual std::vector<SearchCrawlerConfig> getSearchCrawlerConfigs();
    virtual std::vector<CategoryCrawlerConfig> getCategoryCrawlerConfigs();
    virtual std::vector<SuggestionsCrawlerConfig> getSuggestionsCrawlerConfigs();
private:
    PersistentProperties properties;
    ConfigSource source;
};
}

#endif //SERIO_CONFIG_H
