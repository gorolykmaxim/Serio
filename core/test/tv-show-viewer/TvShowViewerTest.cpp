#include <gtest/gtest.h>
#include <TvShowStorageMock.h>
#include <tv-show-viewer/TvShowViewer.h>
#include <TvShowCrawlerStorageMock.h>
#include <TvShowCrawlerRuntimeMock.h>

class TvShowViewerTest : public ::testing::Test {
protected:
    serio::core::TvShow tvShow = serio::core::TvShow("Modern Family");
    ::testing::NiceMock<TvShowStorageMock> tvShowStorage;
    ::testing::NiceMock<TvShowCrawlerStorageMock> crawlerStorage;
    TvShowCrawlerRuntimeMock runtime;
    serio::core::TvShowViewer viewer = serio::core::TvShowViewer(tvShowStorage, crawlerStorage, runtime);
    virtual void SetUp() {
        ON_CALL(tvShowStorage, getTvShowByName(tvShow.getName()))
            .WillByDefault(::testing::Return(std::optional(tvShow)));
    }
};

TEST_F(TvShowViewerTest, shouldFailToReturnSelectedTvShowSinceNoTvShowIsSelected) {
    EXPECT_THROW((void)viewer.getSelectedTvShow(), std::logic_error);
}

TEST_F(TvShowViewerTest, shouldFailToSelectTvShowThatDoesNotExist) {
    ON_CALL(tvShowStorage, getTvShowByName(tvShow.getName()))
        .WillByDefault(::testing::Return(std::optional<serio::core::TvShow>()));
    EXPECT_THROW(viewer.openTvShowWithName(tvShow.getName()), std::logic_error);
    EXPECT_THROW((void)viewer.getSelectedTvShow(), std::logic_error);
}

TEST_F(TvShowViewerTest, shouldViewTvShowWithSpecifiedName) {
    viewer.openTvShowWithName(tvShow.getName());
    EXPECT_EQ(tvShow, viewer.getSelectedTvShow());
}

TEST_F(TvShowViewerTest, shouldFailToGetSelectedTvShowIfItWasDeleted) {
    viewer.openTvShowWithName(tvShow.getName());
    ON_CALL(tvShowStorage, getTvShowByName(tvShow.getName()))
        .WillByDefault(::testing::Return(std::optional<serio::core::TvShow>()));
    EXPECT_THROW((void)viewer.getSelectedTvShow(), std::logic_error);
}

TEST_F(TvShowViewerTest, shouldFailToGetEpisodesOfCurrentlySelectedTvShowSinceNoTvShowIsSelected) {
    EXPECT_THROW((void)viewer.getTvShowEpisodes(0, 0), std::logic_error);
}

TEST_F(TvShowViewerTest, shouldReturnSpecifiedListPageOfEpisodesOfCurrentlySelectedTvShow) {
    viewer.openTvShowWithName(tvShow.getName());
    serio::core::ListPage<serio::core::Episode> page(0, 10, {serio::core::Episode(1, "")});
    ON_CALL(tvShowStorage, getEpisodesOfTvShowWithName(tvShow.getName(), 0, 1))
        .WillByDefault(::testing::Return(page));
    EXPECT_EQ(page, viewer.getTvShowEpisodes(0, 1));
}

TEST_F(TvShowViewerTest, shouldFailToGetCrawlerOfSelectedTvShowSinceNoTvShowIsSelected) {
    EXPECT_THROW(viewer.getRawCrawlerOfSelectedTvShow(), std::logic_error);
}

TEST_F(TvShowViewerTest, shouldFailToGetCrawlerOfSelectedTvShowThatHasBeenAlreadyDeleted) {
    viewer.openTvShowWithName(tvShow.getName());
    ON_CALL(crawlerStorage, getTvShowCrawlerByTvShowName(tvShow.getName()))
        .WillByDefault(::testing::Return(std::optional<std::string>()));
    EXPECT_THROW(viewer.getRawCrawlerOfSelectedTvShow(), std::logic_error);
}

TEST_F(TvShowViewerTest, shouldReturnfCrawlerOfSelectedTvShow) {
    viewer.openTvShowWithName(tvShow.getName());
    std::string rawCrawler = "raw crawler";
    ON_CALL(crawlerStorage, getTvShowCrawlerByTvShowName(tvShow.getName()))
        .WillByDefault(::testing::Return(std::optional(rawCrawler)));
    EXPECT_EQ(rawCrawler, viewer.getRawCrawlerOfSelectedTvShow());
}

TEST_F(TvShowViewerTest, shouldFailToRecrawlSelectedTvShowSinceNoTvShowIsSelected) {
    EXPECT_THROW(viewer.crawlSelectedTvShow(), std::logic_error);
}

TEST_F(TvShowViewerTest, shouldCrawlSelectedTvShow) {
    viewer.openTvShowWithName(tvShow.getName());
    EXPECT_CALL(runtime, crawlTvShow(tvShow.getName()));
    viewer.crawlSelectedTvShow();
}

TEST_F(TvShowViewerTest, shouldFailToClearWatchHistorySinceNoTvShowIsSelected) {
    EXPECT_THROW(viewer.clearSelectedTvShowWatchHistory(), std::logic_error);
}

TEST_F(TvShowViewerTest, shouldClearWatchHistoryOfSelectedTvShow) {
    viewer.openTvShowWithName(tvShow.getName());
    EXPECT_CALL(tvShowStorage, clearTvShowWatchHistory(tvShow.getName()));
    viewer.clearSelectedTvShowWatchHistory();
}

TEST_F(TvShowViewerTest, shouldFailToDeleteTvShowSinceNoTvShowIsSelected) {
    EXPECT_THROW(viewer.deleteSelectedTvShow(), std::logic_error);
}

TEST_F(TvShowViewerTest, shouldDeleteSelectedTvShowAndItsCrawler) {
    viewer.openTvShowWithName(tvShow.getName());
    EXPECT_CALL(tvShowStorage, deleteTvShow(tvShow.getName()));
    EXPECT_CALL(crawlerStorage, deleteCrawlerOfTvShow(tvShow.getName()));
    viewer.deleteSelectedTvShow();
}
