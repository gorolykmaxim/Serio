#include <crawler-runtime/CrawlerRuntime.h>
#include "CrawlerExecution.h"
#include "RegExpSystem.h"
#include "ResultFetchSystem.h"
#include "CrawlerExecutionSystem.h"
#include "CrawlerArgumentSystem.h"
#include "HttpRequestSystem.h"

namespace serio {
CrawlerRuntime::CrawlerRuntime(CrawlerHttpClient &httpClient) : httpClient(httpClient) {}

std::vector<nlohmann::json> CrawlerRuntime::executeCrawlers(std::vector<Crawler> crawlers) {
    std::vector<serio::CrawlerExecution> executions;
    RegExpSystem regExpSystem(crawlers, executions);
    HttpRequestSystem httpRequestSystem(crawlers, executions, httpClient);
    ResultFetchSystem resultFetchSystem(crawlers, executions);
    CrawlerExecutionSystem crawlerExecutionSystem(crawlers, executions);
    CrawlerArgumentSystem crawlerArgumentSystem(crawlers, executions);
    while (!crawlerExecutionSystem.isFinished()) {
        crawlerExecutionSystem.update();
        regExpSystem.update();
        httpRequestSystem.update();
    }
    return resultFetchSystem.fetch();
}
}