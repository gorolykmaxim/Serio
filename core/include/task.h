#ifndef SERIO_TASK_H
#define SERIO_TASK_H

#include <nlohmann/json.hpp>
#include <queue.h>

enum task_type {
    error, init, process_http_response
};

struct task {
    task_type type;
    std::vector<nlohmann::json> args;

    bool operator==(const task &rhs) const;
    bool operator!=(const task &rhs) const;
};

class task_parse_error: public std::logic_error {
public:
    explicit task_parse_error(const std::string& task, const std::exception& error);
};

task read_task(queue<std::string>& queue);

#endif //SERIO_TASK_H
