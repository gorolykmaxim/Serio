#ifndef SERIO_TVSHOWVIEWERMOCK_H
#define SERIO_TVSHOWVIEWERMOCK_H

#include <gmock/gmock.h>
#include <tv-show-viewer/TvShowViewer.h>
#include <TvShowStorageMock.h>

class TvShowViewerMock : public serio::core::TvShowViewer {
public:
    static TvShowViewerMock create() {
        TvShowStorageMock storage;
        return TvShowViewerMock(storage);
    }
    explicit TvShowViewerMock(serio::core::TvShowStorage &storage) : TvShowViewer(storage) {}
    MOCK_METHOD(serio::core::TvShow, getSelectedTvShow, (), (const override));
    MOCK_METHOD((serio::core::ListPage<serio::core::Episode>), getTvShowEpisodes, (unsigned int, unsigned int), (override));
    MOCK_METHOD(void, openTvShowWithName, (const std::string&), (override));
};

#endif //SERIO_TVSHOWVIEWERMOCK_H
