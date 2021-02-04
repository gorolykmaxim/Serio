#ifndef SERIO_PERSISTENTPROPERTIES_H
#define SERIO_PERSISTENTPROPERTIES_H

#include <SQLiteCpp/Database.h>

namespace serio {
class PersistentProperties {
public:
    explicit PersistentProperties(SQLite::Database& database);
    void setProperty(const std::string& name, const std::string& value);
    std::optional<std::string> getProperty(const std::string& name);
private:
    SQLite::Database& database;
};
}

#endif //SERIO_PERSISTENTPROPERTIES_H
