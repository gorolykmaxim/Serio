#include "tv-show-player/TvShowPlayer.h"

namespace serio::core {

TvShowPlayer::TvShowPlayer(TvShowStorage &storage) : storage(storage) {}

Player TvShowPlayer::playEpisodeOfTvShow(const std::string &tvShowName, unsigned int episodeId) {
    return createPlayer(tvShowName, getEpisodeOrFail(tvShowName, episodeId));
}

void TvShowPlayer::updatePlayingEpisodeWatchProgress(WatchProgress progress) {
    assertEpisodeIsPlaying();
    storage.watchTvShowEpisode(player->getPlayingTvShowName(),
                               player->getPlayingEpisode().getId(),
                               LastWatchDate(),
                               progress);
}

Player TvShowPlayer::playTvShow(const std::string &tvShowName) {
    auto lastWatchedEpisode = storage.getLastWatchedEpisodeOfTvShow(tvShowName);
    if (lastWatchedEpisode && !lastWatchedEpisode->getWatchProgress().isComplete()) {
        return createPlayer(tvShowName, std::move(*lastWatchedEpisode), false);
    } else if (lastWatchedEpisode) {
        return createPlayer(tvShowName, getEpisodeOrFail(tvShowName, lastWatchedEpisode->getNextEpisodeId()));
    } else {
        return createPlayer(tvShowName, getEpisodeOrFail(tvShowName, 1));
    }
}

Episode TvShowPlayer::getEpisodeOrFail(const std::string &tvShowName, unsigned int episodeId) {
    auto episode = storage.getEpisodeOfTvShowWithName(tvShowName, episodeId);
    if (!episode) {
        throw TvShowEpisodeDoesNotExistError(tvShowName, episodeId);
    } else {
        return *episode;
    }
}

Player TvShowPlayer::createPlayer(const std::string& tvShowName,
                                                            Episode&& episode, bool fromStart) {
    auto nextEpisode = storage.getEpisodeOfTvShowWithName(tvShowName, episode.getNextEpisodeId());
    player = Player(tvShowName, episode, nextEpisode.has_value(), fromStart);
    return *player;
}

Player TvShowPlayer::playPreviousEpisode() {
    return playNextOrPreviousEpisode(false);
}

Player TvShowPlayer::playNextEpisode() {
    return playNextOrPreviousEpisode(true);
}

Player TvShowPlayer::playNextOrPreviousEpisode(bool next) {
    assertEpisodeIsPlaying();
    auto tvShowName = player->getPlayingTvShowName();
    auto episode = player->getPlayingEpisode();
    return createPlayer(tvShowName, getEpisodeOrFail(tvShowName, next ? episode.getNextEpisodeId() : episode.getPreviousEpisodeId()));
}

void TvShowPlayer::assertEpisodeIsPlaying() const {
    if (!player) {
        throw NoTvShowEpisodePlayingError();
    }
}

bool TvShowPlayer::isTvShowWatchComplete(const std::string& tvShowName) {
    auto lastWatchedEpisode = storage.getLastWatchedEpisodeOfTvShow(tvShowName);
    if (!lastWatchedEpisode || !lastWatchedEpisode->getWatchProgress().isComplete()) {
        return false;
    } else {
        return !storage.getEpisodeOfTvShowWithName(tvShowName, lastWatchedEpisode->getNextEpisodeId());
    }
}

TvShowEpisodeDoesNotExistError::TvShowEpisodeDoesNotExistError(const std::string &tvShowName, unsigned int episodeId)
    : std::logic_error("Episode " + std::to_string(episodeId) + " of tv show " + tvShowName + " does not exist") {}

NoTvShowEpisodePlayingError::NoTvShowEpisodePlayingError()
    : std::logic_error("No tv show episode is being played right now") {}

}