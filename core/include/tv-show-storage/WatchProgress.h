#ifndef SERIO_WATCHPROGRESS_H
#define SERIO_WATCHPROGRESS_H

#include <stdexcept>

namespace serio::core {

class WatchProgress {
public:
    explicit WatchProgress(double percentage = 0);
    [[nodiscard]] double getPercentage() const;
    bool operator==(const WatchProgress &rhs) const;
    bool operator!=(const WatchProgress &rhs) const;
private:
    double percentage;
};

class InvalidProgressValueError : public std::invalid_argument {
public:
    InvalidProgressValueError();
};

}

#endif //SERIO_WATCHPROGRESS_H
