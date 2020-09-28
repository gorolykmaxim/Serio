#ifndef SERIO_TVSHOWVIEWERMOCK_H
#define SERIO_TVSHOWVIEWERMOCK_H

#include <gmock/gmock.h>
#include <tv-show-viewer/TvShowViewer.h>
#include <TvShowStorageMock.h>
#include "TvShowCrawlerStorageMock.h"
#include "TvShowCrawlerRuntimeMock.h"

class TvShowViewerMock : public serio::core::TvShowViewer {
public:
    static TvShowViewerMock create() {
        TvShowStorageMock tvShowStorage;
        TvShowCrawlerStorageMock crawlerStorage;
        TvShowCrawlerRuntimeMock runtime = TvShowCrawlerRuntimeMock::create();
        return TvShowViewerMock(tvShowStorage, crawlerStorage, runtime);
    }
    explicit TvShowViewerMock(serio::core::TvShowStorage &tvShowStorage, serio::core::TvShowCrawlerStorage& crawlerStorage,
                              serio::core::TvShowCrawlerRuntime& runtime)
        : TvShowViewer(tvShowStorage, crawlerStorage, runtime) {}
    MOCK_METHOD(serio::core::TvShow, getSelectedTvShow, (), (const override));
    MOCK_METHOD((serio::core::ListPage<serio::core::Episode>), getTvShowEpisodes, (unsigned int, unsigned int), (override));
    MOCK_METHOD(void, openTvShowWithName, (const std::string&), (override));
    MOCK_METHOD(void, crawlSelectedTvShow, (), (override));
    MOCK_METHOD(void, clearSelectedTvShowWatchHistory, (), (override));
};

#endif //SERIO_TVSHOWVIEWERMOCK_H
