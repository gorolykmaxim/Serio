#ifndef SERIO_TVSHOWVIEWERMOCK_H
#define SERIO_TVSHOWVIEWERMOCK_H

#include <gmock/gmock.h>
#include <tv-show-viewer/TvShowViewer.h>
#include <TvShowStorageMock.h>
#include "TvShowCrawlerStorageMock.h"

class TvShowViewerMock : public serio::core::TvShowViewer {
public:
    static TvShowViewerMock create() {
        TvShowStorageMock tvShowStorage;
        TvShowCrawlerStorageMock crawlerStorage;
        return TvShowViewerMock(tvShowStorage, crawlerStorage);
    }
    explicit TvShowViewerMock(serio::core::TvShowStorage &tvShowStorage, serio::core::TvShowCrawlerStorage& crawlerStorage)
        : TvShowViewer(tvShowStorage, crawlerStorage) {}
    MOCK_METHOD(serio::core::TvShow, getSelectedTvShow, (), (const override));
    MOCK_METHOD((serio::core::ListPage<serio::core::Episode>), getTvShowEpisodes, (unsigned int, unsigned int), (override));
    MOCK_METHOD(void, openTvShowWithName, (const std::string&), (override));
};

#endif //SERIO_TVSHOWVIEWERMOCK_H
