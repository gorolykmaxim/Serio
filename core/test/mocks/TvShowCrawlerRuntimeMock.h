#ifndef SERIO_TVSHOWCRAWLERRUNTIMEMOCK_H
#define SERIO_TVSHOWCRAWLERRUNTIMEMOCK_H

#include <gmock/gmock.h>
#include <tv-show-crawler-runtime/TvShowCrawlerRuntime.h>
#include "HttpClientMock.h"
#include "TvShowStorageMock.h"
#include "TvShowCrawlerStorageMock.h"
#include "TvShowCrawlerLogStorageMock.h"

class TvShowCrawlerRuntimeMock : public serio::core::TvShowCrawlerRuntime {
    inline static HttpClientMock httpClientMock;
    inline static TvShowStorageMock tvShowStorageMock;
    inline static TvShowCrawlerStorageMock tvShowCrawlerStorageMock;
    inline static TvShowCrawlerLogStorageMock tvShowCrawlerLogStorageMock;
public:
    static TvShowCrawlerRuntimeMock create() {
        return TvShowCrawlerRuntimeMock();
    }
    TvShowCrawlerRuntimeMock()
        : TvShowCrawlerRuntime(tvShowCrawlerStorageMock, tvShowStorageMock, tvShowCrawlerLogStorageMock, httpClientMock) {};
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
