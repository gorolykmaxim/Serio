#ifndef SERIO_CONFIG_H
#define SERIO_CONFIG_H

#include <optional>
#include <string>
#include <SQLiteCpp/Database.h>

const std::string CONFIG_SOURCE_URL = "source-url";

void init_config(SQLite::Database& database);
void set_config_property(SQLite::Database& database, const std::string& name, const std::string& value);
std::optional<std::string> get_config_property(SQLite::Database& database, const std::string& name);

#endif //SERIO_CONFIG_H
