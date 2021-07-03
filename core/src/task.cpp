#include "task.h"

bool task::operator==(const task &rhs) const {
    return id == rhs.id && args == rhs.args;
}

bool task::operator!=(const task &rhs) const {
    return !(rhs == *this);
}

task parse_task(const nlohmann::json &raw_task) {
    return task{
        raw_task["taskId"].get<task_id>(),
        raw_task["args"].get<std::vector<nlohmann::json>>()
    };
}
