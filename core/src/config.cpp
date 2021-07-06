#include <SQLiteCpp/Database.h>
#include <http.h>
#include "config.h"

const auto SOURCE_URL_PROPERTY = "source-url";

void init_config(SQLite::Database &database) {
    database.exec("CREATE TABLE IF NOT EXISTS CONFIG_ENTRY("
                  "NAME TEXT PRIMARY KEY,"
                  "VALUE TEXT)");
}

static void set_config_property(SQLite::Database &database, const std::string &name, const std::string &value) {
    SQLite::Statement insert(database, "INSERT OR REPLACE INTO CONFIG_ENTRY VALUES(?, ?)");
    insert.bind(1, name);
    insert.bind(2, value);
    insert.exec();
}

static std::optional<std::string> get_config_property(SQLite::Database &database, const std::string &name) {
    SQLite::Statement select(database, "SELECT VALUE FROM CONFIG_ENTRY WHERE NAME=?");
    select.bind(1, name);
    return select.executeStep() ? select.getColumn(0).getString() : std::optional<std::string>();
}

static void display_crawler_config_url_edit_dialog(ui_data& ui_data, const std::string& crawler_config_url, id_seed& id_seed) {
    ui_data = {view_id::edit_text_dialog_view};
    ui_data.dialog = {
            "Crawler config URL",
            "Specify URL to the file, that contains configuration of all the crawlers, responsible for crawling tv shows.",
            "",
            "Save",
            {{create_id(id_seed), download_config_task}},
    };
    ui_data.edit_text = {
            "Crawler config URL",
            crawler_config_url,
            {create_id(id_seed), set_crawler_config_url_task},
            {create_id(id_seed), download_config_task},
    };
}

static void download_config(std::vector<http_request>& requests_to_send, id_seed& seed,
                            const std::string& crawler_config_url, ui_data& ui_data) {
    http_request req{create_id(seed), crawler_config_url};
    req.cache_ttl = std::chrono::milliseconds(0);
    requests_to_send.push_back(std::move(req));
    ui_data = {view_id::loading_screen_view};
    ui_data.loading = {"Downloading crawler config..."};
}

static void save_downloaded_config(SQLite::Database& database, const std::vector<http_response>& responses,
                                   ui_data& ui_data, std::string& crawler_config_url, queue<task>& task_queue,
                                   id_seed& id_seed) {
    for (const auto& res: responses) {
        if (res.code > 399) {
            ui_data = {view_id::dialog_view};
            ui_data.dialog = {
                    "Failed to download crawler config",
                    "Failed to download '" + crawler_config_url + "': " + res.body,
                    "Change URL"
            };
            ui_data.back_task = {create_id(id_seed), init_task};
        } else {
            set_config_property(database, SOURCE_URL_PROPERTY, crawler_config_url);
            crawler_config_url = "";
            task_queue.enqueue({create_id(id_seed), display_title_screen_task});
        }
    }
}

void fetch_crawler_config(SQLite::Database& database, ui_data& ui_data, std::string& crawler_config_url, id_seed& seed,
                          std::vector<http_request>& requests_to_send, std::vector<http_response>& responses,
                          queue<task>& task_queue, const task& task) {
    if (task.type == init_task) {
        if (!get_config_property(database, SOURCE_URL_PROPERTY)) {
            display_crawler_config_url_edit_dialog(ui_data, crawler_config_url, seed);
        } else {
            task_queue.enqueue({create_id(seed), display_title_screen_task});
        }
    } else if (task.type == set_crawler_config_url_task) {
        crawler_config_url = task.args[0].get<std::string>();
    } else if (task.type == download_config_task) {
        download_config(requests_to_send, seed, crawler_config_url, ui_data);
    } else if (task.type == process_http_response_task) {
        save_downloaded_config(database, responses, ui_data, crawler_config_url, task_queue, seed);
    }
}