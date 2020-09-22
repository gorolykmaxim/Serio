#include <gtest/gtest.h>
#include <tv-show-crawler-runtime/model/CrawlLogEntry.h>
#include <tv-show-crawler-runtime/model/CrawlerStep.h>

class CrawlLogEntryTest : public ::testing::Test {
protected:
    const std::vector<std::string> stepData = {"line 1", "line 2"};
    const serio::core::CrawlerStep value = serio::core::CrawlerStep("value", {{"value", "the value"}});
};

TEST_F(CrawlLogEntryTest, shouldBeASimpleEntryWithoutInputAndOutput) {
    std::string text = "crawl log entry text";
    serio::core::CrawlLogEntry entry(text);
    EXPECT_EQ(text, entry.getText());
    EXPECT_FALSE(entry.hasData());
    EXPECT_TRUE(entry.getStepInputData().empty());
    EXPECT_TRUE(entry.getStepOutputData().empty());
}

TEST_F(CrawlLogEntryTest, shouldHaveDataIfStepInputDataIsNotEmpty) {
    serio::core::CrawlLogEntry entry(value);
    entry.setStepInputData(stepData, 20);
    EXPECT_FALSE(entry.getStepInputData().empty());
    EXPECT_TRUE(entry.getStepOutputData().empty());
    EXPECT_TRUE(entry.hasData());
}

TEST_F(CrawlLogEntryTest, shouldHaveDataIfStepOutputDataIsNotEmpty) {
    serio::core::CrawlLogEntry entry(value);
    entry.setStepOutputData(stepData, 20);
    EXPECT_TRUE(entry.getStepInputData().empty());
    EXPECT_FALSE(entry.getStepOutputData().empty());
    EXPECT_TRUE(entry.hasData());
}

TEST_F(CrawlLogEntryTest, shouldTextDescribingTheExecutedCrawlerStep) {
    serio::core::CrawlLogEntry entry(value);
    EXPECT_EQ("Executing value step with properties: 'value: the value'", entry.getText());
}

TEST_F(CrawlLogEntryTest, shouldHaveSpecifiedInputAndOutputUncut) {
    std::vector<std::string> stepOutputData = {"line 3"};
    serio::core::CrawlLogEntry entry(value);
    entry.setStepInputData(stepData, 20);
    entry.setStepOutputData(stepOutputData, 20);
    EXPECT_EQ("[line 1, line 2]", entry.getStepInputData());
    EXPECT_EQ("[line 3]", entry.getStepOutputData());
}

TEST_F(CrawlLogEntryTest, shouldHaveSpecifiedInputAndOutputCut) {
    std::string cutData = "[line 1, lin...";
    serio::core::CrawlLogEntry entry(value);
    entry.setStepInputData(stepData, 15);
    entry.setStepOutputData(stepData, 15);
    EXPECT_EQ(cutData, entry.getStepInputData());
    EXPECT_EQ(cutData, entry.getStepOutputData());
}

TEST_F(CrawlLogEntryTest, shouldCutInputAndOutputOnComma) {
    std::string cutData = "[line 1,...";
    serio::core::CrawlLogEntry entry(value);
    entry.setStepInputData(stepData, 11);
    entry.setStepOutputData(stepData, 11);
    EXPECT_EQ(cutData, entry.getStepInputData());
    EXPECT_EQ(cutData, entry.getStepOutputData());
}

TEST_F(CrawlLogEntryTest, shouldCutInputAndOutputOnLargeChunk) {
    std::vector<std::string> stepData = {"very large chunk that will be cut"};
    std::string cutData = "[very large chunk...";
    serio::core::CrawlLogEntry entry(value);
    entry.setStepInputData(stepData, 20);
    entry.setStepOutputData(stepData, 20);
    EXPECT_EQ(cutData, entry.getStepInputData());
    EXPECT_EQ(cutData, entry.getStepOutputData());
}

TEST_F(CrawlLogEntryTest, shouldCutInputAndOutputCompletelyLeavingOnlyDots) {
    for (int i: {0, 3}) {
        serio::core::CrawlLogEntry entry(value);
        entry.setStepInputData(stepData, i);
        entry.setStepOutputData(stepData, i);
        EXPECT_EQ("...", entry.getStepInputData());
        EXPECT_EQ("...", entry.getStepInputData());
    }
}
