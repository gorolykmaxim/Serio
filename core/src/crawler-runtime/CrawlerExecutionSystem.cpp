#include "CrawlerExecutionSystem.h"

namespace serio {
CrawlerExecutionSystem::CrawlerExecutionSystem(std::vector<Crawler> &crawlers,
                                               std::vector<CrawlerExecution> &executions) : executions(executions) {
    executions.reserve(crawlers.size());
    for (const auto& crawler: crawlers) {
        try {
            executions.emplace_back(crawler.code, crawler.arguments);
        } catch (std::logic_error& e) {
            throw InvalidCrawlerError(crawler, e.what());
        }
    }
}

void CrawlerExecutionSystem::update() {
    for (auto& execution: executions) {
        if (execution.isDone()) {
            finishedExecutions.emplace(&execution);
        } else {
            execution.executeStep();
        }
    }
}

bool CrawlerExecutionSystem::isFinished() {
    return finishedExecutions.size() == executions.size();
}
}