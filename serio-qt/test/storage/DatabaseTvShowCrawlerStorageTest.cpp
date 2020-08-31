#include <gtest/gtest.h>
#include <storage/DatabaseStorage.h>

TEST(DatabaseTvShowCrawlerStorageTest, shouldNotFindCrawlerOfTvShowThatDoesNotExist) {
    serio::qt::DatabaseStorage storage;
    storage.initialize(":memory:");
    EXPECT_FALSE(storage.getTvShowCrawlerByTvShowName("Friends"));
}

TEST(DatabaseTvShowCrawlerStorageTest, shouldOverrideExistingTvShowCrawlerWithNewOne) {
    serio::qt::DatabaseStorage storage;
    storage.initialize(":memory:");
    std::string expectedCrawler = "{\"showName\":\"Friends\",\"episodeVideoCrawler\":{}}";
    storage.saveTvShowCrawler("Friends", "{}");
    storage.saveTvShowCrawler("Friends", expectedCrawler);
    EXPECT_EQ(expectedCrawler, *storage.getTvShowCrawlerByTvShowName("Friends"));
}
