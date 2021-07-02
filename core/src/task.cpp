#include "task.h"

bool task::operator==(const task &rhs) const {
    return type == rhs.type && args == rhs.args;
}

bool task::operator!=(const task &rhs) const {
    return !(rhs == *this);
}

task_parse_error::task_parse_error(const std::string& task, const std::exception &error)
    : logic_error("Failed to parse task '" + task + "': " + error.what()) {}

task read_task(queue<std::string> &queue) {
    const auto raw_task = queue.dequeue();
    try {
        const auto json_task = nlohmann::json::parse(raw_task);
        task task;
        task.type = static_cast<task_type>(json_task["event"].get<int>());
        const auto args = json_task.find("args");
        if (args != json_task.end()) {
            task.args = args->get<std::vector<nlohmann::json>>();
        }
        return task;
    } catch (const std::exception& e) {
        throw task_parse_error(raw_task, e);
    }
}
