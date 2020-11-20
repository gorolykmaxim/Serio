#ifndef SERIO_DATABASESTUB_H
#define SERIO_DATABASESTUB_H

#include <SQLiteCpp/Database.h>

namespace mocks {
class Database {
public:
    inline static SQLite::Database STUB = SQLite::Database(":memory:", SQLite::OPEN_READWRITE);
};
}

#endif //SERIO_DATABASESTUB_H
