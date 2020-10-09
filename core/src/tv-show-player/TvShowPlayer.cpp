#include "tv-show-player/TvShowPlayer.h"

serio::core::TvShowPlayer::TvShowPlayer(serio::core::TvShowStorage &storage) : storage(storage) {}

serio::core::Player serio::core::TvShowPlayer::playEpisodeOfTvShow(const std::string &tvShowName, unsigned int episodeId) {
    return createPlayer(tvShowName, getEpisodeOrFail(tvShowName, episodeId));
}

void serio::core::TvShowPlayer::updatePlayingEpisodeWatchProgress(serio::core::WatchProgress progress) {
    assertEpisodeIsPlaying();
    storage.watchTvShowEpisode(player->getPlayingTvShowName(),
                               player->getPlayingEpisode().getId(),
                               serio::core::LastWatchDate(),
                               progress);
}

serio::core::Player serio::core::TvShowPlayer::playTvShow(const std::string &tvShowName) {
    auto lastWatchedEpisode = storage.getLastWatchedEpisodeOfTvShow(tvShowName);
    if (lastWatchedEpisode && !lastWatchedEpisode->getWatchProgress().isComplete()) {
        return createPlayer(tvShowName, std::move(*lastWatchedEpisode), false);
    } else if (lastWatchedEpisode) {
        return createPlayer(tvShowName, getEpisodeOrFail(tvShowName, lastWatchedEpisode->getNextEpisodeId()));
    } else {
        return createPlayer(tvShowName, getEpisodeOrFail(tvShowName, 1));
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

serio::core::Player serio::core::TvShowPlayer::createPlayer(const std::string& tvShowName,
                                                            serio::core::Episode&& episode, bool fromStart) {
    auto nextEpisode = storage.getEpisodeOfTvShowWithName(tvShowName, episode.getNextEpisodeId());
    player = serio::core::Player(tvShowName, episode, nextEpisode.has_value(), fromStart);
    return *player;
}

serio::core::Player serio::core::TvShowPlayer::playPreviousEpisode() {
    return playNextOrPreviousEpisode(false);
}

serio::core::Player serio::core::TvShowPlayer::playNextEpisode() {
    return playNextOrPreviousEpisode(true);
}

serio::core::Player serio::core::TvShowPlayer::playNextOrPreviousEpisode(bool next) {
    assertEpisodeIsPlaying();
    auto tvShowName = player->getPlayingTvShowName();
    auto episode = player->getPlayingEpisode();
    return createPlayer(tvShowName, getEpisodeOrFail(tvShowName, next ? episode.getNextEpisodeId() : episode.getPreviousEpisodeId()));
}

void serio::core::TvShowPlayer::assertEpisodeIsPlaying() const {
    if (!player) {
        throw NoTvShowEpisodePlayingError();
    }
}

bool serio::core::TvShowPlayer::isTvShowWatchComplete(const std::string& tvShowName) {
    auto lastWatchedEpisode = storage.getLastWatchedEpisodeOfTvShow(tvShowName);
    if (!lastWatchedEpisode || !lastWatchedEpisode->getWatchProgress().isComplete()) {
        return false;
    } else {
        return !storage.getEpisodeOfTvShowWithName(tvShowName, lastWatchedEpisode->getNextEpisodeId());
    }
}

serio::core::TvShowEpisodeDoesNotExistError::TvShowEpisodeDoesNotExistError(const std::string &tvShowName, unsigned int episodeId)
    : std::logic_error("Episode " + std::to_string(episodeId) + " of tv show " + tvShowName + " does not exist") {}

serio::core::NoTvShowEpisodePlayingError::NoTvShowEpisodePlayingError()
    : std::logic_error("No tv show episode is being played right now") {}
