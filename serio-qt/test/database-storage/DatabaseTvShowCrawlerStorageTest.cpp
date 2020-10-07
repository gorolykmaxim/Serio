#include <gtest/gtest.h>
#include <database-storage/DatabaseStorage.h>

class DatabaseTvShowCrawlerStorageTest : public ::testing::Test {
protected:
    const std::string tvShowName = "Friends";
    const std::string dummyCrawler = "{}";
    serio::qt::DatabaseStorage storage;
    virtual void SetUp() {
        storage.initialize(true);
    }
};

TEST_F(DatabaseTvShowCrawlerStorageTest, shouldNotFindCrawlerOfTvShowThatDoesNotExist) {
    EXPECT_FALSE(storage.getTvShowCrawlerByTvShowName(tvShowName));
}

TEST_F(DatabaseTvShowCrawlerStorageTest, shouldOverrideExistingTvShowCrawlerWithNewOne) {
    auto expectedCrawler = R"({"showName":"Friends","episodeVideoCrawler":{}})";
    storage.saveTvShowCrawler(tvShowName, dummyCrawler);
    storage.saveTvShowCrawler(tvShowName, expectedCrawler);
    EXPECT_EQ(expectedCrawler, *storage.getTvShowCrawlerByTvShowName(tvShowName));
}

TEST_F(DatabaseTvShowCrawlerStorageTest, shouldDeleteCrawlerOfTvShow) {
    storage.saveTvShowCrawler(tvShowName, dummyCrawler);
    storage.deleteCrawlerOfTvShow(tvShowName);
    EXPECT_FALSE(storage.getTvShowCrawlerByTvShowName(tvShowName));
}
