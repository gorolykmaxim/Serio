#ifndef SERIO_LOGGINGSYSTEM_H
#define SERIO_LOGGINGSYSTEM_H

#include <vector>
#include "CrawlerExecution.h"

namespace serio {
enum ExecutionStatus {
    running, waiting, failed, done
};

class LoggingSystem {
public:
    explicit LoggingSystem(std::vector<CrawlerExecution>& executions);
    void update();
private:
    std::vector<CrawlerExecution>& executions;
    std::vector<ExecutionStatus> statuses;

    ExecutionStatus getStatusOf(CrawlerExecution& execution) const;
    void logExecutionStatusChange(ExecutionStatus newStatus, CrawlerExecution& execution,
                                  uint32_t executionHandle) const;
};
}

#endif //SERIO_LOGGINGSYSTEM_H
