#ifndef SERIO_CRAWLEREXECUTIONSYSTEM_H
#define SERIO_CRAWLEREXECUTIONSYSTEM_H

#include <vector>
#include <set>
#include <crawler-runtime/CrawlerRuntime.h>
#include "CrawlerExecution.h"

namespace serio {
class CrawlerExecutionSystem {
public:
    CrawlerExecutionSystem(std::vector<Crawler>& crawlers, std::vector<CrawlerExecution>& executions);
    void update();
    bool isFinished();
private:
    std::vector<CrawlerExecution>& executions;
    std::set<CrawlerExecution*> finishedExecutions;
};
}

#endif //SERIO_CRAWLEREXECUTIONSYSTEM_H
