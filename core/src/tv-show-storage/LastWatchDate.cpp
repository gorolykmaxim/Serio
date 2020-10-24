#include "tv-show-storage/LastWatchDate.h"

namespace serio::core {

LastWatchDate::LastWatchDate(std::chrono::system_clock::time_point date)
    : date(date) {}

LastWatchDate::LastWatchDate(long long timeSinceEpoch)
    : date(std::chrono::system_clock::time_point(std::chrono::system_clock::duration(timeSinceEpoch))) {}

std::string LastWatchDate::toString() const {
    auto hoursSinceLastWatch = std::chrono::duration_cast<std::chrono::hours>(std::chrono::system_clock::now() - date);
    if (hoursSinceLastWatch < std::chrono::hours(24)) {
        return "today";
    } else if (hoursSinceLastWatch < std::chrono::hours(48)) {
        return "yesterday";
    } else if (hoursSinceLastWatch < std::chrono::hours(24 * 30)) {
        return std::to_string(hoursSinceLastWatch.count() / 24) + " days ago";
    } else if (hoursSinceLastWatch < std::chrono::hours(24 * 30 * 12)) {
        return std::to_string(hoursSinceLastWatch.count() / 24 / 30) + " month ago";
    } else {
        auto yearsSinceLastWatch = hoursSinceLastWatch.count() / 24 / 30 / 12;
        auto unitOfMeasure = yearsSinceLastWatch > 1 ? "years" : "year";
        return std::to_string(yearsSinceLastWatch) + " " + unitOfMeasure + " ago";
    }
}

bool LastWatchDate::operator==(const LastWatchDate &rhs) const {
    return date == rhs.date;
}

bool LastWatchDate::operator!=(const LastWatchDate &rhs) const {
    return !(rhs == *this);
}

long long LastWatchDate::getSinceEpoch() const {
    return date.time_since_epoch().count();
}

bool LastWatchDate::operator<(const LastWatchDate &rhs) const {
    return date < rhs.date;
}

bool LastWatchDate::operator>(const LastWatchDate &rhs) const {
    return rhs < *this;
}

bool LastWatchDate::operator<=(const LastWatchDate &rhs) const {
    return !(rhs < *this);
}

bool LastWatchDate::operator>=(const LastWatchDate &rhs) const {
    return !(*this < rhs);
}

}