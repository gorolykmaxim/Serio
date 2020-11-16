#include "Cache.h"

namespace serio {
Cache::Cache(SQLite::Database &database) : database(database) {
    database.exec("CREATE TABLE IF NOT EXISTS CACHE_ENTRY("
                  "KEY TEXT PRIMARY KEY, "
                  "VALUE TEXT, "
                  "EXPIRATION_DATE BIGINT)");
}

void Cache::put(const std::string &key, const std::string &value, const std::chrono::milliseconds &duration) {
    SQLite::Statement insertEntry(database, "INSERT INTO CACHE_ENTRY VALUES(?, ?, ?)");
    insertEntry.bind(1, key);
    insertEntry.bind(2, value);
    insertEntry.bind(3, (std::chrono::system_clock::now() + duration).time_since_epoch().count());
    insertEntry.exec();
}

std::optional<std::string> Cache::get(const std::string &key) {
    clearExpiredEntries();
    return getEntry(key);
}

long Cache::size() {
    return database.execAndGet("SELECT COUNT() FROM CACHE_ENTRY");
}

void Cache::clearExpiredEntries() {
    SQLite::Statement clearExpired(database, "DELETE FROM CACHE_ENTRY WHERE EXPIRATION_DATE < ?");
    clearExpired.bind(1, std::chrono::system_clock::now().time_since_epoch().count());
    clearExpired.exec();
}

std::optional<std::string> Cache::getEntry(const std::string &key) {
    SQLite::Statement getEntry(database, "SELECT VALUE FROM CACHE_ENTRY WHERE KEY = ?");
    getEntry.bind(1, key);
    if (getEntry.executeStep()) {
        return getEntry.getColumn(0);
    } else {
        return std::optional<std::string>();
    }
}
}