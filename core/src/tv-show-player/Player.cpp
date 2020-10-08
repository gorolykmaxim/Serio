#include "tv-show-player/Player.h"
#include <utility>

serio::core::Player::Player(std::string playingTvShowName, serio::core::Episode playingEpisode, bool fromStart)
    : playingTvShowName(std::move(playingTvShowName)), playingEpisode(std::move(playingEpisode)), fromStart(fromStart) {}

serio::core::Episode serio::core::Player::getPlayingEpisode() const {
    return playingEpisode;
}

std::string serio::core::Player::getPlayingTvShowName() const {
    return playingTvShowName;
}

double serio::core::Player::getStartPercentage() const {
    return fromStart ? 0 : playingEpisode.getWatchProgress().getPercentage();
}
