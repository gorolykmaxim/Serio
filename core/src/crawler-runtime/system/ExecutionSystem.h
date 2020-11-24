#ifndef SERIO_EXECUTIONSYSTEM_H
#define SERIO_EXECUTIONSYSTEM_H

#include <vector>
#include <set>
#include <crawler-runtime/CrawlerRuntime.h>
#include "../component/ExecutionComponent.h"

namespace serio {
class ExecutionSystem {
public:
    ExecutionSystem(const std::vector<Crawler>& crawlers, std::vector<ExecutionComponent>& executions);
    void update();
    [[nodiscard]] bool isExecutionFinished() const;
private:
    std::set<ExecutionComponent*> finishedExecutions;
    std::vector<ExecutionComponent>& executions;
};
}

#endif //SERIO_EXECUTIONSYSTEM_H
