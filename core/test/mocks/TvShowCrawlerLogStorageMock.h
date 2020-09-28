#ifndef SERIO_TVSHOWCRAWLERLOGSTORAGEMOCK_H
#define SERIO_TVSHOWCRAWLERLOGSTORAGEMOCK_H

#include <gmock/gmock.h>
#include <tv-show-crawler-log-storage/TvShowCrawlerLogStorage.h>

class TvShowCrawlerLogStorageMock : public serio::core::TvShowCrawlerLogStorage {
public:
    MOCK_METHOD(void, saveCrawlLog, (const std::string&, const std::vector<serio::core::CrawlLogEntry>&), (override));
    MOCK_METHOD((std::vector<serio::core::CrawlLogEntry>), getLastCrawlLogOfTvShow, (const std::string&), (override));
};

#endif //SERIO_TVSHOWCRAWLERLOGSTORAGEMOCK_H
