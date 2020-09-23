#ifndef SERIO_INVALIDLISTROLE_H
#define SERIO_INVALIDLISTROLE_H

#include <stdexcept>

namespace serio::qt {

class InvalidListRole : public std::invalid_argument {
public:
    explicit InvalidListRole(int role);
};

}

#endif //SERIO_INVALIDLISTROLE_H
