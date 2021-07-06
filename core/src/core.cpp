#include <utility>
#include <config.h>
#include "core.h"

static void display_title_screen(ui_data& ui_data, const task& task) {
    if (task.type != display_title_screen_task) return;
    ui_data = {view_id::title_screen_view};
    ui_data.animation = {animation_speed::slow, true, false};
}

void init_core(core& core, const std::string &database_path) {
    core.database = std::make_unique<SQLite::Database>(database_path, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
    core.http_client.nf_client = nativeformat::http::createClient("", "");
    init_config(*core.database);
    init_http_client_cache(*core.database);
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
    read_http_responses(task, core.http_client.response_queue, http_responses);
    fetch_crawler_config(*core.database, core.ui_data, core.crawler_config_url, core.id_seed,
                         core.http_client.requests_to_send, http_responses, core.task_queue, task);
    display_title_screen(core.ui_data, task);
    send_http_requests(core.http_client, *core.database, core.task_queue, core.id_seed);
    render_ui(core.ui_data, core.render_task_queue);
}