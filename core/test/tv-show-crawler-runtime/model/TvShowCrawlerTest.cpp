#include <gtest/gtest.h>
#include <tv-show-crawler-runtime/model/TvShowCrawler.h>
#include <tv-show-crawler-runtime/model/Crawler.h>

class TvShowCrawlerTest : public ::testing::Test {
protected:
    std::string tvShowName = "Friends";
    serio::core::Crawler crawler;
    serio::core::Crawler nonEmptyCrawler = serio::core::Crawler({
            serio::core::CrawlerStep("value", {{"value", "v"}})
    });
};

TEST_F(TvShowCrawlerTest, shouldCreateTvShowCrawlerWithSpecifiedTvShowName) {
    serio::core::TvShowCrawler tvShowCrawler(tvShowName, crawler, serio::core::Crawler());
    EXPECT_EQ(tvShowName, tvShowCrawler.getTvShowName());
    tvShowCrawler = serio::core::TvShowCrawler("Scrubs", crawler, serio::core::Crawler());
    EXPECT_EQ("Scrubs", tvShowCrawler.getTvShowName());
}

TEST_F(TvShowCrawlerTest, shouldCreateTvShowWithTheSpecifiedEpisodeVideoCrawler) {
    serio::core::TvShowCrawler tvShowCrawler(tvShowName, nonEmptyCrawler, serio::core::Crawler());
    EXPECT_EQ(nonEmptyCrawler, tvShowCrawler.getEpisodeVideoCrawler());
}

TEST_F(TvShowCrawlerTest, shouldReturnDefaultEmptyThumbnailCrawlerIfItWasNotSpecified) {
    serio::core::TvShowCrawler tvShowCrawler(tvShowName, crawler, serio::core::Crawler());
    EXPECT_EQ(crawler, tvShowCrawler.getThumbnailCrawler());
}

TEST_F(TvShowCrawlerTest, shouldReturnSpecifiedThumbnailCrawler) {
    serio::core::TvShowCrawler tvShowCrawler(tvShowName, crawler, nonEmptyCrawler);
    EXPECT_EQ(nonEmptyCrawler, tvShowCrawler.getThumbnailCrawler());
}

TEST_F(TvShowCrawlerTest, shouldReturnDefaultEmptyEpisodeNameCrawlerIfItWasNotSpecified) {
    serio::core::TvShowCrawler tvShowCrawler(tvShowName, crawler, crawler);
    EXPECT_EQ(crawler, tvShowCrawler.getEpisodeNameCrawler());
}

TEST_F(TvShowCrawlerTest, shouldReturnSpecifiedEpisodeNameCrawler) {
    serio::core::TvShowCrawler tvShowCrawler(tvShowName, crawler, crawler, nonEmptyCrawler);
    EXPECT_EQ(nonEmptyCrawler, tvShowCrawler.getEpisodeNameCrawler());
}

TEST_F(TvShowCrawlerTest, twoTvShowCrawlersWithSameTvShowNameAndCrawlersShouldBeEqual) {
    EXPECT_EQ(serio::core::TvShowCrawler(tvShowName, crawler),
              serio::core::TvShowCrawler(tvShowName, crawler));
}

TEST_F(TvShowCrawlerTest, twoTvShowCrawlersWithDifferentTvShowNameOrCrawlersShouldNotBeEqual) {
    serio::core::TvShowCrawler tvShowCrawler(tvShowName, nonEmptyCrawler, nonEmptyCrawler, nonEmptyCrawler);
    EXPECT_NE(serio::core::TvShowCrawler(tvShowName, nonEmptyCrawler, nonEmptyCrawler), tvShowCrawler);
    EXPECT_NE(serio::core::TvShowCrawler(tvShowName, nonEmptyCrawler), tvShowCrawler);
}
