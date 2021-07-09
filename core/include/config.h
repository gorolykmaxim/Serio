#ifndef SERIO_CONFIG_H
#define SERIO_CONFIG_H

#include <SQLiteCpp/Database.h>
#include "http.h"
#include "id.h"
#include "task.h"
#include "ui.h"
#include "localization.h"

void init_config(SQLite::Database& database, const std::vector<const language*>& languages,
                 language const** current_language);
void fetch_crawler_config(SQLite::Database& database, ui_data& ui_data, std::string& crawler_config_url, id_seed& seed,
                          std::vector<http_request>& requests_to_send, std::vector<http_response>& responses,
                          const std::vector<translation>& translations, std::optional<task>& active_task,
                          queue<task>& task_queue, const task& task);

#endif //SERIO_CONFIG_H
