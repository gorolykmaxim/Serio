#ifndef SERIO_PLAYER_H
#define SERIO_PLAYER_H

#include <tv-show-storage/Episode.h>

namespace serio::core {

class Player {
public:
    Player(std::string playingTvShowName, Episode playingEpisode, bool fromStart = true);
    [[nodiscard]] Episode getPlayingEpisode() const;
    [[nodiscard]] std::string getPlayingTvShowName() const;
    [[nodiscard]] double getStartPercentage() const;
private:
    bool fromStart;
    std::string playingTvShowName;
    Episode playingEpisode;
};

}

#endif //SERIO_PLAYER_H
