#ifndef SERIO_TASK_H
#define SERIO_TASK_H

enum task_type {
    process_http_response
};

struct task {
    task_type type;

    bool operator==(const task &rhs) const;
    bool operator!=(const task &rhs) const;
};

#endif //SERIO_TASK_H
