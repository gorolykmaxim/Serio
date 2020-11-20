#ifndef SERIO_CACHE_H
#define SERIO_CACHE_H

#include <chrono>
#include <SQLiteCpp/Database.h>

namespace serio {
class Cache {
public:
    explicit Cache(SQLite::Database &database);
    virtual void put(const std::string& key, const std::string& value, const std::chrono::milliseconds& duration);
    virtual std::optional<std::string> get(const std::string& key);
    virtual long size();
private:
    SQLite::Database& database;
    void clearExpiredEntries();
    std::optional<std::string> getEntry(const std::string& key);
};
}

#endif //SERIO_CACHE_H
