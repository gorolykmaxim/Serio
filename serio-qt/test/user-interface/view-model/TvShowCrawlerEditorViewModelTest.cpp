#include <gtest/gtest.h>
#include <TvShowCrawlerEditorMock.h>
#include <user-interface/StackOfViews.h>
#include <user-interface/view-model/TvShowCrawlerEditorViewModel.h>
#include <QSignalSpy>

class TvShowCrawlerEditorViewModelTest : public ::testing::Test {
protected:
    QString tvShowName = "Friends";
    QString rawCrawler = "raw crawler";
    TvShowCrawlerEditorMock editor = TvShowCrawlerEditorMock::create();
    serio::qt::StackOfViews stack;
    serio::qt::TvShowCrawlerEditorViewModel viewModel = serio::qt::TvShowCrawlerEditorViewModel(editor, stack);
    QSignalSpy tvShowNameSpy = QSignalSpy(&viewModel, &serio::qt::TvShowCrawlerEditorViewModel::tvShowNameChanged);
    QSignalSpy canCrawlerBeSavedSpy = QSignalSpy(&viewModel, &serio::qt::TvShowCrawlerEditorViewModel::canCrawlerBeSavedChanged);
    QSignalSpy stackPushSpy = QSignalSpy(&stack, &serio::qt::StackOfViews::push);
    QSignalSpy stackPopSpy = QSignalSpy(&stack, &serio::qt::StackOfViews::pop);
    QSignalSpy stackReplaceSpy = QSignalSpy(&stack, &serio::qt::StackOfViews::replace);
    void expectTvShowNameSet() {
        EXPECT_EQ(tvShowName, viewModel.getTvShowName());
        EXPECT_TRUE(viewModel.canCrawlerBeSaved());
        EXPECT_EQ(1, tvShowNameSpy.count());
        EXPECT_EQ(1, canCrawlerBeSavedSpy.count());
    }
    void expectViewToBePushedToStack(QString viewName) {
        ASSERT_EQ(1, stackPushSpy.count());
        QVariantList args = stackPushSpy.takeFirst();
        EXPECT_EQ(QStringList({std::move(viewName)}), args[0].toStringList());
    }
    void expectAllViewsToBePoppedFromStack() {
        ASSERT_EQ(1, stackPopSpy.count());
        QVariantList args = stackPopSpy.takeFirst();
        EXPECT_FALSE(args[0].toBool());
    }
    void expectCurrentViewToBeReplacedWith(const QString& newView) {
        ASSERT_EQ(1, stackReplaceSpy.count());
        QVariantList args = stackReplaceSpy.takeFirst();
        EXPECT_EQ(newView, args[0].toString());
    }
    void expectTvShowCrawlerToBeImported(bool isImported) {
        EXPECT_CALL(editor, importTvShowCrawler(rawCrawler.toStdString()));
        EXPECT_CALL(editor, getTvShowName()).WillOnce(::testing::Return(tvShowName.toStdString()));
        EXPECT_CALL(editor, willOverrideExistingTvShow()).WillOnce(::testing::Return(!isImported));
        EXPECT_CALL(editor, saveAndRunTvShowCrawler()).Times(isImported ? 1 : 0);
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
    viewModel.openAddTvShowView();
    expectViewToBePushedToStack("views/AddTvShowView.qml");
}

TEST_F(TvShowCrawlerEditorViewModelTest, shouldOpenTvShowCrawlerEditorView) {
    EXPECT_CALL(editor, createTvShowCrawler());
    viewModel.openTvShowCrawlerEditorView();
    expectCurrentViewToBeReplacedWith("views/TvShowCrawlerEditorView.qml");
}

TEST_F(TvShowCrawlerEditorViewModelTest, shouldOpenTvShowOverrideConfirmationDialog) {
    EXPECT_CALL(editor, willOverrideExistingTvShow()).WillOnce(::testing::Return(true));
    EXPECT_CALL(editor, saveAndRunTvShowCrawler()).Times(0);
    viewModel.save();
    expectViewToBePushedToStack("views/TvShowCrawlerOverrideDialogView.qml");
}

TEST_F(TvShowCrawlerEditorViewModelTest, shouldSaveNewTvShowCrawler) {
    EXPECT_CALL(editor, willOverrideExistingTvShow()).WillOnce(::testing::Return(false));
    EXPECT_CALL(editor, saveAndRunTvShowCrawler());
    viewModel.save();
    expectViewToBePushedToStack("views/CrawlingInProgressView.qml");
    expectAllViewsToBePoppedFromStack();
}

TEST_F(TvShowCrawlerEditorViewModelTest, shouldOverrideExistingTvShowCrawlerWithNewOne) {
    EXPECT_CALL(editor, saveAndRunTvShowCrawler());
    viewModel.saveWithOverride();
    expectViewToBePushedToStack("views/CrawlingInProgressView.qml");
    expectAllViewsToBePoppedFromStack();
}

TEST_F(TvShowCrawlerEditorViewModelTest, shouldPopCrawlingInProgressViewEvenIfAnErrorOccursDuringCrawl) {
    EXPECT_CALL(editor, saveAndRunTvShowCrawler()).WillOnce(::testing::Throw(std::runtime_error("error")));
    EXPECT_THROW(viewModel.saveWithOverride(), std::runtime_error);
    ASSERT_EQ(1, stackPopSpy.count());
    QVariantList args = stackPopSpy.takeFirst();
    EXPECT_TRUE(args[0].toBool());
}

TEST_F(TvShowCrawlerEditorViewModelTest, shouldOpenImportTvShowCrawlerView) {
    viewModel.openImportTvShowCrawlerView();
    expectCurrentViewToBeReplacedWith("views/ImportTvShowCrawlerView.qml");
}

TEST_F(TvShowCrawlerEditorViewModelTest, shouldOpenTvShowOverrideConfirmationDialogWhileImportingExistingTvShow) {
    expectTvShowCrawlerToBeImported(false);
    viewModel.importTvShowCrawler(QVariantList({rawCrawler}));
    expectTvShowNameSet();
    expectViewToBePushedToStack("views/TvShowCrawlerOverrideDialogView.qml");
}

TEST_F(TvShowCrawlerEditorViewModelTest, shouldImportNewTvShowCrawler) {
    expectTvShowCrawlerToBeImported(true);
    viewModel.importTvShowCrawler(QVariantList({rawCrawler}));
    expectTvShowNameSet();
    expectViewToBePushedToStack("views/CrawlingInProgressView.qml");
    expectAllViewsToBePoppedFromStack();
}