#include "CrawlerExecutionSystem.h"

namespace serio {
CrawlerExecutionSystem::CrawlerExecutionSystem(std::vector<Crawler> &crawlers,
                                               std::vector<CrawlerExecution> &executions) : executions(executions) {
    executions.reserve(crawlers.size());
    for (const auto& crawler: crawlers) {
        executions.emplace_back(crawler.code);
    }
}

void CrawlerExecutionSystem::update() {
    for (auto& execution: executions) {
        if (execution.isDone()) {
            finishedExecutions.emplace(&execution);
        } else if (!execution.isWaiting()) {
            execution.executeStep();
        }
    }
}

bool CrawlerExecutionSystem::isFinished() {
    return finishedExecutions.size() == executions.size();
}
}