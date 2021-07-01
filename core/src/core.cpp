#include <SQLiteCpp/Database.h>
#include "core.h"

void core_main(const std::string &database_path, queue<std::string>& task_queue, const render_view& render_view) {
    SQLite::Database database(database_path, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
    while (true) {
        const auto data = task_queue.dequeue();
        render_view("Updated by C++ in LOOP: " + data);
    }
}
