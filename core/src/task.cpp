#include "task.h"

bool task::operator==(const task &rhs) const {
    return id == rhs.id && type == rhs.type && args == rhs.args;
}

bool task::operator!=(const task &rhs) const {
    return !(rhs == *this);
}

task parse_task(const nlohmann::json &raw_task) {
    return task{
        raw_task["id"].get<id>(),
        raw_task["type"].get<task_type>(),
        raw_task["args"].get<std::vector<nlohmann::json>>()
    };
}
