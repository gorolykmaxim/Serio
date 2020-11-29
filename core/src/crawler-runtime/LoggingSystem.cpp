#include "LoggingSystem.h"
#include <iostream>

namespace serio {
LoggingSystem::LoggingSystem(std::vector<CrawlerExecution> &executions)
    : executions(executions), statuses(executions.size(), ExecutionStatus::running) {}

void LoggingSystem::update() {
    for (auto i = 0; i < executions.size(); i++) {
        auto& execution = executions[i];
        auto& status = statuses[i];
        auto currentStatus = getStatusOf(execution);
        if (currentStatus != status) {
            logExecutionStatusChange(currentStatus, execution, i + 1);
            status = currentStatus;
        }
    }
}

ExecutionStatus LoggingSystem::getStatusOf(CrawlerExecution &execution) const {
    if (execution.hasFailed()) {
        return ExecutionStatus::failed;
    } else if (execution.isWaiting()) {
        return ExecutionStatus::waiting;
    } else if (execution.isDone()) {
        return ExecutionStatus::done;
    } else {
        return ExecutionStatus::running;
    }
}

void LoggingSystem::logExecutionStatusChange(ExecutionStatus newStatus, CrawlerExecution &execution,
                                             uint32_t executionHandle) const {
    switch (newStatus) {
        case done:
            std::cerr << "Crawler #" << std::to_string(executionHandle) << ": Finished executing successfully" << std::endl << std::endl;
            break;
        case running:
            std::cerr << "Crawler #" << std::to_string(executionHandle) << ": Back to running" << std::endl
                      << "Shared buffer contents: " << execution.readSharedBuffer().stringify() << std::endl << std::endl;
            break;
        case waiting:
            std::cerr << "Crawler #" << std::to_string(executionHandle) << ": Has started waiting" << std::endl
                      << "Waiting target: " << execution.getWaitingTarget() << std::endl
                      << "Shared buffer contents: " << execution.readSharedBuffer().stringify() << std::endl << std::endl;
            break;
        case failed:
            std::cerr << "Crawler #" << std::to_string(executionHandle) << ": Has failed:" << std::endl
                      << execution.getErrorMessage() << std::endl << std::endl;
            break;
    }
}
}