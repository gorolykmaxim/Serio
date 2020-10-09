#ifndef SERIO_PLAYER_H
#define SERIO_PLAYER_H

#include <tv-show-storage/Episode.h>

namespace serio::core {

class Player {
public:
    Player(std::string playingTvShowName, Episode playingEpisode, bool nextEpisode, bool fromStart = true);
    [[nodiscard]] Episode getPlayingEpisode() const;
    [[nodiscard]] std::string getPlayingTvShowName() const;
    [[nodiscard]] double getStartPercentage() const;
    [[nodiscard]] bool hasPreviousEpisode() const;
    [[nodiscard]] bool hasNextEpisode() const;
private:
    bool fromStart;
    bool nextEpisode;
    std::string playingTvShowName;
    Episode playingEpisode;
};

}

#endif //SERIO_PLAYER_H
