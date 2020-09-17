#include <gtest/gtest.h>
#include <TvShowCrawlerEditorMock.h>
#include <user-interface/StackOfViews.h>
#include <user-interface/view-model/TvShowCrawlerEditorViewModel.h>
#include <QSignalSpy>

class TvShowCrawlerEditorViewModelTest : public ::testing::Test {
protected:
    QString tvShowName = "Friends";
    TvShowCrawlerEditorMock editor = TvShowCrawlerEditorMock::create();
    serio::qt::StackOfViews stack;
    serio::qt::TvShowCrawlerEditorViewModel viewModel = serio::qt::TvShowCrawlerEditorViewModel(editor, stack);
    QSignalSpy tvShowNameSpy = QSignalSpy(&viewModel, &serio::qt::TvShowCrawlerEditorViewModel::tvShowNameChanged);
    QSignalSpy canCrawlerBeSavedSpy = QSignalSpy(&viewModel, &serio::qt::TvShowCrawlerEditorViewModel::canCrawlerBeSavedChanged);
    QSignalSpy stackPushSpy = QSignalSpy(&stack, &serio::qt::StackOfViews::push);
    QSignalSpy stackPopSpy = QSignalSpy(&stack, &serio::qt::StackOfViews::pop);
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

TEST_F(TvShowCrawlerEditorViewModelTest, shouldOpenTvShowCrawlerEditorView) {
    EXPECT_CALL(editor, createTvShowCrawler());
    viewModel.openView();
    expectViewToBePushedToStack("views/TvShowCrawlerEditorView.qml");
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
