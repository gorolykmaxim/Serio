#ifndef SERIO_CORE_H
#define SERIO_CORE_H

#include <ui.h>
#include <task.h>
#include <string>
#include <queue.h>

void core_main(const std::string& database_path, queue<task>& task_queue, const render_view& render_view);

#endif //SERIO_CORE_H
