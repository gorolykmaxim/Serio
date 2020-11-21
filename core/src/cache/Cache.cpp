#include <cache/Cache.h>
#include <sstream>

namespace serio {
Cache::Cache(SQLite::Database &database) : database(database) {
    database.exec("CREATE TABLE IF NOT EXISTS CACHE_ENTRY("
                  "KEY TEXT PRIMARY KEY, "
                  "VALUE TEXT, "
                  "EXPIRATION_DATE BIGINT)");
}

void Cache::put(const std::string &key, const std::string &value, const std::chrono::milliseconds &duration) {
    SQLite::Statement insertEntry(database, "INSERT OR REPLACE INTO CACHE_ENTRY VALUES(?, ?, ?)");
    insertEntry.bind(1, key);
    insertEntry.bind(2, value);
    insertEntry.bind(3, (std::chrono::system_clock::now() + duration).time_since_epoch().count());
    insertEntry.exec();
}

std::optional<std::string> Cache::get(const std::string &key, bool includeExpired) {
    std::stringstream queryBuilder;
    queryBuilder << "SELECT VALUE FROM CACHE_ENTRY WHERE KEY = ?";
    if (!includeExpired) {
        queryBuilder << " AND EXPIRATION_DATE > ?";
    }
    SQLite::Statement getEntry(database, queryBuilder.str());
    getEntry.bind(1, key);
    if (!includeExpired) {
        getEntry.bind(2, std::chrono::system_clock::now().time_since_epoch().count());
    }
    if (getEntry.executeStep()) {
        return getEntry.getColumn(0).getString();
    } else {
        return std::optional<std::string>();
    }
}

long Cache::size() {
    return database.execAndGet("SELECT COUNT() FROM CACHE_ENTRY");
}
}