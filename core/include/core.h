#ifndef SERIO_CORE_H
#define SERIO_CORE_H

#include <ui.h>
#include <task.h>
#include <string>
#include <queue.h>
#include <SQLiteCpp/Database.h>

struct core {
    queue<task> task_queue;
    std::unique_ptr<SQLite::Database> database;
    ui_data ui_data;
    std::string serialized_ui_data;
    render_view render_view;
};

void init_core(core& core, const std::string& database_path, render_view render_view);
void enqueue_core_task(const std::string& raw_task, queue<task>& task_queue);
bool enqueue_back_task_from_render_task(const std::string& raw_render_task, queue<task>& task_queue);
void execute_core_task(core& core);

#endif //SERIO_CORE_H
