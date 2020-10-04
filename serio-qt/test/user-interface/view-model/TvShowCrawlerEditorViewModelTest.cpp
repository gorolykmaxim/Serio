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
    ::testing::NiceMock<TvShowCrawlerEditorMock> editor;
    ::testing::NiceMock<TvShowViewerMock> viewer;
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
        ON_CALL(editor, getTvShowName()).WillByDefault(::testing::Return(tvShowName.toStdString()));
        ON_CALL(editor, willOverrideExistingTvShow()).WillByDefault(::testing::Return(false));
        ON_CALL(viewer, getSelectedTvShow()).WillByDefault(::testing::Return(serio::core::TvShow(tvShowName.toStdString())));
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
    void expectTvShowCrawlerOverrideDialogToBeDisplayed() {
        ON_CALL(editor, willOverrideExistingTvShow()).WillByDefault(::testing::Return(true));
        EXPECT_CALL(editor, saveAndRunTvShowCrawler()).Times(0);
        EXPECT_CALL(dialog, display(dialogModel));
    }
};

TEST_F(TvShowCrawlerEditorViewModelTest, shouldReturnEmptyTvShowNameByDefault) {
    EXPECT_EQ(QString(), viewModel.getTvShowName());
    EXPECT_FALSE(viewModel.canCrawlerBeSaved());
}

TEST_F(TvShowCrawlerEditorViewModelTest, shouldSetTvShowNameInViewModelAndEditor) {
    EXPECT_CALL(editor, setTvShowName(tvShowName.toStdString()));
    viewModel.setTvShowName(QVariantList({tvShowName}));
    EXPECT_EQ(tvShowName, viewModel.getTvShowName());
}

TEST_F(TvShowCrawlerEditorViewModelTest, shouldNotifyWatchersAboutTvShowNameChange) {
    viewModel.setTvShowName(QVariantList({tvShowName}));
    EXPECT_EQ(1, tvShowNameSpy.count());
}

TEST_F(TvShowCrawlerEditorViewModelTest, shouldAllowSavingTvShowCrawlerAfterSettingNonEmptyTvShowName) {
    viewModel.setTvShowName(QVariantList({tvShowName}));
    EXPECT_TRUE(viewModel.canCrawlerBeSaved());
}

TEST_F(TvShowCrawlerEditorViewModelTest, shouldNotifyWatchersAboutCrawlerSaveabilityStateChange) {
    viewModel.setTvShowName(QVariantList({tvShowName}));
    EXPECT_EQ(1, canCrawlerBeSavedSpy.count());
}

TEST_F(TvShowCrawlerEditorViewModelTest, shouldLoadTvShowFromCrawlerEditor) {
    viewModel.loadTvShowName();
    EXPECT_EQ(tvShowName, viewModel.getTvShowName());
}

TEST_F(TvShowCrawlerEditorViewModelTest, shouldNotifyWatchersAboutTvShowNameChangeAfterLoad) {
    viewModel.loadTvShowName();
    EXPECT_EQ(1, tvShowNameSpy.count());
}

TEST_F(TvShowCrawlerEditorViewModelTest, shouldAllowSavingTvShowCrawlerAfterLoadingNonEmptyTvShowName) {
    viewModel.loadTvShowName();
    EXPECT_TRUE(viewModel.canCrawlerBeSaved());
}

TEST_F(TvShowCrawlerEditorViewModelTest, shouldNotifyWatchersAboutCrawlerSaveabilityStateChangeAfterLoad) {
    viewModel.loadTvShowName();
    EXPECT_EQ(1, canCrawlerBeSavedSpy.count());
}

TEST_F(TvShowCrawlerEditorViewModelTest, shouldOpenAddTvShowView) {
    EXPECT_CALL(stack, pushView(serio::qt::addTvShowView));
    viewModel.openAddTvShowView();
}

TEST_F(TvShowCrawlerEditorViewModelTest, shouldOpenTvShowCrawlerEditorView) {
    EXPECT_CALL(editor, createTvShowCrawler());
    EXPECT_CALL(stack, replaceCurrentViewWith(serio::qt::tvShowCrawlerEditorView));
    QSignalSpy canTvShowNameBeChangedSpy(&viewModel, &serio::qt::TvShowCrawlerEditorViewModel::canTvShowNameBeChangedChanged);
    viewModel.openTvShowCrawlerEditorView();
    EXPECT_TRUE(viewModel.canTvShowNameBeChanged());
    EXPECT_EQ(1, canTvShowNameBeChangedSpy.count());
}

TEST_F(TvShowCrawlerEditorViewModelTest, shouldOpenTvShowOverrideConfirmationDialog) {
    expectTvShowCrawlerOverrideDialogToBeDisplayed();
    viewModel.openTvShowCrawlerEditorView();
    viewModel.loadTvShowName();
    viewModel.save();
}

TEST_F(TvShowCrawlerEditorViewModelTest, shouldSaveNewTvShowCrawler) {
    ON_CALL(editor, willOverrideExistingTvShow()).WillByDefault(::testing::Return(false));
    expectTvShowToBeSavedAndOpenedInViewer(true);
    expectToReplaceTheRootViewWithTvShowViewAfterDisplayingCrawlingInProgress(serio::qt::tvShowCrawlerEditorView);
    viewModel.openTvShowCrawlerEditorView();
    viewModel.save();
}

TEST_F(TvShowCrawlerEditorViewModelTest, shouldOverrideExistingTvShowCrawlerWithNewOne) {
    expectTvShowToBeSavedAndOpenedInViewer(true);
    expectToReplaceTheRootViewWithTvShowViewAfterDisplayingCrawlingInProgress(serio::qt::tvShowCrawlerEditorView);
    viewModel.openTvShowCrawlerEditorView();
    viewModel.saveWithOverride();
}

TEST_F(TvShowCrawlerEditorViewModelTest, shouldPopAllViewsUntilTvShowCrawlerEditorEvenIfAnErrorOccursDuringCrawl) {
    ON_CALL(editor, saveAndRunTvShowCrawler()).WillByDefault(::testing::Throw(std::runtime_error("error")));
    EXPECT_CALL(stack, popAllViewsUntil(serio::qt::tvShowCrawlerEditorView));
    viewModel.openTvShowCrawlerEditorView();
    EXPECT_THROW(viewModel.saveWithOverride(), std::runtime_error);
}

TEST_F(TvShowCrawlerEditorViewModelTest, shouldPopAllViewsUntilImportTvShowCrawlerEvenIfAnErrorOccursDuringCrawl) {
    ON_CALL(editor, saveAndRunTvShowCrawler()).WillByDefault(::testing::Throw(std::runtime_error("error")));
    EXPECT_CALL(stack, popAllViewsUntil(serio::qt::importTvShowCrawlerView));
    viewModel.openImportTvShowCrawlerView();
    EXPECT_THROW(viewModel.saveWithOverride(), std::runtime_error);
}

TEST_F(TvShowCrawlerEditorViewModelTest, shouldOpenImportTvShowCrawlerView) {
    EXPECT_CALL(stack, replaceCurrentViewWith(serio::qt::importTvShowCrawlerView));
    viewModel.openImportTvShowCrawlerView();
}

TEST_F(TvShowCrawlerEditorViewModelTest, shouldOpenTvShowOverrideConfirmationDialogWhileImportingExistingTvShow) {
    expectTvShowCrawlerOverrideDialogToBeDisplayed();
    viewModel.openImportTvShowCrawlerView();
    viewModel.setRawCrawlerToImport(rawCrawler);
    viewModel.importTvShowCrawler();
}

TEST_F(TvShowCrawlerEditorViewModelTest, shouldImportNewTvShowCrawler) {
    expectToReplaceTheRootViewWithTvShowViewAfterDisplayingCrawlingInProgress(serio::qt::importTvShowCrawlerView);
    EXPECT_CALL(editor, importTvShowCrawler(rawCrawler.toStdString()));
    EXPECT_CALL(editor, saveAndRunTvShowCrawler());
    EXPECT_CALL(viewer, openTvShowWithName(tvShowName.toStdString()));
    viewModel.openImportTvShowCrawlerView();
    viewModel.setRawCrawlerToImport(rawCrawler);
    viewModel.importTvShowCrawler();
}

TEST_F(TvShowCrawlerEditorViewModelTest, shouldOpenTvShowCrawlerEditorViewToEditCurrentlyViewedTvShow) {
    EXPECT_CALL(stack, pushView(serio::qt::tvShowCrawlerEditorView));
    EXPECT_CALL(editor, editTvShowCrawler(tvShowName.toStdString()));
    QSignalSpy canTvShowNameBeChangedSpy(&viewModel, &serio::qt::TvShowCrawlerEditorViewModel::canTvShowNameBeChangedChanged);
    viewModel.openCurrentTvShowCrawlerEditorView();
    EXPECT_FALSE(viewModel.canTvShowNameBeChanged());
    EXPECT_EQ(1, canTvShowNameBeChangedSpy.count());
}

TEST_F(TvShowCrawlerEditorViewModelTest, shouldOverrideExistingTvShowCrawlerWithoutAskingUserWhenEditingExistingTvShowCrawler) {
    viewModel.openCurrentTvShowCrawlerEditorView();
    ON_CALL(editor, willOverrideExistingTvShow()).WillByDefault(::testing::Return(true));
    expectTvShowToBeSavedAndOpenedInViewer(false);
    expectToPopAllViewsUntilTvShowViewAfterDisplayingCrawlingInProgress();
    viewModel.save();
}

TEST_F(TvShowCrawlerEditorViewModelTest, shouldGoBackToEditorViewIfErrorHappensWhileSavingChangesToExistingTvShowCrawler) {
    viewModel.openCurrentTvShowCrawlerEditorView();
    ON_CALL(editor, saveAndRunTvShowCrawler()).WillByDefault(::testing::Throw(std::runtime_error("expect")));
    EXPECT_CALL(stack, popAllViewsUntil(serio::qt::tvShowCrawlerEditorView));
    EXPECT_THROW(viewModel.save(), std::runtime_error);
}

TEST_F(TvShowCrawlerEditorViewModelTest, shouldOpenTvShowOverrideDialogIfUserEditsExistingShowAndThenAttemptsToCreateNewOneWithTheSameName) {
    viewModel.openCurrentTvShowCrawlerEditorView();
    expectTvShowCrawlerOverrideDialogToBeDisplayed();
    viewModel.openTvShowCrawlerEditorView();
    viewModel.loadTvShowName();
    viewModel.save();
}

TEST_F(TvShowCrawlerEditorViewModelTest, shouldOpenTvShowOverrideDialogIfUserEditsExistingShowAndThenAttemptsToImportNewOneWithTheSameName) {
    viewModel.openCurrentTvShowCrawlerEditorView();
    expectTvShowCrawlerOverrideDialogToBeDisplayed();
    viewModel.openImportTvShowCrawlerView();
    viewModel.setRawCrawlerToImport(rawCrawler);
    viewModel.importTvShowCrawler();
}

TEST_F(TvShowCrawlerEditorViewModelTest, shouldResetRawCrawlerToImportAfterReopeningImportCrawlerView) {
    ON_CALL(editor, importTvShowCrawler("")).WillByDefault(::testing::Throw(std::runtime_error("")));
    viewModel.setRawCrawlerToImport(rawCrawler);
    viewModel.openImportTvShowCrawlerView();
    EXPECT_THROW(viewModel.importTvShowCrawler(), std::runtime_error);
}

TEST_F(TvShowCrawlerEditorViewModelTest, shouldReturnListOfImportCrawlerActions) {
    QList<serio::qt::ButtonModel*> actions = viewModel.getImportTvShowCrawlerActions();
    EXPECT_EQ(serio::qt::ButtonModel("cancel", serio::qt::ActionType::BACK, {}, false), *actions[0]);
    EXPECT_EQ(serio::qt::ButtonModel("import", serio::qt::ActionType::IMPORT_TV_SHOW_CRAWLER), *actions[1]);
}
