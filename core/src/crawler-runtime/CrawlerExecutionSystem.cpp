#include "CrawlerExecutionSystem.h"

namespace serio {
CrawlerExecutionSystem::CrawlerExecutionSystem(std::vector<Crawler> &crawlers,
                                               std::vector<CrawlerExecution> &executions)
    : executions(executions) {
    executions.reserve(crawlers.size());
    for (const auto& crawler: crawlers) {
        executions.emplace_back(crawler.code);
    }
}

void CrawlerExecutionSystem::update() {
    finished = true;
    for (auto& execution: executions) {
        if (!execution.isWaiting() && !execution.isDone()) {
            execution.executeStep();
            finished = false;
        }
    }
}

bool CrawlerExecutionSystem::isFinished() {
    return finished;
}
}