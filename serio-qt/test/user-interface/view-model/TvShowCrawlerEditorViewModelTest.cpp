#include <gtest/gtest.h>
#include <TvShowCrawlerEditorMock.h>
#include <user-interface/StackOfViews.h>
#include <user-interface/view-model/TvShowCrawlerEditorViewModel.h>
#include <QSignalSpy>
#include <StackOfViewsMock.h>
#include <TvShowViewerMock.h>

class TvShowCrawlerEditorViewModelTest : public ::testing::Test {
protected:
    QString tvShowName = "Friends";
    QString rawCrawler = "raw crawler";
    TvShowCrawlerEditorMock editor = TvShowCrawlerEditorMock::create();
    TvShowViewerMock viewer = TvShowViewerMock::create();
    ::testing::NiceMock<StackOfViewsMock> stack;
    serio::qt::TvShowCrawlerEditorViewModel viewModel = serio::qt::TvShowCrawlerEditorViewModel(editor, viewer, stack);
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
        EXPECT_CALL(editor, getTvShowName()).WillRepeatedly(::testing::Return(tvShowName.toStdString()));
        EXPECT_CALL(editor, willOverrideExistingTvShow()).WillOnce(::testing::Return(!isImported));
        EXPECT_CALL(editor, saveAndRunTvShowCrawler()).Times(isImported ? 1 : 0);
        EXPECT_CALL(stack, pushView(QString("TvShowCrawlerOverrideDialogView.qml"))).Times(isImported ? 0 : 1);
        EXPECT_CALL(viewer, openTvShowWithName(tvShowName.toStdString())).Times(isImported ? 1 : 0);
        viewModel.openImportTvShowCrawlerView();
        viewModel.importTvShowCrawler(QVariantList({rawCrawler}));
    }
    void expectTvShowToBeSavedAndOpenedInViewer() {
        ::testing::InSequence s;
        EXPECT_CALL(editor, saveAndRunTvShowCrawler());
        EXPECT_CALL(viewer, openTvShowWithName(tvShowName.toStdString()));
    }
    void expectToReplaceTheRootViewWithTvShowViewAfterDisplayingCrawlingInProgress(const QString& rootView) {
        ::testing::InSequence s;
        EXPECT_CALL(stack, pushView(QString("CrawlingInProgressView.qml")));
        EXPECT_CALL(stack, replaceSpecifiedViewWith(rootView, QString("TvShowView.qml")));
    }
    void expectSelectedTvShowToBeEdited() {
        EXPECT_CALL(viewer, getSelectedTvShow())
            .WillOnce(::testing::Return(serio::core::TvShow(tvShowName.toStdString())));
        EXPECT_CALL(editor, editTvShowCrawler(tvShowName.toStdString()));
        QSignalSpy canTvShowNameBeChangedSpy(&viewModel, &serio::qt::TvShowCrawlerEditorViewModel::canTvShowNameBeChangedChanged);
        viewModel.openCurrentTvShowCrawlerEditorView();
        EXPECT_FALSE(viewModel.canTvShowNameBeChanged());
        EXPECT_EQ(1, canTvShowNameBeChangedSpy.count());
    }
    void expectTvShowCrawlerOverrideDialogToBeDisplayed() {
        EXPECT_CALL(editor, willOverrideExistingTvShow()).WillOnce(::testing::Return(true));
        EXPECT_CALL(editor, saveAndRunTvShowCrawler()).Times(0);
        EXPECT_CALL(stack, pushView(QString("TvShowCrawlerOverrideDialogView.qml")));
    }
    void expectNewTvShowCrawlerEditorViewToBeOpened() {
        EXPECT_CALL(editor, createTvShowCrawler());
        EXPECT_CALL(stack, replaceCurrentViewWith(QString("TvShowCrawlerEditorView.qml")));
        QSignalSpy canTvShowNameBeChangedSpy(&viewModel, &serio::qt::TvShowCrawlerEditorViewModel::canTvShowNameBeChangedChanged);
        viewModel.openTvShowCrawlerEditorView();
        EXPECT_TRUE(viewModel.canTvShowNameBeChanged());
        EXPECT_EQ(1, canTvShowNameBeChangedSpy.count());
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
    expectNewTvShowCrawlerEditorViewToBeOpened();
}

TEST_F(TvShowCrawlerEditorViewModelTest, shouldOpenTvShowOverrideConfirmationDialog) {
    expectNewTvShowCrawlerEditorViewToBeOpened();
    expectTvShowCrawlerOverrideDialogToBeDisplayed();
    viewModel.save();
}

TEST_F(TvShowCrawlerEditorViewModelTest, shouldSaveNewTvShowCrawler) {
    EXPECT_CALL(editor, getTvShowName()).WillOnce(::testing::Return(tvShowName.toStdString()));
    EXPECT_CALL(editor, willOverrideExistingTvShow()).WillOnce(::testing::Return(false));
    expectTvShowToBeSavedAndOpenedInViewer();
    expectToReplaceTheRootViewWithTvShowViewAfterDisplayingCrawlingInProgress("TvShowCrawlerEditorView.qml");
    expectNewTvShowCrawlerEditorViewToBeOpened();
    viewModel.save();
}

TEST_F(TvShowCrawlerEditorViewModelTest, shouldOverrideExistingTvShowCrawlerWithNewOne) {
    EXPECT_CALL(editor, getTvShowName()).WillOnce(::testing::Return(tvShowName.toStdString()));
    expectTvShowToBeSavedAndOpenedInViewer();
    expectToReplaceTheRootViewWithTvShowViewAfterDisplayingCrawlingInProgress("TvShowCrawlerEditorView.qml");
    expectNewTvShowCrawlerEditorViewToBeOpened();
    viewModel.saveWithOverride();
}

TEST_F(TvShowCrawlerEditorViewModelTest, shouldPopAllViewsUntilTvShowCrawlerEditorEvenIfAnErrorOccursDuringCrawl) {
    EXPECT_CALL(editor, getTvShowName()).WillOnce(::testing::Return(tvShowName.toStdString()));
    EXPECT_CALL(editor, saveAndRunTvShowCrawler()).WillOnce(::testing::Throw(std::runtime_error("error")));
    EXPECT_CALL(stack, popAllViewsUntil(QString("TvShowCrawlerEditorView.qml")));
    expectNewTvShowCrawlerEditorViewToBeOpened();
    EXPECT_THROW(viewModel.saveWithOverride(), std::runtime_error);
}

TEST_F(TvShowCrawlerEditorViewModelTest, shouldPopAllViewsUntilImportTvShowCrawlerEvenIfAnErrorOccursDuringCrawl) {
    EXPECT_CALL(editor, getTvShowName()).WillOnce(::testing::Return(tvShowName.toStdString()));
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
    expectTvShowNameSet();
}

TEST_F(TvShowCrawlerEditorViewModelTest, shouldImportNewTvShowCrawler) {
    expectToReplaceTheRootViewWithTvShowViewAfterDisplayingCrawlingInProgress("ImportTvShowCrawlerView.qml");
    expectTvShowCrawlerToBeImported(true);
    expectTvShowNameSet();
}

TEST_F(TvShowCrawlerEditorViewModelTest, shouldOpenTvShowCrawlerEditorViewToEditCurrentlyViewedTvShow) {
    EXPECT_CALL(stack, pushView(QString("TvShowCrawlerEditorView.qml")));
    expectSelectedTvShowToBeEdited();
}

TEST_F(TvShowCrawlerEditorViewModelTest, shouldOverrideExistingTvShowCrawlerWithoutAskingUserWhenEditingExistingTvShowCrawler) {
    expectSelectedTvShowToBeEdited();
    ON_CALL(editor, willOverrideExistingTvShow()).WillByDefault(::testing::Return(true));
    EXPECT_CALL(editor, getTvShowName()).WillOnce(::testing::Return(tvShowName.toStdString()));
    expectTvShowToBeSavedAndOpenedInViewer();
    expectToReplaceTheRootViewWithTvShowViewAfterDisplayingCrawlingInProgress("TvShowView.qml");
    viewModel.save();
}

TEST_F(TvShowCrawlerEditorViewModelTest, shouldGoBackToEditorViewIfErrorHappensWhileSavingChangesToExistingTvShowCrawler) {
    expectSelectedTvShowToBeEdited();
    EXPECT_CALL(editor, getTvShowName()).WillOnce(::testing::Return(tvShowName.toStdString()));
    EXPECT_CALL(editor, saveAndRunTvShowCrawler())
        .WillOnce(::testing::Throw(std::runtime_error("expect")));
    EXPECT_CALL(stack, popAllViewsUntil(QString("TvShowCrawlerEditorView.qml")));
    EXPECT_THROW(viewModel.save(), std::runtime_error);
}

TEST_F(TvShowCrawlerEditorViewModelTest, shouldOpenTvShowOverrideDialogIfUserEditsExistingShowAndThenAttemptsToCreateNewOneWithTheSameName) {
    expectSelectedTvShowToBeEdited();
    expectNewTvShowCrawlerEditorViewToBeOpened();
    expectTvShowCrawlerOverrideDialogToBeDisplayed();
    viewModel.save();
}

TEST_F(TvShowCrawlerEditorViewModelTest, shouldOpenTvShowOverrideDialogIfUserEditsExistingShowAndThenAttemptsToImportNewOneWithTheSameName) {
    expectSelectedTvShowToBeEdited();
    expectTvShowCrawlerToBeImported(false);
}
