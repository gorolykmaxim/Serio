#include <utility>
#include <config.h>
#include "core.h"

void init_core(core& core, const std::string &database_path) {
    core.database = std::make_unique<SQLite::Database>(database_path, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
    core.nf_client = nativeformat::http::createClient("", "");
    init_config(*core.database);
    init_http_client_cache(*core.database);
    init_localization(core.languages, &core.current_language);
    core.task_queue.enqueue({init_task});
}

void enqueue_core_task(const std::string &raw_task, queue<task>& task_queue) {
    const auto json_task = nlohmann::json::parse(raw_task);
    const auto task = parse_task(json_task);
    task_queue.enqueue(task);
}

bool enqueue_back_task_from_render_task(const std::string &raw_render_task, queue<task> &task_queue) {
    const auto render_task_json = nlohmann::json::parse(raw_render_task);
    const auto back_task = render_task_json.find("backTask");
    if (back_task != render_task_json.end()) {
        const auto task = parse_task(*back_task);
        task_queue.enqueue(task);
        return true;
    } else {
        return false;
    }
}

void execute_core_task(core &core) {
    std::vector<http_response> http_responses;
    const auto task = core.task_queue.dequeue();
    read_http_responses(task, core.response_queue, http_responses);
    fetch_crawler_config(*core.database, core.ui_data, core.crawler_config_url, core.id_seed,
                         core.requests_to_send, http_responses, core.current_language->translations,
                         core.active_task, core.task_queue, task);
    send_http_requests(*core.nf_client, core.requests_to_send,core.response_queue, core.user_agents,
                       *core.database, core.task_queue, core.id_seed);
    render_ui(core.ui_data, core.render_task_queue);
}