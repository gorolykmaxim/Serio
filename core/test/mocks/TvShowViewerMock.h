#ifndef SERIO_TVSHOWVIEWERMOCK_H
#define SERIO_TVSHOWVIEWERMOCK_H

#include <gmock/gmock.h>
#include <tv-show-viewer/TvShowViewer.h>
#include <TvShowStorageMock.h>
#include "TvShowCrawlerStorageMock.h"
#include "TvShowCrawlerRuntimeMock.h"

class TvShowViewerMock : public serio::core::TvShowViewer {
    inline static TvShowStorageMock tvShowStorageMock;
    inline static TvShowCrawlerStorageMock tvShowCrawlerStorageMock;
    inline static TvShowCrawlerRuntimeMock runtimeMock;
public:
    TvShowViewerMock() : TvShowViewer(tvShowStorageMock, tvShowCrawlerStorageMock, runtimeMock) {}
    MOCK_METHOD(serio::core::TvShow, getSelectedTvShow, (), (const override));
    MOCK_METHOD((serio::core::ListPage<serio::core::Episode>), getTvShowEpisodes, (unsigned int, unsigned int), (override));
    MOCK_METHOD(void, openTvShowWithName, (const std::string&), (override));
    MOCK_METHOD(void, crawlSelectedTvShow, (), (override));
    MOCK_METHOD(void, clearSelectedTvShowWatchHistory, (), (override));
    MOCK_METHOD(void, deleteSelectedTvShow, (), (override));
};

#endif //SERIO_TVSHOWVIEWERMOCK_H
