#include <crawler-runtime/CrawlerRuntime.h>
#include <set>

namespace serio {
CrawlerRuntime::CrawlerRuntime(CrawlerHttpClient &httpClient) {}

std::vector<nlohmann::json> CrawlerRuntime::executeCrawlers(const std::vector<Crawler>& crawlers) {
    std::vector<serio::CrawlerExecution> executions;
    initializeCrawlerExecutions(executions, crawlers);
    performExecutions(executions);
    return fetchExecutionResults(executions);
}

void CrawlerRuntime::initializeCrawlerExecutions(std::vector<CrawlerExecution> &executions,
                                                 const std::vector<Crawler> &crawlers) {
    executions.reserve(crawlers.size());
    for (const auto& crawler: crawlers) {
        executions.emplace_back(crawler.code);
    }
}

void CrawlerRuntime::performExecutions(std::vector<CrawlerExecution> &executions) {
    std::set<CrawlerExecution*> finishedExecutions;
    while (finishedExecutions.size() < executions.size()) {
        for (auto& execution: executions) {
            if (execution.isDone()) {
                finishedExecutions.emplace(&execution);
            } else {
                execution.executeStep();
            }
        }
    };
}

std::vector<nlohmann::json> CrawlerRuntime::fetchExecutionResults(std::vector<CrawlerExecution> &executions) {
    std::vector<nlohmann::json> results;
    results.reserve(executions.size());
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