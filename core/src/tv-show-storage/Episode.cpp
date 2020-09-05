#include <string>
#include <utility>
#include <stdexcept>
#include "tv-show-storage/Episode.h"

serio::core::Episode::Episode(unsigned int id, std::string videoUrl, std::optional<std::string> episodeName)
    : id(id), name(episodeName ? *episodeName : "Episode " + std::to_string(id)), videoUrl(std::move(videoUrl)) {
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

bool serio::core::Episode::operator==(const serio::core::Episode &rhs) const {
    return id == rhs.id &&
           name == rhs.name &&
           videoUrl == rhs.videoUrl;
}

bool serio::core::Episode::operator!=(const serio::core::Episode &rhs) const {
    return !(rhs == *this);
}
