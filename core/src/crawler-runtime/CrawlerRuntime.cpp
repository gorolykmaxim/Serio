#include <crawler-runtime/CrawlerRuntime.h>
#include "CrawlerExecution.h"
#include "RegExpSystem.h"
#include "ResultFetchSystem.h"
#include "CrawlerExecutionSystem.h"
#include "CrawlerArgumentSystem.h"

namespace serio {
CrawlerRuntime::CrawlerRuntime(CrawlerHttpClient &httpClient) {}

std::vector<nlohmann::json> CrawlerRuntime::executeCrawlers(std::vector<Crawler> crawlers) {
    std::vector<serio::CrawlerExecution> executions;
    RegExpSystem regExpSystem(crawlers, executions);
    ResultFetchSystem resultFetchSystem(crawlers, executions);
    CrawlerExecutionSystem crawlerExecutionSystem(crawlers, executions);
    CrawlerArgumentSystem crawlerArgumentSystem(crawlers, executions);
    while (!crawlerExecutionSystem.isFinished()) {
        crawlerExecutionSystem.update();
        regExpSystem.update();
    }
    return resultFetchSystem.fetch();
}
}