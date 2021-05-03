#include <SQLiteCpp/Database.h>
#include "config.h"

void init_config(SQLite::Database &database) {
    database.exec("CREATE TABLE IF NOT EXISTS CONFIG_ENTRY("
                  "NAME TEXT PRIMARY KEY,"
                  "VALUE TEXT)");
}

void set_config_property(SQLite::Database &database, const std::string &name, const std::string &value) {
    SQLite::Statement insert(database, "INSERT OR REPLACE INTO CONFIG_ENTRY VALUES(?, ?)");
    insert.bind(1, name);
    insert.bind(2, value);
    insert.exec();
}

std::optional<std::string> get_config_property(SQLite::Database &database, const std::string &name) {
    SQLite::Statement select(database, "SELECT VALUE FROM CONFIG_ENTRY WHERE NAME=?");
    select.bind(1, name);
    return select.executeStep() ? select.getColumn(0).getString() : std::optional<std::string>();
}
