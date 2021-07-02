#include <SQLiteCpp/Database.h>
#include <ui.h>
#include "core.h"

static void display_title_screen(ui_data& ui_data) {
    ui_data = {view_id::title_screen};
    ui_data.animation = {animation_speed::slow, true, false};
}

static void display_edit_text_dialog(ui_data& ui_data, bool a) {
    if (a) return;
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
    ui_data.back_event = {process_http_response};
}

static void display_error_dialog(ui_data& ui_data, bool a) {
    if (!a) return;
    ui_data = {view_id::dialog};
    ui_data.dialog = {
            "Whoops...",
            "Something went horribly wrong :(",
            "Understand"
    };
    ui_data.back_event = {process_http_response};
}

void core_main(const std::string &database_path, queue<std::string>& task_queue, const render_view& render_view) {
    SQLite::Database database(database_path, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
    ui_data ui_data{};
    std::string serialized_ui_data;
    display_title_screen(ui_data);
    auto seen_title_screen = false;
    auto a = true;
    while (true) {
        if (seen_title_screen) {
            const auto task = task_queue.dequeue();
            display_edit_text_dialog(ui_data, a);
            display_error_dialog(ui_data, a);
            a = !a;
        }
        serialize_ui_data(ui_data, serialized_ui_data);
        render_view(serialized_ui_data);
        if (!seen_title_screen) {
            std::this_thread::sleep_for(std::chrono::seconds(3));
            seen_title_screen = true;
            task_queue.enqueue(nlohmann::json({{"event", process_http_response}}).dump());
        }
    }
}
