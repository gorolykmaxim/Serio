#include <gtest/gtest.h>
#include <StackOfViewsMock.h>
#include <TvShowCrawlerEditorMock.h>
#include <user-interface/view-model/CrawlLogViewModel.h>
#include <user-interface/model/TileModel.h>
#include <QSignalSpy>
#include <user-interface/ViewNames.h>

class CrawlLogViewModelTest : public ::testing::Test {
protected:
    const QString tvShowName = "How i met your mom";
    std::vector<serio::core::CrawlLogEntry> expectedLog = {serio::core::CrawlLogEntry("entry 1")};
    ::testing::NiceMock<StackOfViewsMock> stack;
    ::testing::NiceMock<TvShowCrawlerEditorMock> editor;
    ::testing::NiceMock<TvShowCrawlerLogStorageMock> logStorage;
    serio::qt::CrawlLogViewModel viewModel = serio::qt::CrawlLogViewModel(editor, logStorage, stack);
    QSignalSpy titleSpy = QSignalSpy(&viewModel, &serio::qt::CrawlLogViewModel::titleChanged);
    QSignalSpy logSpy = QSignalSpy(&viewModel, &serio::qt::CrawlLogViewModel::logChanged);
    virtual void SetUp() {
        serio::core::CrawlLogEntry entry("entry 2");
        const std::vector<std::string> stepData = {"step data 1", "step data 2"};
        entry.setStepInputData(stepData, 100);
        entry.setStepOutputData(stepData, 100);
        expectedLog.push_back(std::move(entry));
        ON_CALL(editor, getPreviewedCrawlerLog()).WillByDefault(::testing::Return(expectedLog));
        ON_CALL(logStorage, getLastCrawlLogOfTvShow(tvShowName.toStdString())).WillByDefault(::testing::Return(expectedLog));
    }
    void expectSpecifiedLogDisplayed() {
        auto log = viewModel.getLog();
        for (int i = 0; i < 2; i++) {
            EXPECT_EQ(expectedLog[i].getText(), log[i]->getTitle().toStdString());
            EXPECT_TRUE(log[i]->getSubtitle().isEmpty());
            EXPECT_TRUE(log[i]->getIcon().isEmpty());
        }
    }
    void expectLogEntryToBeDisplayed(unsigned int logEntryIndex) {
        viewModel.openLogEntryView(QVariantList({logEntryIndex}));
        EXPECT_EQ(expectedLog[logEntryIndex].getStepInputData(), viewModel.getSelectedEntryInputData().toStdString());
        EXPECT_EQ(expectedLog[logEntryIndex].getStepOutputData(), viewModel.getSelectedEntryOutputData().toStdString());
    }
};

TEST_F(CrawlLogViewModelTest, shouldHaveEmptyLogByDefault) {
    EXPECT_TRUE(viewModel.getLog().isEmpty());
}

TEST_F(CrawlLogViewModelTest, shouldHaveEmptyTitleByDefault) {
    EXPECT_TRUE(viewModel.getTitle().isEmpty());
}

TEST_F(CrawlLogViewModelTest, shouldSetTitleAccordingToTypeOfCrawlerLogsOfWhichAreDisplayed) {
    for (const QString& crawlerType: {"Episode Name", "Thumbnail"}) {
        viewModel.openCrawlerPreviewLogView(QVariantList({crawlerType}));
        EXPECT_EQ(crawlerType + " Crawl Log", viewModel.getTitle());
    }
}

TEST_F(CrawlLogViewModelTest, shouldNotifyWatchersAboutTitleChange) {
    viewModel.openCrawlerPreviewLogView(QVariantList({""}));
    EXPECT_EQ(1, titleSpy.count());
}

TEST_F(CrawlLogViewModelTest, shouldPushCrawlLogViewToStack) {
    EXPECT_CALL(stack, pushView(serio::qt::crawlLogView));
    viewModel.openCrawlerPreviewLogView(QVariantList({""}));
}

TEST_F(CrawlLogViewModelTest, shouldNotifyWatchersThatLogHasBeenResetToTheNewOne) {
    viewModel.openCrawlerPreviewLogView(QVariantList({""}));
    EXPECT_EQ(2, logSpy.count());
}

TEST_F(CrawlLogViewModelTest, shouldDisplayCrawlerPreviewExecutionLogAndNotifyWatchers) {
    viewModel.openCrawlerPreviewLogView(QVariantList({""}));
    expectSpecifiedLogDisplayed();
}

TEST_F(CrawlLogViewModelTest, shouldDiscardOldLogAndDisplayNewOne) {
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
    viewModel.openCrawlerPreviewLogView(QVariantList({""}));
    EXPECT_CALL(stack, pushView(serio::qt::crawlLogEntryView));
    viewModel.openLogEntryView(QVariantList({1}));
}

TEST_F(CrawlLogViewModelTest, shouldNotifyWatchersThatSelectedEntryHasChanged) {
    viewModel.openCrawlerPreviewLogView(QVariantList({""}));
    QSignalSpy spy(&viewModel, &serio::qt::CrawlLogViewModel::selectedEntryChanged);
    viewModel.openLogEntryView(QVariantList({1}));
    EXPECT_EQ(1, spy.count());
}

TEST_F(CrawlLogViewModelTest, shouldDisplayEntryWithDataWithTheSpecifiedIndexInCurrentLog) {
    viewModel.openCrawlerPreviewLogView(QVariantList({""}));
    expectLogEntryToBeDisplayed(1);
}

TEST_F(CrawlLogViewModelTest, shouldOpenLastCrawlLogOfSpecifiedTvShow) {
    EXPECT_CALL(stack, pushView(serio::qt::crawlLogView));
    viewModel.openLastCrawlLogOfTvShow(QVariantList({tvShowName}));
}

TEST_F(CrawlLogViewModelTest, shouldSetTitleToLastCrawlLog) {
    viewModel.openLastCrawlLogOfTvShow(QVariantList({tvShowName}));
    EXPECT_EQ(QString("Last Crawl Log"), viewModel.getTitle());
}

TEST_F(CrawlLogViewModelTest, shouldNotifyWatchersThatTitleChangedToLastCrawlLog) {
    viewModel.openLastCrawlLogOfTvShow(QVariantList({tvShowName}));
    EXPECT_EQ(1, titleSpy.count());
}

TEST_F(CrawlLogViewModelTest, shouldDisplayLastCrawlLog) {
    viewModel.openLastCrawlLogOfTvShow(QVariantList({tvShowName}));
    expectSpecifiedLogDisplayed();
}

TEST_F(CrawlLogViewModelTest, shouldDiscardOldLastCrawlLogAndDisplayNewOne) {
    viewModel.openLastCrawlLogOfTvShow(QVariantList({tvShowName}));
    viewModel.openLastCrawlLogOfTvShow(QVariantList({tvShowName}));
    EXPECT_EQ(expectedLog.size(), viewModel.getLog().size());
}

TEST_F(CrawlLogViewModelTest, shouldNotifyWatchersThatLastCrawlLogHasBeenResetToTheNewOne) {
    viewModel.openLastCrawlLogOfTvShow(QVariantList({tvShowName}));
    EXPECT_EQ(2, logSpy.count());
}

TEST_F(CrawlLogViewModelTest, shouldDisplayEntryFromLastCrawlLogWithDataWithTheSpecifiedIndexInCurrentLog) {
    viewModel.openLastCrawlLogOfTvShow(QVariantList({tvShowName}));
    expectLogEntryToBeDisplayed(1);
}

TEST_F(CrawlLogViewModelTest, shouldNotDisplayEntryFromLastCrawlLogWithoutData) {
    viewModel.openLastCrawlLogOfTvShow(QVariantList({tvShowName}));
    EXPECT_CALL(stack, pushView(serio::qt::crawlLogEntryView)).Times(0);
    viewModel.openLogEntryView(QVariantList({0}));
    EXPECT_TRUE(viewModel.getSelectedEntryText().isEmpty());
    EXPECT_TRUE(viewModel.getSelectedEntryInputData().isEmpty());
    EXPECT_TRUE(viewModel.getSelectedEntryOutputData().isEmpty());
}
