#include "tv-show-storage/TvShow.h"

#include <utility>

serio::core::TvShow::TvShow(std::string name, std::string thumbnailUrl)
    : name(std::move(name)), thumbnailUrl(std::move(thumbnailUrl)) {}

serio::core::TvShow::TvShow(std::string name, std::string thumbnailUrl, LastWatchDate lastWatchDate)
    : name(std::move(name)), thumbnailUrl(std::move(thumbnailUrl)), lastWatchDate(std::make_unique<LastWatchDate>(lastWatchDate)) {}

serio::core::TvShow::TvShow(const TvShow& other)
    : name(other.name), thumbnailUrl(other.thumbnailUrl), lastWatchDate(other.lastWatchDate ? std::make_unique<LastWatchDate>(*other.lastWatchDate) : nullptr) {}

serio::core::TvShow::TvShow(serio::core::TvShow &&other) noexcept
    : name(other.name), thumbnailUrl(other.thumbnailUrl), lastWatchDate(std::move(other.lastWatchDate)) {}

std::string serio::core::TvShow::getName() const {
    return name;
}

std::string serio::core::TvShow::getThumbnailUrl() const {
    return thumbnailUrl;
}

serio::core::LastWatchDate* serio::core::TvShow::getLastWatchDate() const {
    return lastWatchDate.get();
}

bool serio::core::TvShow::operator==(const serio::core::TvShow &rhs) const {
    return name == rhs.name &&
           thumbnailUrl == rhs.thumbnailUrl &&
            (lastWatchDate == rhs.lastWatchDate || *lastWatchDate == *rhs.lastWatchDate);
}

bool serio::core::TvShow::operator!=(const serio::core::TvShow &rhs) const {
    return !(rhs == *this);
}

serio::core::TvShow& serio::core::TvShow::operator=(serio::core::TvShow rhs) {
    if (&rhs != this) {
        std::swap(name, rhs.name);
        std::swap(thumbnailUrl, rhs.thumbnailUrl);
        std::swap(lastWatchDate, rhs.lastWatchDate);
    }
    return *this;
}

serio::core::TvShow &serio::core::TvShow::operator=(serio::core::TvShow &&rhs) noexcept {
    name = std::move(rhs.name);
    thumbnailUrl = std::move(rhs.thumbnailUrl);
    lastWatchDate = std::move(rhs.lastWatchDate);
    return *this;
}
