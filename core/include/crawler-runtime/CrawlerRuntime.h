#ifndef SERIO_CRAWLERRUNTIME_H
#define SERIO_CRAWLERRUNTIME_H

#include <string>
#include <chrono>
#include <nlohmann/json.hpp>
#include <http-client/HttpClient.h>
#include <config/Config.h>

namespace serio {
struct Crawler {
    std::string code;
    std::chrono::milliseconds networkCacheTtl;
    nlohmann::json arguments;
};

class CrawlerRuntime {
public:
    explicit CrawlerRuntime(HttpClient& httpClient, Config& config, bool trace = false);
    std::vector<nlohmann::json> executeCrawlers(std::vector<Crawler> crawlers);
private:
    HttpClient& httpClient;
    Config& config;
    bool trace;
};

class InvalidCrawlerError : public std::logic_error {
public:
    InvalidCrawlerError(const Crawler& crawler, const std::string &reason);
};
}

#endif //SERIO_CRAWLERRUNTIME_H
