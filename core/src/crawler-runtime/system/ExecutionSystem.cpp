#include "ExecutionSystem.h"

namespace serio {
ExecutionSystem::ExecutionSystem(const std::vector<Crawler> &crawlers,
                                 std::vector<ExecutionComponent> &executions) : executions(executions) {
    for (auto i = 0; i < crawlers.size(); i++) {
        executions[i].code += crawlers.at(i).code;
    }
}

void ExecutionSystem::update() {
    for (auto& execution: executions) {
        if (execution.execution.mjs == nullptr && execution.error == MJS_OK) {
            execution.error = mjs_start_execution(execution.mjs, &execution.execution, execution.code.c_str(),
                                                  &execution.result);
        } else if (!mjs_is_execution_done(&execution.execution) && execution.error == MJS_OK) {
            execution.error = mjs_execute_step(&execution.execution);
        } else {
            finishedExecutions.emplace(&execution);
        }
    }
}

bool ExecutionSystem::isExecutionFinished() const {
    return finishedExecutions.size() == executions.size();
}
}