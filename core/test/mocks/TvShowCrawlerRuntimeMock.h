#ifndef SERIO_TVSHOWCRAWLERRUNTIMEMOCK_H
#define SERIO_TVSHOWCRAWLERRUNTIMEMOCK_H

#include <gmock/gmock.h>
#include <tv-show-crawler-runtime/TvShowCrawlerRuntime.h>
#include "HttpClientMock.h"
#include "TvShowStorageMock.h"
#include "TvShowCrawlerStorageMock.h"
#include "TvShowCrawlerLogStorageMock.h"

class TvShowCrawlerRuntimeMock : public serio::core::TvShowCrawlerRuntime {
public:
    static TvShowCrawlerRuntimeMock create() {
        HttpClientMock httpClientMock;
        TvShowStorageMock tvShowStorageMock;
        TvShowCrawlerStorageMock tvShowCrawlerStorageMock;
        TvShowCrawlerLogStorageMock logStorageMock;
        return TvShowCrawlerRuntimeMock(tvShowCrawlerStorageMock, tvShowStorageMock, logStorageMock, httpClientMock);
    }
    TvShowCrawlerRuntimeMock(serio::core::TvShowCrawlerStorage& crawlerStorage, serio::core::TvShowStorage& tvShowStorage,
                             serio::core::TvShowCrawlerLogStorage& logStorage, serio::core::HttpClient& httpClient)
        : TvShowCrawlerRuntime(crawlerStorage, tvShowStorage, logStorage, httpClient) {};
    MOCK_METHOD(void, crawlTvShowAndSaveCrawler, (const serio::core::TvShowCrawler&), (override));
    MOCK_METHOD((std::vector<serio::core::CrawlerStepType>), getCrawlerStepTypes, (), (const override));
    MOCK_METHOD(serio::core::TvShowCrawler, deserializeTvShowCrawler, (const std::string&), (const override));
    MOCK_METHOD(bool, willOverrideExistingTvShow, (const serio::core::TvShowCrawler&), (override));
    MOCK_METHOD(serio::core::CrawlResult, executeCrawler, (const serio::core::Crawler&), (override));
    MOCK_METHOD((std::optional<serio::core::TvShowCrawler>), getTvShowCrawlerByTvShowName, (const std::string&), (override));
    MOCK_METHOD(void, crawlTvShow, (const std::string&), (override));
    MOCK_METHOD(serio::core::TvShowCrawler, getTvShowCrawlerByTvShowNameOrFail, (const std::string&), (override));
};

#endif //SERIO_TVSHOWCRAWLERRUNTIMEMOCK_H
