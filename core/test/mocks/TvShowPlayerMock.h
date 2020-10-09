#ifndef SERIO_TVSHOWPLAYERMOCK_H
#define SERIO_TVSHOWPLAYERMOCK_H

#include <gmock/gmock.h>
#include <tv-show-player/TvShowPlayer.h>
#include "TvShowStorageMock.h"

class TvShowPlayerMock : public serio::core::TvShowPlayer {
    inline static TvShowStorageMock storage;
public:
    TvShowPlayerMock() : TvShowPlayer(storage) {};
    MOCK_METHOD(serio::core::Player, playEpisodeOfTvShow, (const std::string&, unsigned int), (override));
    MOCK_METHOD(void, updatePlayingEpisodeWatchProgress, (serio::core::WatchProgress), (override));
    MOCK_METHOD(serio::core::Player, playTvShow, (const std::string&), (override));
    MOCK_METHOD(serio::core::Player, playPreviousEpisode, (), (override));
    MOCK_METHOD(serio::core::Player, playNextEpisode, (), (override));
    MOCK_METHOD(bool, isTvShowWatchComplete, (const std::string&), (override));
};

#endif //SERIO_TVSHOWPLAYERMOCK_H
