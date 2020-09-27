#include <gtest/gtest.h>
#include <TvShowStorageMock.h>
#include <tv-show-viewer/TvShowViewer.h>
#include <TvShowCrawlerStorageMock.h>

class TvShowViewerTest : public ::testing::Test {
protected:
    serio::core::TvShow tvShow = serio::core::TvShow("Modern Family");
    TvShowStorageMock tvShowStorage;
    TvShowCrawlerStorageMock crawlerStorage;
    serio::core::TvShowViewer viewer = serio::core::TvShowViewer(tvShowStorage, crawlerStorage);
};

TEST_F(TvShowViewerTest, shouldFailToReturnSelectedTvShowSinceNoTvShowIsSelected) {
    EXPECT_THROW((void)viewer.getSelectedTvShow(), std::logic_error);
}

TEST_F(TvShowViewerTest, shouldFailToSelectTvShowThatDoesNotExist) {
    EXPECT_CALL(tvShowStorage, getTvShowByName(tvShow.getName()))
        .WillOnce(::testing::Return(std::optional<serio::core::TvShow>()));
    EXPECT_THROW(viewer.openTvShowWithName(tvShow.getName()), std::logic_error);
    EXPECT_THROW((void)viewer.getSelectedTvShow(), std::logic_error);
}

TEST_F(TvShowViewerTest, shouldViewTvShowWithSpecifiedName) {
    EXPECT_CALL(tvShowStorage, getTvShowByName(tvShow.getName()))
        .WillRepeatedly(::testing::Return(std::optional(tvShow)));
    viewer.openTvShowWithName(tvShow.getName());
    EXPECT_EQ(tvShow, viewer.getSelectedTvShow());
}

TEST_F(TvShowViewerTest, shouldFailToGetSelectedTvShowIfItWasDeleted) {
    EXPECT_CALL(tvShowStorage, getTvShowByName(tvShow.getName()))
        .WillOnce(::testing::Return(std::optional(tvShow)))
        .WillOnce(::testing::Return(std::optional<serio::core::TvShow>()));
    viewer.openTvShowWithName(tvShow.getName());
    EXPECT_THROW((void)viewer.getSelectedTvShow(), std::logic_error);
}

TEST_F(TvShowViewerTest, shouldFailToGetEpisodesOfCurrentlySelectedTvShowSinceNoTvShowIsSelected) {
    EXPECT_THROW((void)viewer.getTvShowEpisodes(0, 0), std::logic_error);
}

TEST_F(TvShowViewerTest, shouldReturnSpecifiedListPageOfEpisodesOfCurrentlySelectedTvShow) {
    serio::core::ListPage<serio::core::Episode> page(0, 10, {serio::core::Episode(1, "")});
    EXPECT_CALL(tvShowStorage, getTvShowByName(tvShow.getName()))
        .WillOnce(::testing::Return(std::optional(tvShow)));
    EXPECT_CALL(tvShowStorage, getEpisodesOfTvShowWithName(tvShow.getName(), 0, 1))
        .WillOnce(::testing::Return(page));
    viewer.openTvShowWithName(tvShow.getName());
    EXPECT_EQ(page, viewer.getTvShowEpisodes(0, 1));
}

TEST_F(TvShowViewerTest, shouldFailToGetCrawlerOfSelectedTvShowSinceNoTvShowIsSelected) {
    EXPECT_THROW(viewer.getRawCrawlerOfSelectedTvShow(), std::logic_error);
}

TEST_F(TvShowViewerTest, shouldFailToGetCrawlerOfSelectedTvShowThatHasBeenAlreadyDeleted) {
    EXPECT_CALL(tvShowStorage, getTvShowByName(tvShow.getName()))
        .WillOnce(::testing::Return(std::optional(tvShow)));
    EXPECT_CALL(crawlerStorage, getTvShowCrawlerByTvShowName(tvShow.getName()))
        .WillOnce(::testing::Return(std::optional<std::string>()));
    viewer.openTvShowWithName(tvShow.getName());
    EXPECT_THROW(viewer.getRawCrawlerOfSelectedTvShow(), std::logic_error);
}

TEST_F(TvShowViewerTest, shouldReturnfCrawlerOfSelectedTvShow) {
    std::string rawCrawler = "raw crawler";
    EXPECT_CALL(crawlerStorage, getTvShowCrawlerByTvShowName(tvShow.getName()))
        .WillOnce(::testing::Return(std::optional(rawCrawler)));
    EXPECT_CALL(tvShowStorage, getTvShowByName(tvShow.getName()))
            .WillOnce(::testing::Return(std::optional(tvShow)));
    viewer.openTvShowWithName(tvShow.getName());
    EXPECT_EQ(rawCrawler, viewer.getRawCrawlerOfSelectedTvShow());
}
