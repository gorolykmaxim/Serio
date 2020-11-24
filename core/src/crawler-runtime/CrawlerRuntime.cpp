#include <crawler-runtime/CrawlerRuntime.h>
#include "component/ExecutionComponent.h"
#include "system/ArgumentSystem.h"
#include "system/ExecutionSystem.h"
#include "system/ResultFetchingSystem.h"

namespace serio {
CrawlerRuntime::CrawlerRuntime(CrawlerHttpClient &httpClient) {}

std::vector<nlohmann::json> CrawlerRuntime::executeCrawlers(const std::vector<Crawler>& crawlers) {
    std::vector<ExecutionComponent> executions(crawlers.size());
    ArgumentSystem argumentSystem(crawlers, executions);
    ExecutionSystem executionSystem(crawlers, executions);
    ResultFetchingSystem resultFetchingSystem(crawlers, executions);
    while (!executionSystem.isExecutionFinished()) {
        executionSystem.update();
    }
    return resultFetchingSystem.fetchResults();
}
}