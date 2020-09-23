#include <string>
#include "InvalidListRole.h"

serio::qt::InvalidListRole::InvalidListRole(int role)
    : std::invalid_argument("Invalid role " + std::to_string(role)) {}
