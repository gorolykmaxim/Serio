#ifndef SERIO_TVSHOWCRAWLERRUNTIMEMOCK_H
#define SERIO_TVSHOWCRAWLERRUNTIMEMOCK_H

#include <gmock/gmock.h>
#include <tv-show-crawler-runtime/TvShowCrawlerRuntime.h>
#include "HttpClientMock.h"
#include "TvShowStorageMock.h"
#include "TvShowCrawlerStorageMock.h"

class TvShowCrawlerRuntimeMock : public serio::core::TvShowCrawlerRuntime {
public:
    static TvShowCrawlerRuntimeMock create() {
        HttpClientMock httpClientMock;
        TvShowStorageMock tvShowStorageMock;
        TvShowCrawlerStorageMock tvShowCrawlerStorageMock;
        return TvShowCrawlerRuntimeMock(tvShowCrawlerStorageMock, tvShowStorageMock, httpClientMock);
    }
    TvShowCrawlerRuntimeMock(serio::core::TvShowCrawlerStorage& crawlerStorage, serio::core::TvShowStorage& tvShowStorage, serio::core::HttpClient& httpClient) : TvShowCrawlerRuntime(crawlerStorage, tvShowStorage, httpClient) {};
    MOCK_METHOD(void, crawlTvShowAndSaveCrawler, (const serio::core::TvShowCrawler&), (override));
    MOCK_METHOD((std::vector<serio::core::CrawlerStepType>), getCrawlerStepTypes, (), (const override));
    MOCK_METHOD(bool, willOverrideExistingTvShow, (const serio::core::TvShowCrawler&), (override));
};

#endif //SERIO_TVSHOWCRAWLERRUNTIMEMOCK_H
