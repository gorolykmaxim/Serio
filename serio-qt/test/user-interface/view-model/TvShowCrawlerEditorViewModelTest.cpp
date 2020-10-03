#include <gtest/gtest.h>
#include <TvShowCrawlerEditorMock.h>
#include <user-interface/StackOfViews.h>
#include <user-interface/view-model/TvShowCrawlerEditorViewModel.h>
#include <QSignalSpy>
#include <StackOfViewsMock.h>
#include <TvShowViewerMock.h>
#include <user-interface/ViewNames.h>
#include <DialogViewModelMock.h>
#include <user-interface/model/ButtonModel.h>

class TvShowCrawlerEditorViewModelTest : public ::testing::Test {
protected:
    QString tvShowName = "Friends";
    QString rawCrawler = "raw crawler";
    TvShowCrawlerEditorMock editor = TvShowCrawlerEditorMock::create();
    TvShowViewerMock viewer = TvShowViewerMock::create();
    ::testing::NiceMock<StackOfViewsMock> stack;
    DialogViewModelMock dialog;
    serio::qt::DialogModel dialogModel = serio::qt::DialogModel(
            "TV Show Already Exists",
            "Are you sure you want to override crawler of the '" + tvShowName + "' with the new one?");
    serio::qt::TvShowCrawlerEditorViewModel viewModel = serio::qt::TvShowCrawlerEditorViewModel(editor, viewer, dialog, stack);
    QSignalSpy tvShowNameSpy = QSignalSpy(&viewModel, &serio::qt::TvShowCrawlerEditorViewModel::tvShowNameChanged);
    QSignalSpy canCrawlerBeSavedSpy = QSignalSpy(&viewModel, &serio::qt::TvShowCrawlerEditorViewModel::canCrawlerBeSavedChanged);
    void SetUp() override {
        dialogModel.setTopButtonAction(serio::qt::ActionType::SAVE_TV_SHOW_CRAWLER_WITH_OVERRIDE);
    }
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
        EXPECT_CALL(dialog, display(dialogModel)).Times(isImported ? 0 : 1);
        EXPECT_CALL(viewer, openTvShowWithName(tvShowName.toStdString())).Times(isImported ? 1 : 0);
        viewModel.openImportTvShowCrawlerView();
        viewModel.setRawCrawlerToImport(rawCrawler);
        viewModel.importTvShowCrawler();
    }
    void expectTvShowToBeSavedAndOpenedInViewer(bool isNewTvShow) {
        ::testing::InSequence s;
        EXPECT_CALL(editor, saveAndRunTvShowCrawler());
        EXPECT_CALL(viewer, openTvShowWithName(tvShowName.toStdString())).Times(isNewTvShow ? 1 : 0);
    }
    void expectToReplaceTheRootViewWithTvShowViewAfterDisplayingCrawlingInProgress(const QString& rootView) {
        ::testing::InSequence s;
        EXPECT_CALL(stack, pushView(serio::qt::crawlingInProgressView));
        EXPECT_CALL(stack, replaceSpecifiedViewWith(rootView, serio::qt::tvShowView));
    }
    void expectToPopAllViewsUntilTvShowViewAfterDisplayingCrawlingInProgress() {
        ::testing::InSequence s;
        EXPECT_CALL(stack, pushView(serio::qt::crawlingInProgressView));
        EXPECT_CALL(stack, popAllViewsUntil(serio::qt::tvShowView));
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
        EXPECT_CALL(dialog, display(dialogModel));
    }
    void expectNewTvShowCrawlerEditorViewToBeOpened() {
        EXPECT_CALL(editor, createTvShowCrawler());
        EXPECT_CALL(stack, replaceCurrentViewWith(serio::qt::tvShowCrawlerEditorView));
        QSignalSpy canTvShowNameBeChangedSpy(&viewModel, &serio::qt::TvShowCrawlerEditorViewModel::canTvShowNameBeChangedChanged);
        viewModel.openTvShowCrawlerEditorView();
        EXPECT_TRUE(viewModel.canTvShowNameBeChanged());
        EXPECT_EQ(1, canTvShowNameBeChangedSpy.count());
    }
    void expectTvShowNameToBeLoaded() {
        EXPECT_CALL(editor, getTvShowName()).WillOnce(::testing::Return(tvShowName.toStdString()));
        viewModel.loadTvShowName();
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
    expectTvShowNameToBeLoaded();
    expectTvShowNameSet();
}

TEST_F(TvShowCrawlerEditorViewModelTest, shouldOpenAddTvShowView) {
    EXPECT_CALL(stack, pushView(serio::qt::addTvShowView));
    viewModel.openAddTvShowView();
}

TEST_F(TvShowCrawlerEditorViewModelTest, shouldOpenTvShowCrawlerEditorView) {
    expectNewTvShowCrawlerEditorViewToBeOpened();
}

TEST_F(TvShowCrawlerEditorViewModelTest, shouldOpenTvShowOverrideConfirmationDialog) {
    expectNewTvShowCrawlerEditorViewToBeOpened();
    expectTvShowNameToBeLoaded();
    expectTvShowCrawlerOverrideDialogToBeDisplayed();
    viewModel.save();
}

TEST_F(TvShowCrawlerEditorViewModelTest, shouldSaveNewTvShowCrawler) {
    EXPECT_CALL(editor, getTvShowName()).WillOnce(::testing::Return(tvShowName.toStdString()));
    EXPECT_CALL(editor, willOverrideExistingTvShow()).WillOnce(::testing::Return(false));
    expectTvShowToBeSavedAndOpenedInViewer(true);
    expectToReplaceTheRootViewWithTvShowViewAfterDisplayingCrawlingInProgress(serio::qt::tvShowCrawlerEditorView);
    expectNewTvShowCrawlerEditorViewToBeOpened();
    viewModel.save();
}

TEST_F(TvShowCrawlerEditorViewModelTest, shouldOverrideExistingTvShowCrawlerWithNewOne) {
    EXPECT_CALL(editor, getTvShowName()).WillOnce(::testing::Return(tvShowName.toStdString()));
    expectTvShowToBeSavedAndOpenedInViewer(true);
    expectToReplaceTheRootViewWithTvShowViewAfterDisplayingCrawlingInProgress(serio::qt::tvShowCrawlerEditorView);
    expectNewTvShowCrawlerEditorViewToBeOpened();
    viewModel.saveWithOverride();
}

TEST_F(TvShowCrawlerEditorViewModelTest, shouldPopAllViewsUntilTvShowCrawlerEditorEvenIfAnErrorOccursDuringCrawl) {
    EXPECT_CALL(editor, getTvShowName()).WillOnce(::testing::Return(tvShowName.toStdString()));
    EXPECT_CALL(editor, saveAndRunTvShowCrawler()).WillOnce(::testing::Throw(std::runtime_error("error")));
    EXPECT_CALL(stack, popAllViewsUntil(serio::qt::tvShowCrawlerEditorView));
    expectNewTvShowCrawlerEditorViewToBeOpened();
    EXPECT_THROW(viewModel.saveWithOverride(), std::runtime_error);
}

TEST_F(TvShowCrawlerEditorViewModelTest, shouldPopAllViewsUntilImportTvShowCrawlerEvenIfAnErrorOccursDuringCrawl) {
    EXPECT_CALL(editor, getTvShowName()).WillOnce(::testing::Return(tvShowName.toStdString()));
    EXPECT_CALL(editor, saveAndRunTvShowCrawler()).WillOnce(::testing::Throw(std::runtime_error("error")));
    EXPECT_CALL(stack, popAllViewsUntil(serio::qt::importTvShowCrawlerView));
    viewModel.openImportTvShowCrawlerView();
    EXPECT_THROW(viewModel.saveWithOverride(), std::runtime_error);
}

TEST_F(TvShowCrawlerEditorViewModelTest, shouldOpenImportTvShowCrawlerView) {
    EXPECT_CALL(stack, replaceCurrentViewWith(serio::qt::importTvShowCrawlerView));
    viewModel.openImportTvShowCrawlerView();
}

TEST_F(TvShowCrawlerEditorViewModelTest, shouldOpenTvShowOverrideConfirmationDialogWhileImportingExistingTvShow) {
    expectTvShowCrawlerToBeImported(false);
    expectTvShowNameSet();
}

TEST_F(TvShowCrawlerEditorViewModelTest, shouldImportNewTvShowCrawler) {
    expectToReplaceTheRootViewWithTvShowViewAfterDisplayingCrawlingInProgress(serio::qt::importTvShowCrawlerView);
    expectTvShowCrawlerToBeImported(true);
}

TEST_F(TvShowCrawlerEditorViewModelTest, shouldOpenTvShowCrawlerEditorViewToEditCurrentlyViewedTvShow) {
    EXPECT_CALL(stack, pushView(serio::qt::tvShowCrawlerEditorView));
    expectSelectedTvShowToBeEdited();
}

TEST_F(TvShowCrawlerEditorViewModelTest, shouldOverrideExistingTvShowCrawlerWithoutAskingUserWhenEditingExistingTvShowCrawler) {
    expectSelectedTvShowToBeEdited();
    ON_CALL(editor, willOverrideExistingTvShow()).WillByDefault(::testing::Return(true));
    EXPECT_CALL(editor, getTvShowName()).WillOnce(::testing::Return(tvShowName.toStdString()));
    expectTvShowToBeSavedAndOpenedInViewer(false);
    expectToPopAllViewsUntilTvShowViewAfterDisplayingCrawlingInProgress();
    viewModel.save();
}

TEST_F(TvShowCrawlerEditorViewModelTest, shouldGoBackToEditorViewIfErrorHappensWhileSavingChangesToExistingTvShowCrawler) {
    expectSelectedTvShowToBeEdited();
    EXPECT_CALL(editor, getTvShowName()).WillOnce(::testing::Return(tvShowName.toStdString()));
    EXPECT_CALL(editor, saveAndRunTvShowCrawler())
        .WillOnce(::testing::Throw(std::runtime_error("expect")));
    EXPECT_CALL(stack, popAllViewsUntil(serio::qt::tvShowCrawlerEditorView));
    EXPECT_THROW(viewModel.save(), std::runtime_error);
}

TEST_F(TvShowCrawlerEditorViewModelTest, shouldOpenTvShowOverrideDialogIfUserEditsExistingShowAndThenAttemptsToCreateNewOneWithTheSameName) {
    expectSelectedTvShowToBeEdited();
    expectNewTvShowCrawlerEditorViewToBeOpened();
    expectTvShowNameToBeLoaded();
    expectTvShowCrawlerOverrideDialogToBeDisplayed();
    viewModel.save();
}

TEST_F(TvShowCrawlerEditorViewModelTest, shouldOpenTvShowOverrideDialogIfUserEditsExistingShowAndThenAttemptsToImportNewOneWithTheSameName) {
    expectSelectedTvShowToBeEdited();
    expectTvShowCrawlerToBeImported(false);
}

TEST_F(TvShowCrawlerEditorViewModelTest, shouldResetRawCrawlerToImportAfterReopeningImportCrawlerView) {
    EXPECT_CALL(editor, importTvShowCrawler("")).WillOnce(::testing::Throw(std::runtime_error("")));
    viewModel.setRawCrawlerToImport(rawCrawler);
    viewModel.openImportTvShowCrawlerView();
    EXPECT_THROW(viewModel.importTvShowCrawler(), std::runtime_error);
}

TEST_F(TvShowCrawlerEditorViewModelTest, shouldReturnListOfImportCrawlerActions) {
    QList<serio::qt::ButtonModel*> actions = viewModel.getImportTvShowCrawlerActions();
    EXPECT_EQ(*actions[0], serio::qt::ButtonModel("cancel", serio::qt::ActionType::BACK));
    EXPECT_EQ(*actions[1], serio::qt::ButtonModel("import", serio::qt::ActionType::IMPORT_TV_SHOW_CRAWLER));
}
