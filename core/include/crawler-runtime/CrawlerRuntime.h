#ifndef SERIO_CRAWLERRUNTIME_H
#define SERIO_CRAWLERRUNTIME_H

#include <string>
#include <chrono>
#include <nlohmann/json.hpp>
#include <crawler-http-client/CrawlerHttpClient.h>

namespace serio {
struct Crawler {
    std::string code;
    std::chrono::milliseconds networkCacheTtl;
    nlohmann::json arguments;
};

class CrawlerRuntime {
public:
    explicit CrawlerRuntime(CrawlerHttpClient& httpClient, bool enableLogging = false);
    std::vector<nlohmann::json> executeCrawlers(std::vector<Crawler> crawlers);
private:
    CrawlerHttpClient& httpClient;
    bool enableLogging;
};
}

#endif //SERIO_CRAWLERRUNTIME_H
