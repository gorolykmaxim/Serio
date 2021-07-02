#include "error.h"

void trigger_fatal_error(const task &task) {
    if (task.type != error) return;
    throw std::logic_error(task.args.empty() ? std::string("no error message ") + u8"\U0001F602" : task.args[0].get<std::string>());
}
