#include "ResultFetchSystem.h"

namespace serio {
ResultFetchSystem::ResultFetchSystem(std::vector<Crawler> &crawlers, std::vector<CrawlerExecution>& executions) : executions(executions) {
    for (auto& crawler: crawlers) {
        crawler.code += " JSON.stringify(crawl(args));";
    }
}

std::vector<nlohmann::json> ResultFetchSystem::fetch() {
    std::vector<nlohmann::json> results;
    results.reserve(executions.size());
    for (auto& execution: executions) {
        if (execution.hasFailed()) {
            continue;
        }
        try {
            const std::string rawResult = execution.getResult();
            auto result = nlohmann::json::parse(rawResult);
            results.push_back(std::move(result));
        } catch (std::exception& e) {
            continue;
        }
    }
    return results;
}
}