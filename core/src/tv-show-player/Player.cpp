#include "tv-show-player/Player.h"
#include <utility>

namespace serio::core {

Player::Player(std::string playingTvShowName, Episode playingEpisode, bool nextEpisode, bool fromStart)
    : playingTvShowName(std::move(playingTvShowName)),
      playingEpisode(std::move(playingEpisode)),
      nextEpisode(nextEpisode),
      fromStart(fromStart) {}

Episode Player::getPlayingEpisode() const {
    return playingEpisode;
}

std::string Player::getPlayingTvShowName() const {
    return playingTvShowName;
}

double Player::getStartPercentage() const {
    return fromStart ? 0 : playingEpisode.getWatchProgress().getPercentage();
}

bool Player::hasPreviousEpisode() const {
    return !playingEpisode.isFirst();
}

bool Player::hasNextEpisode() const {
    return nextEpisode;
}

}