#include <gtest/gtest.h>
#include <tv-show-crawler-runtime/model/Crawler.h>

class CrawlerTest : public ::testing::Test {
protected:
    std::vector<serio::core::CrawlerStep> steps = {
            serio::core::CrawlerStep("value", {{"value", "v"}}),
            serio::core::CrawlerStep("fetch")
    };
};

TEST_F(CrawlerTest, shouldHaveNotSteps) {
    serio::core::Crawler crawler;
    EXPECT_FALSE(crawler.hasSteps());
}

TEST_F(CrawlerTest, shouldHaveSpecifiedSteps) {
    serio::core::Crawler crawler(steps);
    EXPECT_TRUE(crawler.hasSteps());
    EXPECT_EQ(steps, crawler.getSteps());
}

TEST_F(CrawlerTest, twoCrawlersWithSameStepsShouldBeEqual) {
    EXPECT_EQ(serio::core::Crawler(steps), serio::core::Crawler(steps));
}

TEST_F(CrawlerTest, twoCrawlersWithDifferentStepsShouldNotBeEqual) {
    EXPECT_NE(serio::core::Crawler(steps), serio::core::Crawler());
}
