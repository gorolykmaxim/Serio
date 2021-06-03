#include "task.h"

bool task::operator==(const task &rhs) const {
    return true;
}

bool task::operator!=(const task &rhs) const {
    return !(rhs == *this);
}