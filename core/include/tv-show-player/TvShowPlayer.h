#ifndef SERIO_TVSHOWPLAYER_H
#define SERIO_TVSHOWPLAYER_H

#include <tv-show-storage/TvShowStorage.h>
#include "Player.h"

namespace serio::core {

class TvShowPlayer {
public:
    explicit TvShowPlayer(TvShowStorage& storage);
    [[nodiscard]] virtual Player playEpisodeOfTvShow(const std::string &tvShowName, unsigned int episodeId);
    virtual void updatePlayingEpisodeWatchProgress(WatchProgress progress);
    [[nodiscard]] virtual Player playTvShow(const std::string& tvShowName);
    [[nodiscard]] virtual Player playPreviousEpisode();
    [[nodiscard]] virtual Player playNextEpisode();
    [[nodiscard]] virtual bool isTvShowWatchComplete(const std::string& tvShowName);
private:
    std::optional<Player> player;
    TvShowStorage& storage;
    void assertEpisodeIsPlaying() const;
    [[nodiscard]] Episode getEpisodeOrFail(const std::string& tvShowName, unsigned int episodeId);
    [[nodiscard]] Player createPlayer(const std::string& tvShowName, serio::core::Episode&& episode, bool fromStart = true);
    [[nodiscard]] Player playNextOrPreviousEpisode(bool next);
};

class TvShowEpisodeDoesNotExistError : public std::logic_error {
public:
    TvShowEpisodeDoesNotExistError(const std::string& tvShowName, unsigned int episodeId);
};

class NoTvShowEpisodePlayingError : public std::logic_error {
public:
    NoTvShowEpisodePlayingError();
};

}

#endif //SERIO_TVSHOWPLAYER_H
