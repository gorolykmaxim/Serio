#include <gtest/gtest.h>
#include <database-storage/DatabaseStorage.h>

class DatabaseTvShowCrawlerStorageTest : public ::testing::Test {
protected:
    serio::qt::DatabaseStorage storage;
    virtual void SetUp() {
        storage.initialize(":memory:");
    }
};

TEST_F(DatabaseTvShowCrawlerStorageTest, shouldNotFindCrawlerOfTvShowThatDoesNotExist) {
    EXPECT_FALSE(storage.getTvShowCrawlerByTvShowName("Friends"));
}

TEST_F(DatabaseTvShowCrawlerStorageTest, shouldOverrideExistingTvShowCrawlerWithNewOne) {
    std::string expectedCrawler = "{\"showName\":\"Friends\",\"episodeVideoCrawler\":{}}";
    storage.saveTvShowCrawler("Friends", "{}");
    storage.saveTvShowCrawler("Friends", expectedCrawler);
    EXPECT_EQ(expectedCrawler, *storage.getTvShowCrawlerByTvShowName("Friends"));
}
