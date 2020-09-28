#ifndef SERIO_TVSHOWSTORAGEMOCK_H
#define SERIO_TVSHOWSTORAGEMOCK_H

#include <gmock/gmock.h>
#include <tv-show-storage/TvShowStorage.h>

class TvShowStorageMock : public serio::core::TvShowStorage {
public:
    MOCK_METHOD((std::optional<serio::core::TvShow>), getTvShowByName, (const std::string&), (override));
    MOCK_METHOD((serio::core::ListPage<serio::core::TvShow>), getAllTvShows, (unsigned int, unsigned int), (override));
    MOCK_METHOD((serio::core::ListPage<serio::core::TvShow>), getWatchedTvShows, (unsigned int, unsigned int), (override));
    MOCK_METHOD((serio::core::ListPage<serio::core::Episode>), getEpisodesOfTvShowWithName, (const std::string&, unsigned int, unsigned int), (override));
    MOCK_METHOD(void, saveTvShow, (const serio::core::TvShow&, const std::vector<serio::core::Episode>&), (override));
    MOCK_METHOD(void, clearTvShowWatchHistory, (const std::string&), (override));
};

#endif //SERIO_TVSHOWSTORAGEMOCK_H
