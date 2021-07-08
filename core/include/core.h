#ifndef SERIO_CORE_H
#define SERIO_CORE_H

#include <ui.h>
#include <task.h>
#include <string>
#include <queue.h>
#include <http.h>
#include <SQLiteCpp/Database.h>
#include "localization.h"

struct core {
    id_seed id_seed;
    queue<task> task_queue;
    queue<std::string> render_task_queue;
    std::unique_ptr<SQLite::Database> database;
    std::shared_ptr<nativeformat::http::Client> nf_client;
    std::vector<http_request> requests_to_send;
    queue<http_response> response_queue;
    std::vector<std::string> user_agents;
    std::string crawler_config_url;
    std::optional<task> active_task;
    language const* current_language;
    std::vector<const language*> languages;
    ui_data ui_data;
};

void init_core(core& core, const std::string& database_path);
void enqueue_core_task(const std::string& raw_task, queue<task>& task_queue);
bool enqueue_back_task_from_render_task(const std::string& raw_render_task, queue<task>& task_queue);
void execute_core_task(core& core);

#endif //SERIO_CORE_H
