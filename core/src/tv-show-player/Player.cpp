#include "tv-show-player/Player.h"
#include <utility>

serio::core::Player::Player(std::string playingTvShowName, serio::core::Episode playingEpisode)
    : playingTvShowName(std::move(playingTvShowName)), playingEpisode(std::move(playingEpisode)) {}

serio::core::Episode serio::core::Player::getPlayingEpisode() const {
    return playingEpisode;
}

std::string serio::core::Player::getPlayingTvShowName() const {
    return playingTvShowName;
}
