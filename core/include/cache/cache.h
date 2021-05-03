#ifndef SERIO_CACHE_H
#define SERIO_CACHE_H

#include <SQLiteCpp/Database.h>
#include <chrono>
#include <optional>
#include <string>

namespace serio::cache {

void init(SQLite::Database &database);
void put(SQLite::Database &database, const std::string &key, const std::string &value, std::chrono::milliseconds duration);
std::optional<std::string> get(SQLite::Database& database, const std::string& key, bool include_expired = false);

}

#endif //SERIO_CACHE_H
