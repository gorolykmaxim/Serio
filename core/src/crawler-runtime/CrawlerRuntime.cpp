#include <crawler-runtime/CrawlerRuntime.h>
#include <set>
#include "CrawlerExecution.h"
#include "RegExpSystem.h"
#include <sstream>

namespace serio {
CrawlerRuntime::CrawlerRuntime(CrawlerHttpClient &httpClient) {}

static void initializeCrawlerExecutions(std::vector<CrawlerExecution> &executions, const std::vector<Crawler> &crawlers,
                                        const RegExpSystem& regExpSystem) {
    executions.reserve(crawlers.size());
    for (const auto& crawler: crawlers) {
        try {
            std::stringstream code;
            code << regExpSystem.getCode() << " " << crawler.code;
            executions.emplace_back(code.str(), crawler.arguments);
        } catch (std::logic_error& e) {
            throw InvalidCrawlerError(crawler, e.what());
        }
    }
}

static void performExecutions(std::vector<CrawlerExecution> &executions, const RegExpSystem& regExpSystem) {
    std::set<CrawlerExecution*> finishedExecutions;
    while (finishedExecutions.size() < executions.size()) {
        for (auto& execution: executions) {
            if (execution.isDone()) {
                finishedExecutions.emplace(&execution);
            } else {
                execution.executeStep();
                regExpSystem.update(execution);
            }
        }
    };
}

static std::vector<nlohmann::json> fetchExecutionResults(std::vector<CrawlerExecution> &executions) {
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

std::vector<nlohmann::json> CrawlerRuntime::executeCrawlers(const std::vector<Crawler>& crawlers) {
    std::vector<serio::CrawlerExecution> executions;
    RegExpSystem regExpSystem;
    initializeCrawlerExecutions(executions, crawlers, regExpSystem);
    performExecutions(executions, regExpSystem);
    return fetchExecutionResults(executions);
}


InvalidCrawlerError::InvalidCrawlerError(const Crawler& crawler, const std::string &reason)
    : std::logic_error("Crawler with code '" + crawler.code + "' and arguments '" + crawler.arguments.dump() + "' is invalid: " + reason) {}
}