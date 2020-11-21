#ifndef SERIO_CONFIG_H
#define SERIO_CONFIG_H

#include <SQLiteCpp/Database.h>
#include <cache/Cache.h>
#include <NFHTTP/Client.h>
#include "HttpClientConfig.h"
#include "ConfigSource.h"
#include "ConfigStructure.h"
#include "EpisodeCrawlerConfig.h"
#include "SearchCrawlerConfig.h"
#include "CategoryCrawlerConfig.h"
#include <nlohmann/json.hpp>

namespace serio {
class Config {
public:
    Config(SQLite::Database &database, CachingHttpClient& httpClient);
    virtual void setSourceUrl(const std::string& url);
    virtual std::optional<std::string> getSourceUrl();
    virtual HttpClientConfig getHttpClientConfig();
    virtual std::vector<EpisodeCrawlerConfig> getEpisodeCrawlerConfigs();
    virtual std::vector<SearchCrawlerConfig> getSearchCrawlerConfigs();
    virtual std::vector<CategoryCrawlerConfig> getCategoryCrawlerConfigs();
private:
    ConfigSource source;

    template<typename T>
    std::vector<T> getPlatformConfigs(const std::vector<std::vector<std::string>>& fields,
                                      std::function<T(const std::vector<nlohmann::json>&)> factoryMethod) {
        const auto config = source.fetchConfig();
        std::vector<T> configs;
        const auto platforms = config.getList({"platforms"});
        configs.reserve(platforms.size());
        for (const auto& platform: platforms) {
            std::vector<nlohmann::json> values;
            values.reserve(fields.size());
            for (const auto& field: fields) {
                const auto value = platform.getParameter(field);
                if (value) {
                    values.push_back(*value);
                }
            }
            if (values.size() < fields.size()) {
                continue;
            }
            configs.push_back(std::move(factoryMethod(values)));
        }
        return configs;
    }
};
}

#endif //SERIO_CONFIG_H
