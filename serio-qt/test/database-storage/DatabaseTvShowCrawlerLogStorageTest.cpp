#include <gtest/gtest.h>
#include <database-storage/DatabaseStorage.h>

class DatabaseTvShowCrawlerLogStorageTest : public ::testing::Test {
protected:
    const unsigned int maxCrawlLogEntryDataSize = 100;
    serio::qt::DatabaseStorage storage;
    virtual void SetUp() {
        storage.initialize(true);
    }
    std::vector<serio::core::CrawlLogEntry> generateThumbnailCrawlLog(const std::string& thumbnailUrl) {
        std::vector<serio::core::CrawlLogEntry> crawlLog;
        crawlLog.emplace_back("Crawling thumbnail");
        crawlLog.emplace_back(serio::core::CrawlerStep("value", {{"value", thumbnailUrl}}));
        crawlLog.back().setStepInputData({}, maxCrawlLogEntryDataSize);
        crawlLog.back().setStepOutputData({thumbnailUrl}, maxCrawlLogEntryDataSize);
        return crawlLog;
    }
};

TEST_F(DatabaseTvShowCrawlerLogStorageTest, shouldReturnLatestSavedCrawlLogOfTvShow) {
    std::string tvShowName = "The Big Bang Theory";
    std::vector<serio::core::CrawlLogEntry> crawlLog = generateThumbnailCrawlLog("https://tv-show.com/thumbnail.jpg");
    storage.saveCrawlLog(tvShowName, crawlLog);
    EXPECT_EQ(crawlLog, storage.getLastCrawlLogOfTvShow(tvShowName));
    crawlLog = generateThumbnailCrawlLog("https://tv-show.com/thumbnail-4k.jpg");
    storage.saveCrawlLog(tvShowName, crawlLog);
    EXPECT_EQ(crawlLog, storage.getLastCrawlLogOfTvShow(tvShowName));
}