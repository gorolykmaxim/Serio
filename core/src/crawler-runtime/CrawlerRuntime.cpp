#include <crawler-runtime/CrawlerRuntime.h>
#include "CrawlerExecution.h"
#include "RegExpSystem.h"
#include "ResultFetchSystem.h"
#include "CrawlerExecutionSystem.h"
#include "CrawlerArgumentSystem.h"
#include "HttpRequestSystem.h"
#include "LoggingSystem.h"

namespace serio {
CrawlerRuntime::CrawlerRuntime(CrawlerHttpClient &httpClient, bool enableLogging)
    : httpClient(httpClient), enableLogging(enableLogging) {}

std::vector<nlohmann::json> CrawlerRuntime::executeCrawlers(std::vector<Crawler> crawlers) {
    std::vector<serio::CrawlerExecution> executions;
    RegExpSystem regExpSystem(crawlers, executions);
    HttpRequestSystem httpRequestSystem(crawlers, executions, httpClient);
    ResultFetchSystem resultFetchSystem(crawlers, executions);
    CrawlerExecutionSystem crawlerExecutionSystem(crawlers, executions);
    CrawlerArgumentSystem crawlerArgumentSystem(crawlers, executions);
    LoggingSystem loggingSystem(executions);
    while (!crawlerExecutionSystem.isFinished()) {
        crawlerExecutionSystem.update();
        if (enableLogging) {
            loggingSystem.update();
        }
        regExpSystem.update();
        httpRequestSystem.update();
    }
    return resultFetchSystem.fetch();
}
}