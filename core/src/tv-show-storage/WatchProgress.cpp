#include <stdexcept>
#include "tv-show-storage/WatchProgress.h"

namespace serio::core {

WatchProgress::WatchProgress(double percentage) : percentage(percentage) {
    if (percentage < 0 || percentage > 100) {
        throw InvalidProgressValueError();
    }
}

double WatchProgress::getPercentage() const {
    return percentage;
}

bool WatchProgress::isComplete() const {
    return percentage >= 90;
}

bool WatchProgress::operator==(const WatchProgress &rhs) const {
    return percentage == rhs.percentage;
}

bool WatchProgress::operator!=(const WatchProgress &rhs) const {
    return !(rhs == *this);
}

InvalidProgressValueError::InvalidProgressValueError()
    : std::invalid_argument("Watch progress is a percentage value and should be in range from 0 to 100") {}

}