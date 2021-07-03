#include <SQLiteCpp/Database.h>
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

void core_main(const std::string &database_path, queue<task>& task_queue, const render_view& render_view) {
    SQLite::Database database(database_path, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
    ui_data ui_data{};
    std::string serialized_ui_data;
    task_queue.enqueue({init});
    while (true) {
        const auto task = task_queue.dequeue();
        display_title_screen(ui_data, task);
        display_edit_text_dialog(ui_data, task);
        display_error_dialog(ui_data, task);
        serialize_ui_data(ui_data, serialized_ui_data);
        render_view(serialized_ui_data);
        hide_title_screen_after_delay(task_queue, task);
    }
}
