#include <crawler-runtime/CrawlerRuntime.h>
#include <set>
#include "CrawlerExecution.h"
#include "RegExpSystem.h"
#include "ResultFetchSystem.h"
#include "CrawlerExecutionSystem.h"
#include <sstream>

namespace serio {
CrawlerRuntime::CrawlerRuntime(CrawlerHttpClient &httpClient) {}

std::vector<nlohmann::json> CrawlerRuntime::executeCrawlers(std::vector<Crawler> crawlers) {
    std::vector<serio::CrawlerExecution> executions;
    RegExpSystem regExpSystem(crawlers, executions);
    ResultFetchSystem resultFetchSystem(crawlers, executions);
    CrawlerExecutionSystem crawlerExecutionSystem(crawlers, executions);
    while (!crawlerExecutionSystem.isFinished()) {
        crawlerExecutionSystem.update();
        regExpSystem.update();
    }
    return resultFetchSystem.fetch();
}


InvalidCrawlerError::InvalidCrawlerError(const Crawler& crawler, const std::string &reason)
    : std::logic_error("Crawler with code '" + crawler.code + "' and arguments '" + crawler.arguments.dump() + "' is invalid: " + reason) {}
}