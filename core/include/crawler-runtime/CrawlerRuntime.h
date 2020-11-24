#ifndef SERIO_CRAWLERRUNTIME_H
#define SERIO_CRAWLERRUNTIME_H

#include <string>
#include <chrono>
#include <nlohmann/json.hpp>
#include <crawler-http-client/CrawlerHttpClient.h>
#include "CrawlerExecution.h"

namespace serio {
struct Crawler {
    std::string code;
    std::chrono::milliseconds networkCacheTtl;
    nlohmann::json arguments;
};

class CrawlerRuntime {
public:
    explicit CrawlerRuntime(CrawlerHttpClient& httpClient);
    std::vector<nlohmann::json> executeCrawlers(const std::vector<Crawler>& crawlers);
private:
    void initializeCrawlerExecutions(std::vector<CrawlerExecution>& executions, const std::vector<Crawler>& crawlers);
    void performExecutions(std::vector<CrawlerExecution>& executions);
    std::vector<nlohmann::json> fetchExecutionResults(std::vector<CrawlerExecution> &executions);
};

class InvalidCrawlerError : std::logic_error {
public:
    InvalidCrawlerError(const Crawler& crawler, const std::string &reason);
};
}

#endif //SERIO_CRAWLERRUNTIME_H
