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
    TvShowCrawlerEditorMock editor = TvShowCrawlerEditorMock::create();
    TvShowCrawlerLogStorageMock logStorage;
    serio::qt::CrawlLogViewModel viewModel = serio::qt::CrawlLogViewModel(editor, logStorage, stack);
    QSignalSpy titleSpy = QSignalSpy(&viewModel, &serio::qt::CrawlLogViewModel::titleChanged);
    QSignalSpy logSpy = QSignalSpy(&viewModel, &serio::qt::CrawlLogViewModel::logChanged);
    virtual void SetUp() {
        serio::core::CrawlLogEntry entry("entry 2");
        const std::vector<std::string> stepData = {"step data 1", "step data 2"};
        entry.setStepInputData(stepData, 100);
        entry.setStepOutputData(stepData, 100);
        expectedLog.push_back(std::move(entry));
    }
    void expectCrawlerPreviewLogViewOpened() {
        EXPECT_CALL(editor, getPreviewedCrawlerLog()).WillOnce(::testing::Return(expectedLog));
        viewModel.openCrawlerPreviewLogView(QVariantList({""}));
    }
    void expectLastCrawlLogViewOpened() {
        EXPECT_CALL(logStorage, getLastCrawlLogOfTvShow(tvShowName.toStdString()))
            .WillOnce(::testing::Return(expectedLog));
        viewModel.openLastCrawlLogOfTvShow(QVariantList({tvShowName}));
    }
    void expectSpecifiedLogDisplayed() {
        QList<serio::qt::TileModel*> log = viewModel.getLog();
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
    EXPECT_CALL(editor, getPreviewedCrawlerLog()).WillRepeatedly(::testing::Return(expectedLog));
    for (const QString& crawlerType: {"Episode Name", "Thumbnail"}) {
        viewModel.openCrawlerPreviewLogView(QVariantList({crawlerType}));
        EXPECT_EQ(crawlerType + " Crawl Log", viewModel.getTitle());
    }
}

TEST_F(CrawlLogViewModelTest, shouldNotifyWatchersAboutTitleChange) {
    expectCrawlerPreviewLogViewOpened();
    EXPECT_EQ(1, titleSpy.count());
}

TEST_F(CrawlLogViewModelTest, shouldPushCrawlLogViewToStack) {
    EXPECT_CALL(stack, pushView(serio::qt::crawlLogView));
    expectCrawlerPreviewLogViewOpened();
}

TEST_F(CrawlLogViewModelTest, shouldNotifyWatchersThatLogHasBeenResetToTheNewOne) {
    expectCrawlerPreviewLogViewOpened();
    EXPECT_EQ(2, logSpy.count());
}

TEST_F(CrawlLogViewModelTest, shouldDisplayCrawlerPreviewExecutionLogAndNotifyWatchers) {
    expectCrawlerPreviewLogViewOpened();
    expectSpecifiedLogDisplayed();
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
    expectCrawlerPreviewLogViewOpened();
    EXPECT_CALL(stack, pushView(serio::qt::crawlLogEntryView));
    viewModel.openLogEntryView(QVariantList({0}));
}

TEST_F(CrawlLogViewModelTest, shouldNotifyWatchersThatSelectedEntryHasChanged) {
    expectCrawlerPreviewLogViewOpened();
    QSignalSpy spy(&viewModel, &serio::qt::CrawlLogViewModel::selectedEntryChanged);
    viewModel.openLogEntryView(QVariantList({0}));
    EXPECT_EQ(1, spy.count());
}

TEST_F(CrawlLogViewModelTest, shouldDisplayEntryWithoutDataWithTheSpecifiedIndexInTheCurrentLog) {
    expectCrawlerPreviewLogViewOpened();
    viewModel.openLogEntryView(QVariantList({0}));
    EXPECT_EQ(expectedLog[0].getText(), viewModel.getSelectedEntryText().toStdString());
    EXPECT_TRUE(viewModel.getSelectedEntryInputData().isEmpty());
    EXPECT_TRUE(viewModel.getSelectedEntryOutputData().isEmpty());
}

TEST_F(CrawlLogViewModelTest, shouldDisplayEntryWithDataWithTheSpecifiedIndexInCurrentLog) {
    expectCrawlerPreviewLogViewOpened();
    expectLogEntryToBeDisplayed(1);
}

TEST_F(CrawlLogViewModelTest, shouldOpenLastCrawlLogOfSpecifiedTvShow) {
    EXPECT_CALL(stack, pushView(serio::qt::crawlLogView));
    expectLastCrawlLogViewOpened();
}

TEST_F(CrawlLogViewModelTest, shouldSetTitleToLastCrawlLog) {
    expectLastCrawlLogViewOpened();
    EXPECT_EQ(QString("Last Crawl Log"), viewModel.getTitle());
}

TEST_F(CrawlLogViewModelTest, shouldNotifyWatchersThatTitleChangedToLastCrawlLog) {
    expectLastCrawlLogViewOpened();
    EXPECT_EQ(1, titleSpy.count());
}

TEST_F(CrawlLogViewModelTest, shouldDisplayLastCrawlLog) {
    expectLastCrawlLogViewOpened();
    expectSpecifiedLogDisplayed();
}

TEST_F(CrawlLogViewModelTest, shouldDiscardOldLastCrawlLogAndDisplayNewOne) {
    EXPECT_CALL(logStorage, getLastCrawlLogOfTvShow(tvShowName.toStdString()))
        .WillRepeatedly(::testing::Return(expectedLog));
    viewModel.openLastCrawlLogOfTvShow(QVariantList({tvShowName}));
    viewModel.openLastCrawlLogOfTvShow(QVariantList({tvShowName}));
    EXPECT_EQ(expectedLog.size(), viewModel.getLog().size());
}

TEST_F(CrawlLogViewModelTest, shouldNotifyWatchersThatLastCrawlLogHasBeenResetToTheNewOne) {
    expectLastCrawlLogViewOpened();
    EXPECT_EQ(2, logSpy.count());
}

TEST_F(CrawlLogViewModelTest, shouldDisplayEntryFromLastCrawlLogWithDataWithTheSpecifiedIndexInCurrentLog) {
    expectLastCrawlLogViewOpened();
    expectLogEntryToBeDisplayed(1);
}