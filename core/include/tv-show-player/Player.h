#ifndef SERIO_PLAYER_H
#define SERIO_PLAYER_H

#include <tv-show-storage/Episode.h>

namespace serio::core {

class Player {
public:
    Player(std::string playingTvShowName, Episode playingEpisode);
    [[nodiscard]] Episode getPlayingEpisode() const;
    [[nodiscard]] std::string getPlayingTvShowName() const;
private:
    std::string playingTvShowName;
    Episode playingEpisode;
};

}

#endif //SERIO_PLAYER_H
