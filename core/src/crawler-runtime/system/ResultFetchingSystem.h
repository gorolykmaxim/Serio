#ifndef SERIO_RESULTFETCHINGSYSTEM_H
#define SERIO_RESULTFETCHINGSYSTEM_H

#include <vector>
#include <set>
#include <crawler-runtime/CrawlerRuntime.h>
#include "../component/ExecutionComponent.h"

namespace serio {
class ResultFetchingSystem {
public:
    ResultFetchingSystem(const std::vector<Crawler>& crawlers, std::vector<ExecutionComponent>& executions);
    std::vector<nlohmann::json> fetchResults();
private:
    std::vector<ExecutionComponent>& executions;
};
}

#endif //SERIO_RESULTFETCHINGSYSTEM_H
