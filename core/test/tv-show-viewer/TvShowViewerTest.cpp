#include <gtest/gtest.h>
#include <TvShowStorageMock.h>
#include <tv-show-viewer/TvShowViewer.h>
#include <TvShowCrawlerStorageMock.h>
#include <TvShowCrawlerRuntimeMock.h>

class TvShowViewerTest : public ::testing::Test {
protected:
    serio::core::TvShow tvShow = serio::core::TvShow("Modern Family");
    TvShowStorageMock tvShowStorage;
    TvShowCrawlerStorageMock crawlerStorage;
    TvShowCrawlerRuntimeMock runtime = TvShowCrawlerRuntimeMock::create();
    serio::core::TvShowViewer viewer = serio::core::TvShowViewer(tvShowStorage, crawlerStorage, runtime);
    void expectTvShowToBeSelected() {
        EXPECT_CALL(tvShowStorage, getTvShowByName(tvShow.getName()))
            .WillOnce(::testing::Return(std::optional(tvShow)));
        viewer.openTvShowWithName(tvShow.getName());
    }
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
    expectTvShowToBeSelected();
    EXPECT_CALL(tvShowStorage, getTvShowByName(tvShow.getName()))
        .WillOnce(::testing::Return(std::optional<serio::core::TvShow>()));
    EXPECT_THROW((void)viewer.getSelectedTvShow(), std::logic_error);
}

TEST_F(TvShowViewerTest, shouldFailToGetEpisodesOfCurrentlySelectedTvShowSinceNoTvShowIsSelected) {
    EXPECT_THROW((void)viewer.getTvShowEpisodes(0, 0), std::logic_error);
}

TEST_F(TvShowViewerTest, shouldReturnSpecifiedListPageOfEpisodesOfCurrentlySelectedTvShow) {
    expectTvShowToBeSelected();
    serio::core::ListPage<serio::core::Episode> page(0, 10, {serio::core::Episode(1, "")});
    EXPECT_CALL(tvShowStorage, getEpisodesOfTvShowWithName(tvShow.getName(), 0, 1))
        .WillOnce(::testing::Return(page));
    EXPECT_EQ(page, viewer.getTvShowEpisodes(0, 1));
}

TEST_F(TvShowViewerTest, shouldFailToGetCrawlerOfSelectedTvShowSinceNoTvShowIsSelected) {
    EXPECT_THROW(viewer.getRawCrawlerOfSelectedTvShow(), std::logic_error);
}

TEST_F(TvShowViewerTest, shouldFailToGetCrawlerOfSelectedTvShowThatHasBeenAlreadyDeleted) {
    expectTvShowToBeSelected();
    EXPECT_CALL(crawlerStorage, getTvShowCrawlerByTvShowName(tvShow.getName()))
        .WillOnce(::testing::Return(std::optional<std::string>()));
    EXPECT_THROW(viewer.getRawCrawlerOfSelectedTvShow(), std::logic_error);
}

TEST_F(TvShowViewerTest, shouldReturnfCrawlerOfSelectedTvShow) {
    expectTvShowToBeSelected();
    std::string rawCrawler = "raw crawler";
    EXPECT_CALL(crawlerStorage, getTvShowCrawlerByTvShowName(tvShow.getName()))
        .WillOnce(::testing::Return(std::optional(rawCrawler)));
    EXPECT_EQ(rawCrawler, viewer.getRawCrawlerOfSelectedTvShow());
}

TEST_F(TvShowViewerTest, shouldFailToRecrawlSelectedTvShowSinceNoTvShowIsSelected) {
    EXPECT_THROW(viewer.crawlSelectedTvShow(), std::logic_error);
}

TEST_F(TvShowViewerTest, shouldCrawlSelectedTvShow) {
    expectTvShowToBeSelected();
    EXPECT_CALL(runtime, crawlTvShow(tvShow.getName()));
    viewer.crawlSelectedTvShow();
}

TEST_F(TvShowViewerTest, shouldFailToClearWatchHistorySinceNoTvShowIsSelected) {
    EXPECT_THROW(viewer.clearSelectedTvShowWatchHistory(), std::logic_error);
}

TEST_F(TvShowViewerTest, shouldClearWatchHistoryOfSelectedTvShow) {
    expectTvShowToBeSelected();
    EXPECT_CALL(tvShowStorage, clearTvShowWatchHistory(tvShow.getName()));
    viewer.clearSelectedTvShowWatchHistory();
}

TEST_F(TvShowViewerTest, shouldFailToDeleteTvShowSinceNoTvShowIsSelected) {
    EXPECT_THROW(viewer.deleteSelectedTvShow(), std::logic_error);
}

TEST_F(TvShowViewerTest, shouldDeleteSelectedTvShowAndItsCrawler) {
    expectTvShowToBeSelected();
    EXPECT_CALL(tvShowStorage, deleteTvShow(tvShow.getName()));
    EXPECT_CALL(crawlerStorage, deleteCrawlerOfTvShow(tvShow.getName()));
    viewer.deleteSelectedTvShow();
}
