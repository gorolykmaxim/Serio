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
private:
    std::optional<Player> player;
    TvShowStorage& storage;
    [[nodiscard]] Episode getEpisodeOrFail(const std::string& tvShowName, unsigned int episodeId);
    [[nodiscard]] Player setPlayer(Player&& newPlayer);
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
