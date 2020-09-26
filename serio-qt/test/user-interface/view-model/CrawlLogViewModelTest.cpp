#include <gtest/gtest.h>
#include <StackOfViewsMock.h>
#include <TvShowCrawlerEditorMock.h>
#include <user-interface/view-model/CrawlLogViewModel.h>
#include <user-interface/model/TileModel.h>
#include <QSignalSpy>
#include <user-interface/ViewNames.h>

class CrawlLogViewModelTest : public ::testing::Test {
protected:
    std::vector<serio::core::CrawlLogEntry> expectedLog = {serio::core::CrawlLogEntry("entry 1")};
    ::testing::NiceMock<StackOfViewsMock> stack;
    TvShowCrawlerEditorMock editor = TvShowCrawlerEditorMock::create();
    serio::qt::CrawlLogViewModel viewModel = serio::qt::CrawlLogViewModel(editor, stack);
    virtual void SetUp() {
        serio::core::CrawlLogEntry entry("entry 2");
        const std::vector<std::string> stepData = {"step data 1", "step data 2"};
        entry.setStepInputData(stepData, 100);
        entry.setStepOutputData(stepData, 100);
        expectedLog.push_back(std::move(entry));
    }
};

TEST_F(CrawlLogViewModelTest, shouldHaveEmptyLogByDefault) {
    EXPECT_TRUE(viewModel.getLog().isEmpty());
}

TEST_F(CrawlLogViewModelTest, shouldHaveEmptyTitleByDefault) {
    EXPECT_TRUE(viewModel.getTitle().isEmpty());
}

TEST_F(CrawlLogViewModelTest, shouldSetTitleAccordingToTypeOfCrawlerLogsOfWhichAreDisplayed) {
    EXPECT_CALL(editor, getPreviewedCrawlerLog()).WillRepeatedly(::testing::Return(expectedLog));
    for (const QString& crawlerType: {"Episode Name", "Thumbnail"}) {
        viewModel.openCrawlerPreviewLogView(QVariantList({crawlerType}));
        EXPECT_EQ(crawlerType + " Crawl Log", viewModel.getTitle());
    }
}

TEST_F(CrawlLogViewModelTest, shouldNotifyWatchersAboutTitleChange) {
    EXPECT_CALL(editor, getPreviewedCrawlerLog()).WillOnce(::testing::Return(expectedLog));
    QSignalSpy spy(&viewModel, &serio::qt::CrawlLogViewModel::titleChanged);
    viewModel.openCrawlerPreviewLogView(QVariantList({""}));
    EXPECT_EQ(1, spy.count());
}

TEST_F(CrawlLogViewModelTest, shouldPushCrawlLogViewToStack) {
    EXPECT_CALL(editor, getPreviewedCrawlerLog()).WillOnce(::testing::Return(expectedLog));
    EXPECT_CALL(stack, pushView(serio::qt::crawlLogView));
    viewModel.openCrawlerPreviewLogView(QVariantList({""}));
}

TEST_F(CrawlLogViewModelTest, shouldNotifyWatchersThatLogHasBeenResetToTheNewOne) {
    EXPECT_CALL(editor, getPreviewedCrawlerLog()).WillOnce(::testing::Return(expectedLog));
    QSignalSpy spy(&viewModel, &serio::qt::CrawlLogViewModel::logChanged);
    viewModel.openCrawlerPreviewLogView(QVariantList({""}));
    EXPECT_EQ(2, spy.count());
}

TEST_F(CrawlLogViewModelTest, shouldDisplayCrawlerPreviewExecutionLogAndNotifyWatchers) {
    EXPECT_CALL(editor, getPreviewedCrawlerLog()).WillOnce(::testing::Return(expectedLog));
    viewModel.openCrawlerPreviewLogView(QVariantList({""}));
    QList<serio::qt::TileModel*> log = viewModel.getLog();
    for (int i = 0; i < 2; i++) {
        EXPECT_EQ(expectedLog[i].getText(), log[i]->getTitle().toStdString());
        EXPECT_TRUE(log[i]->getSubtitle().isEmpty());
        EXPECT_TRUE(log[i]->getIcon().isEmpty());
    }
}

TEST_F(CrawlLogViewModelTest, shouldDiscardOldLogAndDisplayNewOne) {
    EXPECT_CALL(editor, getPreviewedCrawlerLog()).WillRepeatedly(::testing::Return(expectedLog));
    viewModel.openCrawlerPreviewLogView(QVariantList({""}));
    viewModel.openCrawlerPreviewLogView(QVariantList({""}));
    EXPECT_EQ(expectedLog.size(), viewModel.getLog().size());
}

TEST_F(CrawlLogViewModelTest, shouldDisplayEmptyEntryIfNoEntryIsSelected) {
    EXPECT_TRUE(viewModel.getSelectedEntryText().isEmpty());
    EXPECT_TRUE(viewModel.getSelectedEntryInputData().isEmpty());
    EXPECT_TRUE(viewModel.getSelectedEntryOutputData().isEmpty());
}

TEST_F(CrawlLogViewModelTest, shouldPushCrawlLogEntryViewToStack) {
    EXPECT_CALL(editor, getPreviewedCrawlerLog()).WillOnce(::testing::Return(expectedLog));
    EXPECT_CALL(stack, pushView(serio::qt::crawlLogEntryView));
    viewModel.openLogEntryView(QVariantList({0}));
}

TEST_F(CrawlLogViewModelTest, shouldNotifyWatchersThatSelectedEntryHasChanged) {
    EXPECT_CALL(editor, getPreviewedCrawlerLog()).WillOnce(::testing::Return(expectedLog));
    QSignalSpy spy(&viewModel, &serio::qt::CrawlLogViewModel::selectedEntryChanged);
    viewModel.openLogEntryView(QVariantList({0}));
    EXPECT_EQ(1, spy.count());
}

TEST_F(CrawlLogViewModelTest, shouldDisplayEntryWithoutDataWithTheSpecifiedIndexInTheCurrentLog) {
    EXPECT_CALL(editor, getPreviewedCrawlerLog()).WillOnce(::testing::Return(expectedLog));
    viewModel.openLogEntryView(QVariantList({0}));
    EXPECT_EQ(expectedLog[0].getText(), viewModel.getSelectedEntryText().toStdString());
    EXPECT_TRUE(viewModel.getSelectedEntryInputData().isEmpty());
    EXPECT_TRUE(viewModel.getSelectedEntryOutputData().isEmpty());
}

TEST_F(CrawlLogViewModelTest, shouldDisplayEntryWithDataWithTheSpecifiedIndexInCurrentLog) {
    EXPECT_CALL(editor, getPreviewedCrawlerLog()).WillOnce(::testing::Return(expectedLog));
    viewModel.openLogEntryView(QVariantList({1}));
    EXPECT_EQ(expectedLog[1].getStepInputData(), viewModel.getSelectedEntryInputData().toStdString());
    EXPECT_EQ(expectedLog[1].getStepOutputData(), viewModel.getSelectedEntryOutputData().toStdString());
}
