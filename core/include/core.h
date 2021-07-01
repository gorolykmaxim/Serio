#ifndef SERIO_CORE_H
#define SERIO_CORE_H

#include <string>
#include <queue.h>

typedef std::function<void(const std::string&)> render_view;

void core_main(const std::string& database_path, queue<std::string>& task_queue, const render_view& render_view);

#endif //SERIO_CORE_H
