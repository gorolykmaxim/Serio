#include <string>
#include <utility>
#include <stdexcept>
#include "tv-show-storage/Episode.h"

namespace serio::core {

Episode::Episode(unsigned int id, std::string videoUrl, std::optional<std::string> episodeName,
                 std::optional<LastWatchDate> lastWatchDate, WatchProgress watchProgress)
    : id(id),
      name(episodeName ? *episodeName : "Episode " + std::to_string(id)),
      videoUrl(std::move(videoUrl)),
      lastWatchDate(lastWatchDate),
      watchProgress(watchProgress) {
    if (id == 0) {
        throw std::invalid_argument("Episode ID must be greater than 0");
    }
}

unsigned int Episode::getId() const {
    return id;
}

std::string Episode::getName() const {
    return name;
}

std::string Episode::getVideoUrl() const {
    return videoUrl;
}

std::optional<LastWatchDate> Episode::getLastWatchDate() const {
    return lastWatchDate;
}

WatchProgress Episode::getWatchProgress() const {
    return watchProgress;
}

unsigned int Episode::getNextEpisodeId() const {
    return id + 1;
}

unsigned int Episode::getPreviousEpisodeId() const {
    return id == 1 ? 1 : id - 1;
}

bool Episode::isFirst() const {
    return id == 1;
}

bool Episode::operator==(const Episode &rhs) const {
    return id == rhs.id &&
           name == rhs.name &&
           videoUrl == rhs.videoUrl &&
           lastWatchDate == rhs.lastWatchDate &&
           watchProgress == rhs.watchProgress;
}

bool Episode::operator!=(const Episode &rhs) const {
    return !(rhs == *this);
}

}