#ifndef SERIO_RESULTFETCHSYSTEM_H
#define SERIO_RESULTFETCHSYSTEM_H

#include <vector>
#include <crawler-runtime/CrawlerRuntime.h>
#include "CrawlerExecution.h"

namespace serio {
class ResultFetchSystem {
public:
    explicit ResultFetchSystem(std::vector<Crawler>& crawlers, std::vector<CrawlerExecution>& executions);
    std::vector<nlohmann::json> fetch();
private:
    std::vector<CrawlerExecution>& executions;
};
}

#endif //SERIO_RESULTFETCHSYSTEM_H
