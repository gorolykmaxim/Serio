#include <SQLiteCpp/Database.h>
#include <nlohmann/json.hpp>
#include "core.h"

static std::string create_edit_text_view_task() {
    nlohmann::json res = {
            {"view", 1},
            {"dialog", {
                {"title", "Crawler config URL"},
                {"description", "Specify URL to the file, that contains configuration of all the crawlers, responsible for crawling tv shows."},
                {"cancelText", "Cancel"},
                {"confirmText", "Save"},
                {"confirmEvent", {{"event", 2}}},
            }},
            {"editText", {
                {"label", "Crawler config URL"},
                {"value", "https://github.com/gorolykmaxim/content.json"},
                {"valueChangedEvent", {{"event", 3}}},
                {"saveValueEvent", {{"event", 4}}},
            }},
            {"backEvent", {{"event", 1}}},
    };
    return res.dump();
}

static std::string create_error_view_task() {
    nlohmann::json res = {
            {"view", 2},
            {"dialog", {
                {"title", "Whoops..."},
                {"description", "Something went horribly wrong :("},
                {"cancelText", "Understand"},
            }},
            {"backEvent", {{"event", 1}}},
    };
    return res.dump();
}

void core_main(const std::string &database_path, queue<std::string>& task_queue, const render_view& render_view) {
    SQLite::Database database(database_path, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
    std::this_thread::sleep_for(std::chrono::seconds(2));
    auto a = false;
    while (true) {
        render_view(a ? create_error_view_task() : create_edit_text_view_task());
        a = !a;
        const auto data = task_queue.dequeue();
    }
}
