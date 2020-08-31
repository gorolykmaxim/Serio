#include "tv-show-storage/TvShow.h"

#include <utility>
#include <optional>

serio::core::TvShow::TvShow(std::string name, std::string thumbnailUrl)
    : name(std::move(name)), thumbnailUrl(std::move(thumbnailUrl)) {}

serio::core::TvShow::TvShow(std::string name, std::string thumbnailUrl, LastWatchDate lastWatchDate)
    : name(std::move(name)), thumbnailUrl(std::move(thumbnailUrl)), lastWatchDate(lastWatchDate) {}

std::string serio::core::TvShow::getName() const {
    return name;
}

std::string serio::core::TvShow::getThumbnailUrl() const {
    return thumbnailUrl;
}

std::optional<serio::core::LastWatchDate> serio::core::TvShow::getLastWatchDate() const {
    return lastWatchDate;
}

bool serio::core::TvShow::operator==(const serio::core::TvShow &rhs) const {
    return name == rhs.name &&
           thumbnailUrl == rhs.thumbnailUrl &&
           lastWatchDate == rhs.lastWatchDate;
}

bool serio::core::TvShow::operator!=(const serio::core::TvShow &rhs) const {
    return !(rhs == *this);
}
