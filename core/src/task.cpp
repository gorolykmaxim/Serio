#include "task.h"

bool task::operator==(const task &rhs) const {
    return type == rhs.type && args == rhs.args;
}

bool task::operator!=(const task &rhs) const {
    return !(rhs == *this);
}

task parse_task(const nlohmann::json &raw_task) {
    return task{
        raw_task["event"].get<task_type>(),
        raw_task["args"].get<std::vector<nlohmann::json>>()
    };
}
