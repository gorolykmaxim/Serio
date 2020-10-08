#include "tv-show-player/TvShowPlayer.h"

serio::core::TvShowPlayer::TvShowPlayer(serio::core::TvShowStorage &storage) : storage(storage) {}

serio::core::Player serio::core::TvShowPlayer::playEpisodeOfTvShow(const std::string &tvShowName, unsigned int episodeId) {
    return setPlayer(serio::core::Player(tvShowName, getEpisodeOrFail(tvShowName, episodeId)));
}

void serio::core::TvShowPlayer::updatePlayingEpisodeWatchProgress(serio::core::WatchProgress progress) {
    if (!player) {
        throw NoTvShowEpisodePlayingError();
    } else {
        storage.watchTvShowEpisode(player->getPlayingTvShowName(),
                                   player->getPlayingEpisode().getId(),
                                   serio::core::LastWatchDate(),
                                   progress);
    }
}

serio::core::Player serio::core::TvShowPlayer::playTvShow(const std::string &tvShowName) {
    auto lastWatchedEpisode = storage.getLastWatchedEpisodeOfTvShow(tvShowName);
    if (lastWatchedEpisode && !lastWatchedEpisode->getWatchProgress().isComplete()) {
        return setPlayer(serio::core::Player(tvShowName, *lastWatchedEpisode, false));
    } else if (lastWatchedEpisode) {
        return setPlayer(serio::core::Player(tvShowName, getEpisodeOrFail(tvShowName, lastWatchedEpisode->getId() + 1)));
    } else {
        return setPlayer(serio::core::Player(tvShowName, getEpisodeOrFail(tvShowName, 1)));
    }
}

serio::core::Episode serio::core::TvShowPlayer::getEpisodeOrFail(const std::string &tvShowName, unsigned int episodeId) {
    auto episode = storage.getEpisodeOfTvShowWithName(tvShowName, episodeId);
    if (!episode) {
        throw TvShowEpisodeDoesNotExistError(tvShowName, episodeId);
    } else {
        return *episode;
    }
}

serio::core::Player serio::core::TvShowPlayer::setPlayer(serio::core::Player newPlayer) {
    player = std::move(newPlayer);
    return *player;
}

serio::core::TvShowEpisodeDoesNotExistError::TvShowEpisodeDoesNotExistError(const std::string &tvShowName, unsigned int episodeId)
    : std::logic_error("Episode " + std::to_string(episodeId) + " of tv show " + tvShowName + " does not exist") {}

serio::core::NoTvShowEpisodePlayingError::NoTvShowEpisodePlayingError()
    : std::logic_error("No tv show episode is being played right now") {}
