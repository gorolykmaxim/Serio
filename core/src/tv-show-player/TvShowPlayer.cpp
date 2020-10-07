#include "tv-show-player/TvShowPlayer.h"

serio::core::TvShowPlayer::TvShowPlayer(serio::core::TvShowStorage &storage) : storage(storage) {}

serio::core::Player serio::core::TvShowPlayer::playEpisodeOfTvShow(const std::string &tvShowName, unsigned int episodeId) {
    auto episodeToPlay = storage.getEpisodeOfTvShowWithName(tvShowName, episodeId);
    if (!episodeToPlay) {
        throw TvShowEpisodeDoesNotExistError(tvShowName, episodeId);
    } else {
        player = serio::core::Player(tvShowName, *episodeToPlay);
        return *player;
    }
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

serio::core::TvShowEpisodeDoesNotExistError::TvShowEpisodeDoesNotExistError(const std::string &tvShowName, unsigned int episodeId)
    : std::logic_error("Episode " + std::to_string(episodeId) + " of tv show " + tvShowName + " does not exist") {}

serio::core::NoTvShowEpisodePlayingError::NoTvShowEpisodePlayingError()
    : std::logic_error("No tv show episode is being played right now") {}
