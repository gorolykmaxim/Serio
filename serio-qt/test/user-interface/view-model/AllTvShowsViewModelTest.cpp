#include <gtest/gtest.h>
#include <user-interface/view-model/AllTvShowsViewModel.h>
#include <TvShowStorageMock.h>
#include <StackOfViewsMock.h>
#include <TvShowViewerMock.h>
#include <QSignalSpy>
#include <user-interface/ViewNames.h>
#include <user-interface/model/ButtonModel.h>
#include <user-interface/model/ListModel.h>

class AllTvShowsViewModelTest : public ::testing::Test {
protected:
    unsigned int pageSize = 100;
    TvShowViewerMock viewer = TvShowViewerMock::create();
    StackOfViewsMock stack;
    TvShowStorageMock storage;
    QModelIndex index;
    serio::core::ListPage<serio::core::TvShow> page = serio::core::ListPage<serio::core::TvShow>(0, 300, {});
    serio::core::ListPage<serio::core::TvShow> smallPage = serio::core::ListPage<serio::core::TvShow>(0, 15, {});
    serio::core::ListPage<serio::core::TvShow> emptyPage = serio::core::ListPage<serio::core::TvShow>(0, 0, {});
    serio::qt::AllTvShowsViewModel viewModel = serio::qt::AllTvShowsViewModel(pageSize, 2, storage, viewer, stack);
    void expectTvShowsToBeLoaded() {
        EXPECT_CALL(storage, getWatchedTvShows(0, pageSize))
                .WillOnce(::testing::Return(smallPage));
        EXPECT_CALL(storage, getAllTvShows(0, pageSize))
                .WillOnce(::testing::Return(page));
        viewModel.loadWatchedShows(QVariantList({0, pageSize}));
        viewModel.loadAllShows(QVariantList({0, pageSize}));
    }
    void expectActionToBeHighlighted(const QString& actionName) {
        for (const auto* action: viewModel.getActions()) {
            EXPECT_EQ(action->getText() == actionName, action->isHighlighted());
        }
    }
};

TEST_F(AllTvShowsViewModelTest, shouldLoadPageOfAllTvShowsWithSpecifiedOffsetAndLimit) {
    EXPECT_CALL(storage, getAllTvShows(0, pageSize))
        .WillOnce(::testing::Return(page));
    viewModel.loadAllShows(QVariantList({0, pageSize}));
    viewModel.selectAction(serio::qt::AllTvShowsViewModel::allTvShowsAction);
    EXPECT_EQ(page.getTotalSize(), viewModel.getTvShowList()->rowCount(index));
}

TEST_F(AllTvShowsViewModelTest, shouldLoadPageOfWatchedTvShowsWithSpecifiedOffsetAndLimit) {
    EXPECT_CALL(storage, getWatchedTvShows(0, pageSize))
        .WillOnce(::testing::Return(page));
    viewModel.loadWatchedShows(QVariantList({0, pageSize}));
    viewModel.selectAction(serio::qt::AllTvShowsViewModel::watchedAction);
    EXPECT_EQ(page.getTotalSize(), viewModel.getTvShowList()->rowCount(index));
}

TEST_F(AllTvShowsViewModelTest, shouldOpenTvShowViewWhileSelectingSpecifiedTvShowInViewer) {
    std::string tvShowName = "Breaking Bad";
    EXPECT_CALL(viewer, openTvShowWithName(tvShowName));
    EXPECT_CALL(stack, pushView(serio::qt::tvShowView));
    viewModel.openTvShowView(QVariantList({QString::fromStdString(tvShowName)}));
}

TEST_F(AllTvShowsViewModelTest, shouldLoadFirstPageOfWatchShowsAndAllShows) {
    viewModel.selectAction(serio::qt::AllTvShowsViewModel::allTvShowsAction);
    QSignalSpy allTvShowsSpy(viewModel.getTvShowList(), &serio::qt::TvShowListModel::requestPageLoad);
    viewModel.selectAction(serio::qt::AllTvShowsViewModel::watchedAction);
    QSignalSpy watchedTvShowsSpy(viewModel.getTvShowList(), &serio::qt::TvShowListModel::requestPageLoad);
    viewModel.loadFirstPage();
    for (const auto& args: {allTvShowsSpy.takeFirst(), watchedTvShowsSpy.takeFirst()}) {
        EXPECT_EQ(0, args[0].toUInt());
        EXPECT_EQ(pageSize, args[1].toUInt());
    }
}

TEST_F(AllTvShowsViewModelTest, shouldReturnListOfAllActionsWithWatchedActionSelectedByDefault) {
    QList<serio::qt::ButtonModel*> actions = viewModel.getActions();
    EXPECT_EQ(*actions[0], serio::qt::ButtonModel(serio::qt::AllTvShowsViewModel::watchedAction, serio::qt::ActionType::DISPLAY_WATCHED_TV_SHOWS).setHighlighted(true));
    EXPECT_EQ(*actions[1], serio::qt::ButtonModel(serio::qt::AllTvShowsViewModel::allTvShowsAction, serio::qt::ActionType::DISPLAY_ALL_TV_SHOWS));
    EXPECT_EQ(*actions[2], serio::qt::ButtonModel(serio::qt::AllTvShowsViewModel::addTvShowAction, serio::qt::ActionType::OPEN_ADD_TV_SHOW_VIEW));
}

TEST_F(AllTvShowsViewModelTest, shouldHaveWatchedActionInActionsListAfterLoadingNonEmptyWatchedTvShowsListPage) {
    EXPECT_CALL(storage, getWatchedTvShows(0, pageSize))
        .WillOnce(::testing::Return(emptyPage))
        .WillRepeatedly(::testing::Return(page));
    viewModel.loadWatchedShows(QVariantList({0, pageSize}));
    viewModel.loadWatchedShows(QVariantList({0, pageSize}));
    viewModel.loadWatchedShows(QVariantList({0, pageSize}));
    QList<serio::qt::ButtonModel*> actions = viewModel.getActions();
    ASSERT_EQ(3, actions.size());
    EXPECT_EQ(*actions[0], serio::qt::ButtonModel(serio::qt::AllTvShowsViewModel::watchedAction, serio::qt::ActionType::DISPLAY_WATCHED_TV_SHOWS).setHighlighted(true));
}

TEST_F(AllTvShowsViewModelTest, shouldNotifyWatchersAboutActionsListChangeWhenWatchedActionGetsAddedToTheList) {
    EXPECT_CALL(storage, getWatchedTvShows(0, pageSize))
        .WillOnce(::testing::Return(emptyPage))
        .WillRepeatedly(::testing::Return(page));
    viewModel.loadWatchedShows(QVariantList({0, pageSize}));
    QSignalSpy actionsSpy(&viewModel, &serio::qt::AllTvShowsViewModel::actionsChanged);
    viewModel.loadWatchedShows(QVariantList({0, pageSize}));
    viewModel.loadWatchedShows(QVariantList({0, pageSize}));
    EXPECT_EQ(1, actionsSpy.count());
}

TEST_F(AllTvShowsViewModelTest, shouldHaveWatchedActionRemovedFromActionsListAfterLoadingEmptyWatchedTvShowsListPage) {
    EXPECT_CALL(storage, getWatchedTvShows(0, pageSize))
        .WillOnce(::testing::Return(page))
        .WillRepeatedly(::testing::Return(emptyPage));
    viewModel.loadWatchedShows(QVariantList({0, pageSize}));
    viewModel.loadWatchedShows(QVariantList({0, pageSize}));
    viewModel.loadWatchedShows(QVariantList({0, pageSize}));
    QList<serio::qt::ButtonModel*> actions = viewModel.getActions();
    EXPECT_EQ(*actions[0], serio::qt::ButtonModel(serio::qt::AllTvShowsViewModel::allTvShowsAction, serio::qt::ActionType::DISPLAY_ALL_TV_SHOWS).setHighlighted(true));
    EXPECT_EQ(*actions[1], serio::qt::ButtonModel(serio::qt::AllTvShowsViewModel::addTvShowAction, serio::qt::ActionType::OPEN_ADD_TV_SHOW_VIEW));
}

TEST_F(AllTvShowsViewModelTest, shouldNotifyWatchersAboutActionsListChangeWhenWatchedActionGetsRemovedFromTheList) {
    EXPECT_CALL(storage, getWatchedTvShows(0, pageSize))
            .WillOnce(::testing::Return(page))
            .WillRepeatedly(::testing::Return(emptyPage));
    viewModel.loadWatchedShows(QVariantList({0, pageSize}));
    QSignalSpy actionsSpy(&viewModel, &serio::qt::AllTvShowsViewModel::actionsChanged);
    viewModel.loadWatchedShows(QVariantList({0, pageSize}));
    viewModel.loadWatchedShows(QVariantList({0, pageSize}));
    EXPECT_EQ(1, actionsSpy.count());
}

TEST_F(AllTvShowsViewModelTest, shouldHighlightSelectedAction) {
    expectTvShowsToBeLoaded();
    viewModel.selectAction(serio::qt::AllTvShowsViewModel::allTvShowsAction);
    for (const auto* action: viewModel.getActions()) {
        EXPECT_EQ(action->getText() == serio::qt::AllTvShowsViewModel::allTvShowsAction, action->isHighlighted());
    }
}

TEST_F(AllTvShowsViewModelTest, shouldReturnEmptyWatchedTvShowListByDefault) {
    serio::qt::TvShowListModel* defaultList = viewModel.getTvShowList();
    EXPECT_EQ(0, defaultList->rowCount(index));
    viewModel.selectAction(serio::qt::AllTvShowsViewModel::watchedAction);
    EXPECT_EQ(defaultList, viewModel.getTvShowList());
}

TEST_F(AllTvShowsViewModelTest, shouldReturnTvShowListAccordingToSelectedAction) {
    expectTvShowsToBeLoaded();
    viewModel.selectAction(serio::qt::AllTvShowsViewModel::allTvShowsAction);
    EXPECT_EQ(page.getTotalSize(), viewModel.getTvShowList()->rowCount(index));
    viewModel.selectAction(serio::qt::AllTvShowsViewModel::watchedAction);
    EXPECT_EQ(smallPage.getTotalSize(), viewModel.getTvShowList()->rowCount(index));
}

TEST_F(AllTvShowsViewModelTest, shouldNotifyWatchersAboutTvShowListSelectionChange) {
    expectTvShowsToBeLoaded();
    QSignalSpy tvShowListSpy(&viewModel, &serio::qt::AllTvShowsViewModel::selectedListChanged);
    viewModel.selectAction(serio::qt::AllTvShowsViewModel::allTvShowsAction);
    EXPECT_EQ(1, tvShowListSpy.count());
}

TEST_F(AllTvShowsViewModelTest, shouldHighlightAllTvShowsActionWhenEmptyWatchedTvShowListPageGetsLoaded) {
    EXPECT_CALL(storage, getWatchedTvShows(0, pageSize))
        .WillOnce(::testing::Return(page))
        .WillOnce(::testing::Return(emptyPage));
    viewModel.loadWatchedShows(QVariantList({0, pageSize}));
    viewModel.loadWatchedShows(QVariantList({0, pageSize}));
    expectActionToBeHighlighted(serio::qt::AllTvShowsViewModel::allTvShowsAction);
}

TEST_F(AllTvShowsViewModelTest, shouldHighlightWatchedActionWhenNonEmptyWatchedTvShowListPageGetsLoaded) {
    EXPECT_CALL(storage, getWatchedTvShows(0, pageSize))
        .WillOnce(::testing::Return(emptyPage))
        .WillOnce(::testing::Return(page));
    viewModel.loadWatchedShows(QVariantList({0, pageSize}));
    viewModel.loadWatchedShows(QVariantList({0, pageSize}));
    expectActionToBeHighlighted(serio::qt::AllTvShowsViewModel::watchedAction);
}

TEST_F(AllTvShowsViewModelTest, shouldHightlightAllTvShowsActionWhenEmptyPageOfWatchedTvShowsGetsLoadedInitially) {
    EXPECT_CALL(storage, getWatchedTvShows(0, pageSize))
        .WillOnce(::testing::Return(emptyPage));
    viewModel.loadWatchedShows(QVariantList({0, pageSize}));
    expectActionToBeHighlighted(serio::qt::AllTvShowsViewModel::allTvShowsAction);
}

TEST_F(AllTvShowsViewModelTest, shouldNotAutomaticallySwitchToWatchedTvShowsEverytimeNonEmptyWatchedListGetsReloaded) {
    viewModel.selectAction(serio::qt::AllTvShowsViewModel::allTvShowsAction);
    EXPECT_CALL(storage, getWatchedTvShows(0, pageSize))
        .WillOnce(::testing::Return(page));
    viewModel.loadWatchedShows(QVariantList({0, pageSize}));
    expectActionToBeHighlighted(serio::qt::AllTvShowsViewModel::allTvShowsAction);
}
