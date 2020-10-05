#include "tv-show-player/TvShowPlayer.h"

serio::core::TvShowPlayer::TvShowPlayer(serio::core::TvShowStorage &storage) : storage(storage) {}

serio::core::Player serio::core::TvShowPlayer::playEpisodeOfTvShow(const std::string &tvShowName, unsigned int episodeId) {
    std::optional<serio::core::Episode> episodeToPlay = storage.getEpisodeOfTvShowWithName(tvShowName, episodeId);
    if (!episodeToPlay) {
        throw TvShowEpisodeDoesNotExistError(tvShowName, episodeId);
    } else {
        return serio::core::Player(tvShowName, *episodeToPlay);
    }
}

serio::core::TvShowEpisodeDoesNotExistError::TvShowEpisodeDoesNotExistError(const std::string &tvShowName, unsigned int episodeId)
    : std::logic_error("Episode " + std::to_string(episodeId) + " of tv show " + tvShowName + " does not exist") {}
