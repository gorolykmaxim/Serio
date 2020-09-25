#include <gtest/gtest.h>
#include <TvShowCrawlerEditorMock.h>
#include <user-interface/StackOfViews.h>
#include <user-interface/view-model/TvShowCrawlerEditorViewModel.h>
#include <QSignalSpy>
#include <StackOfViewsMock.h>

class TvShowCrawlerEditorViewModelTest : public ::testing::Test {
protected:
    QString tvShowName = "Friends";
    QString rawCrawler = "raw crawler";
    TvShowCrawlerEditorMock editor = TvShowCrawlerEditorMock::create();
    ::testing::NiceMock<StackOfViewsMock> stack;
    serio::qt::TvShowCrawlerEditorViewModel viewModel = serio::qt::TvShowCrawlerEditorViewModel(editor, stack);
    QSignalSpy tvShowNameSpy = QSignalSpy(&viewModel, &serio::qt::TvShowCrawlerEditorViewModel::tvShowNameChanged);
    QSignalSpy canCrawlerBeSavedSpy = QSignalSpy(&viewModel, &serio::qt::TvShowCrawlerEditorViewModel::canCrawlerBeSavedChanged);
    void expectTvShowNameSet() {
        EXPECT_EQ(tvShowName, viewModel.getTvShowName());
        EXPECT_TRUE(viewModel.canCrawlerBeSaved());
        EXPECT_EQ(1, tvShowNameSpy.count());
        EXPECT_EQ(1, canCrawlerBeSavedSpy.count());
    }
    void expectTvShowCrawlerToBeImported(bool isImported) {
        EXPECT_CALL(editor, importTvShowCrawler(rawCrawler.toStdString()));
        EXPECT_CALL(editor, getTvShowName()).WillOnce(::testing::Return(tvShowName.toStdString()));
        EXPECT_CALL(editor, willOverrideExistingTvShow()).WillOnce(::testing::Return(!isImported));
        EXPECT_CALL(editor, saveAndRunTvShowCrawler()).Times(isImported ? 1 : 0);
    }
    void expectEveryViewToPopAfterPushingCrawlingInProgressView() {
        ::testing::InSequence s;
        EXPECT_CALL(stack, pushView(QString("CrawlingInProgressView.qml")));
        EXPECT_CALL(stack, popAllViews());
    }
};

TEST_F(TvShowCrawlerEditorViewModelTest, shouldReturnEmptyTvShowNameByDefault) {
    EXPECT_EQ(QString(), viewModel.getTvShowName());
    EXPECT_FALSE(viewModel.canCrawlerBeSaved());
}

TEST_F(TvShowCrawlerEditorViewModelTest, shouldSetTvShowNameInViewModeAndEditor) {
    EXPECT_CALL(editor, setTvShowName(tvShowName.toStdString()));
    viewModel.setTvShowName(QVariantList({tvShowName}));
    expectTvShowNameSet();
}

TEST_F(TvShowCrawlerEditorViewModelTest, shouldLoadTvShowFromCrawlerEditor) {
    EXPECT_CALL(editor, getTvShowName()).WillOnce(::testing::Return(tvShowName.toStdString()));
    viewModel.loadTvShowName();
    expectTvShowNameSet();
}

TEST_F(TvShowCrawlerEditorViewModelTest, shouldOpenAddTvShowView) {
    EXPECT_CALL(stack, pushView(QString("AddTvShowView.qml")));
    viewModel.openAddTvShowView();
}

TEST_F(TvShowCrawlerEditorViewModelTest, shouldOpenTvShowCrawlerEditorView) {
    EXPECT_CALL(editor, createTvShowCrawler());
    EXPECT_CALL(stack, replaceCurrentViewWith(QString("TvShowCrawlerEditorView.qml")));
    viewModel.openTvShowCrawlerEditorView();
}

TEST_F(TvShowCrawlerEditorViewModelTest, shouldOpenTvShowOverrideConfirmationDialog) {
    EXPECT_CALL(editor, willOverrideExistingTvShow()).WillOnce(::testing::Return(true));
    EXPECT_CALL(editor, saveAndRunTvShowCrawler()).Times(0);
    EXPECT_CALL(stack, pushView(QString("TvShowCrawlerOverrideDialogView.qml")));
    viewModel.save();
}

TEST_F(TvShowCrawlerEditorViewModelTest, shouldSaveNewTvShowCrawler) {
    EXPECT_CALL(editor, willOverrideExistingTvShow()).WillOnce(::testing::Return(false));
    EXPECT_CALL(editor, saveAndRunTvShowCrawler());
    expectEveryViewToPopAfterPushingCrawlingInProgressView();
    viewModel.save();
}

TEST_F(TvShowCrawlerEditorViewModelTest, shouldOverrideExistingTvShowCrawlerWithNewOne) {
    EXPECT_CALL(editor, saveAndRunTvShowCrawler());
    expectEveryViewToPopAfterPushingCrawlingInProgressView();
    viewModel.saveWithOverride();
}

TEST_F(TvShowCrawlerEditorViewModelTest, shouldPopAllViewsUntilTvShowCrawlerEditorEvenIfAnErrorOccursDuringCrawl) {
    EXPECT_CALL(editor, createTvShowCrawler());
    EXPECT_CALL(editor, saveAndRunTvShowCrawler()).WillOnce(::testing::Throw(std::runtime_error("error")));
    EXPECT_CALL(stack, popAllViewsUntil(QString("TvShowCrawlerEditorView.qml")));
    viewModel.openTvShowCrawlerEditorView();
    EXPECT_THROW(viewModel.saveWithOverride(), std::runtime_error);
}

TEST_F(TvShowCrawlerEditorViewModelTest, shouldPopAllViewsUntilImportTvShowCrawlerEvenIfAnErrorOccursDuringCrawl) {
    EXPECT_CALL(editor, saveAndRunTvShowCrawler()).WillOnce(::testing::Throw(std::runtime_error("error")));
    EXPECT_CALL(stack, popAllViewsUntil(QString("ImportTvShowCrawlerView.qml")));
    viewModel.openImportTvShowCrawlerView();
    EXPECT_THROW(viewModel.saveWithOverride(), std::runtime_error);
}

TEST_F(TvShowCrawlerEditorViewModelTest, shouldOpenImportTvShowCrawlerView) {
    EXPECT_CALL(stack, replaceCurrentViewWith(QString("ImportTvShowCrawlerView.qml")));
    viewModel.openImportTvShowCrawlerView();
}

TEST_F(TvShowCrawlerEditorViewModelTest, shouldOpenTvShowOverrideConfirmationDialogWhileImportingExistingTvShow) {
    expectTvShowCrawlerToBeImported(false);
    EXPECT_CALL(stack, pushView(QString("TvShowCrawlerOverrideDialogView.qml")));
    viewModel.importTvShowCrawler(QVariantList({rawCrawler}));
    expectTvShowNameSet();
}

TEST_F(TvShowCrawlerEditorViewModelTest, shouldImportNewTvShowCrawler) {
    expectTvShowCrawlerToBeImported(true);
    expectEveryViewToPopAfterPushingCrawlingInProgressView();
    viewModel.importTvShowCrawler(QVariantList({rawCrawler}));
    expectTvShowNameSet();
}