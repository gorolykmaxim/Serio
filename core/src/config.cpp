#include <SQLiteCpp/Database.h>
#include <http.h>
#include "config.h"

const auto SOURCE_URL_PROPERTY = "source-url";
const auto CURRENT_LANGUAGE_PROPERTY = "current-language";
const auto CONFIG_CACHE_TTL = std::chrono::hours(24);

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

static void remove_config_property(SQLite::Database& database, const std::string& name) {
    SQLite::Statement remove(database, "DELETE FROM CONFIG_ENTRY WHERE NAME=?");
    remove.bind(1, name);
    remove.exec();
}

void init_config(SQLite::Database& database, const std::vector<const language*>& languages,
                 language const** current_language) {
    database.exec("CREATE TABLE IF NOT EXISTS CONFIG_ENTRY("
                  "NAME TEXT PRIMARY KEY,"
                  "VALUE TEXT)");
    const auto possible_current_language_name = get_config_property(database, CURRENT_LANGUAGE_PROPERTY);
    const auto current_language_name = possible_current_language_name ? *possible_current_language_name : "";
    const auto current = std::find_if(languages.cbegin(), languages.cend(),
                                      [&current_language_name] (const language* l) { return l->name == current_language_name; });
    if (current != languages.cend()) {
        *current_language = *current;
    } else {
        *current_language = languages[0];
        set_config_property(database, CURRENT_LANGUAGE_PROPERTY, (*current_language)->name);
    }
}

static void display_title_screen(ui_data& ui_data) {
    ui_data = {view_id::title_screen_view};
    ui_data.animations = {{view_container_id::body, animation_speed::slow, true, false}};
}

static void send_config_download_request(std::vector<http_request>& requests_to_send, const std::string& config_url,
                                         std::optional<task>& active_task, const task& task) {
    http_request req{task.id, config_url};
    req.cache_ttl = CONFIG_CACHE_TTL;
    requests_to_send.push_back(std::move(req));
    active_task = task;
}

static bool consume_response_to_task(http_response& res, std::vector<http_response>& responses,
                                     task_type expected_type, std::optional<task>& active_task) {
    if (!active_task || active_task->type != expected_type) return false;
    const auto it = std::find_if(responses.cbegin(), responses.cend(),
                                 [&active_task] (const http_response& res) { return res.request.id == active_task->id; });
    if (it == responses.cend()) return false;
    responses.erase(it);
    res = *it;
    active_task.reset();
    return true;
}

static std::string trim_response_body(const std::string& body) {
    return body.size() > 200 ? body.substr(0, 200) + "..." : body;
}

static void display_edit_crawler_config_url_dialog(ui_data& ui_data, const std::vector<translation>& translations,
                                                   const std::string& crawler_config_url, id_seed& id_seed) {
    ui_data = {view_id::edit_text_dialog_view};
    ui_data.dialog = {
            get_text(translations, edit_crawler_config_title),
            get_text(translations, edit_crawler_config_description),
            "",
            get_text(translations, edit_crawler_config_save),
            {{create_id(id_seed), download_new_config_task}},
    };
    ui_data.edit_text = {
            get_text(translations, edit_crawler_config_label),
            crawler_config_url,
            {create_id(id_seed), set_crawler_config_url_task},
            {create_id(id_seed), download_new_config_task},
    };
}

static void download_new_config(std::vector<http_request>& requests_to_send, id_seed& seed,
                                const std::vector<translation>& translations, const std::string& crawler_config_url,
                                ui_data& ui_data, std::optional<task>& active_task, const task& task) {
    send_config_download_request(requests_to_send, crawler_config_url, active_task, task);
    ui_data = {view_id::loading_screen_view};
    ui_data.loading = {get_text(translations, downloading_crawler_config)};
}

static bool read_config_response(const http_response& res, const std::vector<translation>& translations,
                                 const std::string& crawler_config_url, std::string& error_title, std::string& error_description) {
    if (res.code > 399) {
        error_title = get_text(translations, failed_to_download_crawler_config_title);
        error_description = get_text(translations, failed_to_download_crawler_config_description, crawler_config_url) + trim_response_body(res.body);
        return false;
    } else {
        try {
            nlohmann::json::parse(res.body);
        } catch (const std::exception& e) {
            error_title = get_text(translations, failed_to_parse_crawler_config_title);
            error_description = get_text(translations, failed_to_parse_crawler_config_description, trim_response_body(res.body)) + e.what();
            return false;
        }
    }
    return true;
}

static void display_config_download_error_dialog(ui_data& ui_data, const std::vector<translation>& translations,
                                                 const std::string& title, const std::string& description, id_seed& id_seed) {
    ui_data = {view_id::dialog_view};
    ui_data.dialog = {title, description, get_text(translations, failed_to_get_crawler_config_change_url)};
    ui_data.back_task = {create_id(id_seed), init_task};
}

static void save_new_downloaded_config(SQLite::Database& database, std::vector<http_response>& responses,
                                       const std::vector<translation>& translations, ui_data& ui_data,
                                       std::string& crawler_config_url, id_seed& id_seed,
                                       std::optional<task>& active_task) {
    http_response res;
    if (!consume_response_to_task(res, responses, download_new_config_task, active_task)) return;
    std::string err_title, err_description;
    if (read_config_response(res, translations, crawler_config_url, err_title, err_description)) {
        set_config_property(database, SOURCE_URL_PROPERTY, crawler_config_url);
        crawler_config_url = "";
        display_title_screen(ui_data);
    } else {
        clear_cached_response_to(database, res.request);
        display_config_download_error_dialog(ui_data, translations, err_title, err_description, id_seed);
    }
}

static void save_downloaded_config(SQLite::Database& database, std::vector<http_response>& responses,
                                   const std::vector<translation>& translations, ui_data& ui_data,
                                   std::string& crawler_config_url, id_seed& id_seed, std::optional<task>& active_task) {
    http_response res;
    if (!consume_response_to_task(res, responses, init_task, active_task)) return;
    std::string err_title, err_description;
    const auto existing_url = *get_config_property(database, SOURCE_URL_PROPERTY);
    if (!read_config_response(res, translations, existing_url, err_title, err_description)) {
        crawler_config_url = existing_url;
        remove_config_property(database, SOURCE_URL_PROPERTY);
        clear_cached_response_to(database, res.request);
        display_config_download_error_dialog(ui_data, translations, err_title, err_description, id_seed);
    }
}

void fetch_crawler_config(SQLite::Database& database, ui_data& ui_data, std::string& crawler_config_url, id_seed& seed,
                          std::vector<http_request>& requests_to_send, std::vector<http_response>& responses,
                          const std::vector<translation>& translations, std::optional<task>& active_task,
                          queue<task>& task_queue, const task& task) {
    if (task.type == init_task) {
        const auto existing_config_url = get_config_property(database, SOURCE_URL_PROPERTY);
        if (existing_config_url) {
            send_config_download_request(requests_to_send, *existing_config_url, active_task, task);
            display_title_screen(ui_data);
        } else {
            display_edit_crawler_config_url_dialog(ui_data, translations, crawler_config_url, seed);
        }
    } else if (task.type == set_crawler_config_url_task) {
        crawler_config_url = task.args[0].get<std::string>();
    } else if (task.type == download_new_config_task) {
        download_new_config(requests_to_send, seed, translations, crawler_config_url, ui_data, active_task, task);
    } else if (task.type == process_http_response_task) {
        save_new_downloaded_config(database, responses, translations, ui_data, crawler_config_url, seed, active_task);
        save_downloaded_config(database, responses, translations, ui_data, crawler_config_url, seed, active_task);
    }
}