#ifndef SERIO_TVSHOWCRAWLERSTORAGEMOCK_H
#define SERIO_TVSHOWCRAWLERSTORAGEMOCK_H

#include <gmock/gmock.h>
#include <tv-show-crawler-storage/TvShowCrawlerStorage.h>

class TvShowCrawlerStorageMock : public serio::core::TvShowCrawlerStorage {
public:
    MOCK_METHOD((std::optional<std::string>), getTvShowCrawlerByTvShowName, (const std::string&), (override));
    MOCK_METHOD(void, saveTvShowCrawler, (const std::string&, const std::string&), (override));
};

#endif //SERIO_TVSHOWCRAWLERSTORAGEMOCK_H
