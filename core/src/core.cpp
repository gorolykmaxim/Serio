#include <utility>
#include "core.h"

static void display_title_screen(ui_data& ui_data, const task& task) {
    if (task.id != init) return;
    ui_data = {view_id::title_screen};
    ui_data.animation = {animation_speed::slow, true, false};
}

static void hide_title_screen_after_delay(queue<task>& task_queue, const task& task) {
    if (task.id != init) return;
    std::this_thread::sleep_for(std::chrono::seconds(3));
    task_queue.enqueue({edit_crawler_config_url});
}

static void display_edit_text_dialog(ui_data& ui_data, const task& task) {
    if (task.id != edit_crawler_config_url) return;
    ui_data = {view_id::edit_text_dialog};
    ui_data.dialog = {
            "Crawler config URL",
            "Specify URL to the file, that contains configuration of all the crawlers, responsible for crawling tv shows.",
            "Cancel",
            "Save",
            {{process_http_response}},
    };
    ui_data.edit_text = {
            "Crawler config URL",
            "https://github.com/gorolykmaxim/content.json",
            {process_http_response},
            {process_http_response},
    };
    ui_data.back_task = {process_http_response};
}

static void display_error_dialog(ui_data& ui_data, const task& task) {
    if (task.id != process_http_response) return;
    ui_data = {view_id::dialog};
    ui_data.dialog = {
            "Whoops...",
            "Something went horribly wrong :(",
            "Understand"
    };
    ui_data.back_task = {edit_crawler_config_url};
}

void init_core(core& core, const std::string &database_path) {
    core.database = std::make_unique<SQLite::Database>(database_path, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
    core.task_queue.enqueue({init});
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
    const auto task = core.task_queue.dequeue();
    display_title_screen(core.ui_data, task);
    display_edit_text_dialog(core.ui_data, task);
    display_error_dialog(core.ui_data, task);
    render_ui(core.ui_data, core.render_task_queue);
    hide_title_screen_after_delay(core.task_queue, task);
}