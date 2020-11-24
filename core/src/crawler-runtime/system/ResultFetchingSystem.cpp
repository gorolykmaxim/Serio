#include "ResultFetchingSystem.h"

namespace serio {
ResultFetchingSystem::ResultFetchingSystem(const std::vector<Crawler> &crawlers,
                                           std::vector<ExecutionComponent> &executions) : executions(executions) {
    for (auto& execution: executions) {
        execution.code += " JSON.stringify(crawl.apply(null, args))";
    }
}

std::vector<nlohmann::json> ResultFetchingSystem::fetchResults() {
    std::vector<nlohmann::json> results;
    results.reserve(executions.size());
    for (auto& execution: executions) {
        if (execution.error != MJS_OK) {
            continue;
        }
        try {
            const auto stringResult = mjs_get_cstring(execution.mjs, &execution.result);
            auto result = nlohmann::json::parse(stringResult);
            results.push_back(std::move(result));
        } catch (std::exception& e) {
            continue;
        }
    }
    return results;
}
}