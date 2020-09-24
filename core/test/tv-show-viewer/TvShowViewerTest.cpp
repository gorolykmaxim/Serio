#include <gtest/gtest.h>
#include <TvShowStorageMock.h>
#include <tv-show-viewer/TvShowViewer.h>

class TvShowViewerTest : public ::testing::Test {
protected:
    serio::core::TvShow tvShow = serio::core::TvShow("Modern Family");
    TvShowStorageMock storage;
    serio::core::TvShowViewer viewer = serio::core::TvShowViewer(storage);
};

TEST_F(TvShowViewerTest, shouldFailToReturnSelectedTvShowSinceNoTvShowIsSelected) {
    EXPECT_THROW((void)viewer.getSelectedTvShow(), std::logic_error);
}

TEST_F(TvShowViewerTest, shouldViewTvShowWithSpecifiedName) {
    EXPECT_CALL(storage, getTvShowByName(tvShow.getName()))
        .WillOnce(::testing::Return(std::optional(tvShow)));
    viewer.openTvShowWithName(tvShow.getName());
    EXPECT_EQ(tvShow, viewer.getSelectedTvShow());
}

TEST_F(TvShowViewerTest, shouldFailToGetEpisodesOfCurrentlySelectedTvShowSinceNoTvShowIsSelected) {
    EXPECT_THROW((void)viewer.getTvShowEpisodes(0, 0), std::logic_error);
}

TEST_F(TvShowViewerTest, shouldReturnSpecifiedListPageOfEpisodesOfCurrentlySelectedTvShow) {
    serio::core::ListPage<serio::core::Episode> page(0, 10, {serio::core::Episode(1, "")});
    EXPECT_CALL(storage, getTvShowByName(tvShow.getName()))
            .WillOnce(::testing::Return(std::optional(tvShow)));
    EXPECT_CALL(storage, getEpisodesOfTvShowWithName(tvShow.getName(), 0, 1))
        .WillOnce(::testing::Return(page));
    viewer.openTvShowWithName(tvShow.getName());
    EXPECT_EQ(page, viewer.getTvShowEpisodes(0, 1));
}
