#include "config/PersistentProperties.h"

namespace serio {
PersistentProperties::PersistentProperties(SQLite::Database &database) : database(database) {
    database.exec("CREATE TABLE IF NOT EXISTS CONFIG(KEY TEXT PRIMARY KEY, VALUE TEXT)");
}

void PersistentProperties::setProperty(const std::string &name, const std::string &value) {
    SQLite::Statement saveSourceUrl(database, "INSERT OR REPLACE INTO CONFIG VALUES(?, ?)");
    saveSourceUrl.bind(1, name);
    saveSourceUrl.bind(2, value);
    saveSourceUrl.exec();
}

std::optional<std::string> PersistentProperties::getProperty(const std::string &name) {
    SQLite::Statement getSourceUrl(database, "SELECT VALUE FROM CONFIG WHERE KEY = ?");
    getSourceUrl.bind(1, name);
    if (getSourceUrl.executeStep()) {
        return getSourceUrl.getColumn(0).getString();
    } else {
        return std::optional<std::string>();
    }
}
}