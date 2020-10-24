#include "tv-show-storage/TvShow.h"
#include <utility>
#include <optional>

namespace serio::core {

TvShow::TvShow(std::string name, std::string thumbnailUrl, std::optional<LastWatchDate> lastWatchDate)
    : name(std::move(name)), thumbnailUrl(std::move(thumbnailUrl)), lastWatchDate(lastWatchDate) {}

std::string TvShow::getName() const {
    return name;
}

std::string TvShow::getThumbnailUrl() const {
    return thumbnailUrl;
}

std::optional<LastWatchDate> TvShow::getLastWatchDate() const {
    return lastWatchDate;
}

bool TvShow::operator==(const TvShow &rhs) const {
    return name == rhs.name &&
           thumbnailUrl == rhs.thumbnailUrl &&
           lastWatchDate == rhs.lastWatchDate;
}

bool TvShow::operator!=(const TvShow &rhs) const {
    return !(rhs == *this);
}

}