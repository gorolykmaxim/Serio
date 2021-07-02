#ifndef SERIO_TASK_H
#define SERIO_TASK_H

#include <nlohmann/json.hpp>
#include <queue.h>

enum task_type {
    error, init, process_http_response, edit_crawler_config_url
};

struct task {
    task_type type;
    std::vector<nlohmann::json> args;

    bool operator==(const task &rhs) const;
    bool operator!=(const task &rhs) const;
};

task parse_task(const nlohmann::json& raw_task);

#endif //SERIO_TASK_H
