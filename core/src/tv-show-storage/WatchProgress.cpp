#include <stdexcept>
#include "tv-show-storage/WatchProgress.h"

serio::core::WatchProgress::WatchProgress(double percentage) : percentage(percentage) {
    if (percentage < 0 || percentage > 100) {
        throw InvalidProgressValueError();
    }
}

double serio::core::WatchProgress::getPercentage() const {
    return percentage;
}

bool serio::core::WatchProgress::operator==(const serio::core::WatchProgress &rhs) const {
    return percentage == rhs.percentage;
}

bool serio::core::WatchProgress::operator!=(const serio::core::WatchProgress &rhs) const {
    return !(rhs == *this);
}

serio::core::InvalidProgressValueError::InvalidProgressValueError()
    : std::invalid_argument("Watch progress is a percentage value and should be in range from 0 to 100") {}
