#include <string>
#include <utility>
#include <stdexcept>
#include "tv-show-storage/Episode.h"

serio::core::Episode::Episode(unsigned int id, std::string videoUrl, std::optional<std::string> episodeName,
                              std::optional<serio::core::LastWatchDate> lastWatchDate)
    : id(id),
      name(episodeName ? *episodeName : "Episode " + std::to_string(id)),
      videoUrl(std::move(videoUrl)),
      lastWatchDate(std::move(lastWatchDate)) {
    if (id == 0) {
        throw std::invalid_argument("Episode ID must be greater than 0");
    }
}

unsigned int serio::core::Episode::getId() const {
    return id;
}

std::string serio::core::Episode::getName() const {
    return name;
}

std::string serio::core::Episode::getVideoUrl() const {
    return videoUrl;
}

std::optional<serio::core::LastWatchDate> serio::core::Episode::getLastWatchDate() const {
    return lastWatchDate;
}

bool serio::core::Episode::operator==(const serio::core::Episode &rhs) const {
    return id == rhs.id &&
           name == rhs.name &&
           videoUrl == rhs.videoUrl &&
           lastWatchDate == rhs.lastWatchDate;
}

bool serio::core::Episode::operator!=(const serio::core::Episode &rhs) const {
    return !(rhs == *this);
}
