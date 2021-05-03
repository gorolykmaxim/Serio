#include "cache/cache.h"
#include <sstream>

namespace serio::cache {

void init(SQLite::Database &database) {
    database.exec("CREATE TABLE IF NOT EXISTS CACHE_ENTRY("
                  "KEY TEXT PRIMARY KEY,"
                  "VALUE TEXT,"
                  "EXPIRATION_DATE BIGINT)");
}

std::optional<std::string> get(SQLite::Database &database, const std::string &key, bool include_expired) {
    std::stringstream query;
    query << "SELECT VALUE FROM CACHE_ENTRY WHERE KEY=?";
    if (!include_expired) {
        query << " AND EXPIRATION_DATE>?";
    }
    SQLite::Statement select(database, query.str());
    select.bind(1, key);
    if (!include_expired) {
        select.bind(2, std::chrono::system_clock::now().time_since_epoch().count());
    }
    return select.executeStep() ? select.getColumn(0).getString() : std::optional<std::string>();
}

void put(SQLite::Database &database, const std::string &key, const std::string &value, std::chrono::milliseconds duration) {
    SQLite::Statement insert(database, "INSERT OR REPLACE INTO CACHE_ENTRY VALUES(?, ?, ?)");
    insert.bind(1, key);
    insert.bind(2, value);
    insert.bind(3, (std::chrono::system_clock::now() + duration).time_since_epoch().count());
    insert.exec();
}

}