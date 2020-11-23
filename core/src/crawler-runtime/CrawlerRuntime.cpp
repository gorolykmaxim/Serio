#include <crawler-runtime/CrawlerRuntime.h>
#include <set>

namespace serio {
CrawlerRuntime::CrawlerRuntime(CrawlerHttpClient &httpClient) {}

std::vector<nlohmann::json> CrawlerRuntime::executeCrawlers(const std::vector<Crawler>& crawlers) {
    std::vector<serio::CrawlerExecution> executions;
    initializeCrawlerExecutions(executions, crawlers);
    performExecutions(executions);
    return fetchExecutionResults(executions, crawlers.size());
}

void CrawlerRuntime::initializeCrawlerExecutions(std::vector<CrawlerExecution> &executions,
                                                 const std::vector<Crawler> &crawlers) {
    executions.reserve(crawlers.size());
    for (const auto& crawler: crawlers) {
        executions.emplace_back(crawler.code);
    }
}

void CrawlerRuntime::performExecutions(std::vector<CrawlerExecution> &executions) {
    std::set<void*> finishedExecutions;
    do {
        for (auto& execution: executions) {
            execution.executeStep();
            if (execution.isDone()) {
                finishedExecutions.emplace(&execution);
            }
        }
    } while (finishedExecutions.size() < executions.size());
}

std::vector<nlohmann::json> CrawlerRuntime::fetchExecutionResults(std::vector<CrawlerExecution> &executions,
                                                                  uint32_t estimatedSize) {
    std::vector<nlohmann::json> results;
    results.reserve(estimatedSize);
    for (auto& execution: executions) {
        if (execution.hasFailed()) {
            continue;
        }
        try {
            results.push_back(execution.getResult());
        } catch (std::exception& e) {
            continue;
        }
    }
    return results;
}
}