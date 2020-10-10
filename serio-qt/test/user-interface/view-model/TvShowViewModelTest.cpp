#include <gtest/gtest.h>
#include <TvShowStorageMock.h>
#include <user-interface/view-model/TvShowViewModel.h>
#include <QSignalSpy>
#include <TvShowViewerMock.h>
#include <user-interface/view-model/SnackbarViewModel.h>
#include <StackOfViewsMock.h>
#include <user-interface/ViewNames.h>
#include <DialogViewModelMock.h>
#include <BackgroundViewModelMock.h>
#include <user-interface/model/ButtonModel.h>
#include <SnackbarViewModelMock.h>

class TvShowViewModelTest : public ::testing::Test {
protected:
    const unsigned int offset = 100;
    const unsigned int pageSize = 100;
    const std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    const serio::core::TvShow scrubs = serio::core::TvShow("Scrubs", "https://tv-show.com/thumbnail.jpg");
    const serio::core::TvShow friends = serio::core::TvShow("Friends", scrubs.getThumbnailUrl(), serio::core::LastWatchDate(now));
    const std::vector<serio::core::Episode> episodes = {
            serio::core::Episode(1, "https://tv-show.com/episodes/episode-1.mp4", "Episode 1", serio::core::LastWatchDate(now)),
            serio::core::Episode(2, "https://tv-show.com/episodes/episode-2.mp4")
    };
    ::testing::NiceMock<TvShowViewerMock> viewer;
    DialogViewModelMock dialog;
    SnackbarViewModelMock snackbar;
    ::testing::NiceMock<StackOfViewsMock> stack;
    ::testing::NiceMock<BackgroundViewModelMock> background;
    serio::qt::TvShowViewModel viewModel = serio::qt::TvShowViewModel(pageSize, 2, viewer, dialog, background,
                                                                      snackbar, stack);
    QSignalSpy actionsSpy = QSignalSpy(&viewModel, &serio::qt::TvShowViewModel::actionsChanged);
    virtual void SetUp() {
        ON_CALL(viewer, getSelectedTvShow()).WillByDefault(::testing::Return(scrubs));
        serio::core::ListPage<serio::core::Episode> page(offset,offset + 100, episodes);
        ON_CALL(viewer, getTvShowEpisodes(offset, pageSize)).WillByDefault(::testing::Return(page));
    }
    void expectActionsToBePopulated(unsigned int actionsChangedNotifications, std::optional<QString> playButtonText = {}, const std::string& tvShowName = "") {
        auto actions = viewModel.getActions();
        ASSERT_EQ(playButtonText ? 3 : 2, actions.size());
        int i = 0;
        if (playButtonText) {
            EXPECT_EQ(serio::qt::ButtonModel(*playButtonText, serio::qt::ActionType::PLAY_TV_SHOW, {QString::fromStdString(tvShowName)}), *actions[i++]);
        }
        EXPECT_EQ(serio::qt::ButtonModel("back", serio::qt::ActionType::BACK, {}, false), *actions[i++]);
        EXPECT_EQ(serio::qt::ButtonModel("details", serio::qt::ActionType::OPEN_TV_SHOW_DETAILS_VIEW), *actions[i]);
        EXPECT_EQ(actionsChangedNotifications * 2, actionsSpy.count());
    }
};

TEST_F(TvShowViewModelTest, shouldHaveEmptyTvShowNameByDefault) {
    EXPECT_TRUE(viewModel.getTvShowName().isEmpty());
}

TEST_F(TvShowViewModelTest, shouldHaveNullLastWatchDateByDefault) {
    EXPECT_TRUE(viewModel.getLastWatchDate().isEmpty());
}

TEST_F(TvShowViewModelTest, shouldHaveEmptyThumbnailByDefault) {
    EXPECT_TRUE(viewModel.getThumbnailUrl().isEmpty());
}

TEST_F(TvShowViewModelTest, shouldHaveEmptyListOfEpisodesByDefault) {
    EXPECT_EQ(0, viewModel.getEpisodeList()->rowCount(QModelIndex()));
}

TEST_F(TvShowViewModelTest, shouldLoadCurrentlyDisplayedTvShowAndRequestFirstPageOfItsEpisodes) {
    viewModel.load();
    EXPECT_EQ(scrubs.getName(), viewModel.getTvShowName().toStdString());
    EXPECT_EQ(scrubs.getThumbnailUrl(), viewModel.getThumbnailUrl().toStdString());
}

TEST_F(TvShowViewModelTest, shouldNotifyWatchersAboutTvShowChange) {
    QSignalSpy tvShowChangedSpy(&viewModel, &serio::qt::TvShowViewModel::selectedTvShowChanged);
    viewModel.load();
    EXPECT_EQ(1, tvShowChangedSpy.count());
}

TEST_F(TvShowViewModelTest, shouldRequestFirstPageOfSelectedTvShowEpisodes) {
    QSignalSpy requestEpisodePageSpy(viewModel.getEpisodeList(), &serio::qt::EpisodeListModel::requestPageLoad);
    viewModel.load();
    auto args = requestEpisodePageSpy.takeFirst();
    EXPECT_EQ(0, args[0].toUInt());
    EXPECT_EQ(pageSize, args[1].toUInt());
}

TEST_F(TvShowViewModelTest, shouldSetApplicationBackgroundToThumbnailImageOfSelectedTvShow) {
    EXPECT_CALL(background, setImage(QString::fromStdString(scrubs.getThumbnailUrl())));
    viewModel.load();
}

TEST_F(TvShowViewModelTest, shouldHaveLastWatchDateSetToToday) {
    ON_CALL(viewer, getSelectedTvShow()).WillByDefault(::testing::Return(friends));
    viewModel.load();
    EXPECT_EQ(QString("Last watched today"), viewModel.getLastWatchDate());
}

TEST_F(TvShowViewModelTest, shouldHaveLastWatchDateSetToYesterday) {
    serio::core::TvShow mandalorian("Mandalorian", scrubs.getThumbnailUrl(), serio::core::LastWatchDate(now - std::chrono::hours(24)));
    ON_CALL(viewer, getSelectedTvShow()).WillByDefault(::testing::Return(mandalorian));
    viewModel.load();
    EXPECT_EQ(QString("Last watched yesterday"), viewModel.getLastWatchDate());
}

TEST_F(TvShowViewModelTest, shouldLoadSpecifiedPageOfEpisodes) {
    viewModel.loadEpisodes(QVariantList({offset, pageSize}));
    auto episodeList = viewModel.getEpisodeList();
    EXPECT_EQ(episodes[0].getName(), episodeList->data(episodeList->index(offset), serio::qt::EpisodeListModel::Role::TITLE).toString().toStdString());
    EXPECT_EQ(episodes[0].getVideoUrl(), episodeList->data(episodeList->index(offset), serio::qt::EpisodeListModel::Role::SUBTITLE).toString().toStdString());
    EXPECT_EQ(QString("visibility"), episodeList->data(episodeList->index(offset), serio::qt::EpisodeListModel::Role::ICON));
    EXPECT_EQ(episodes[1].getName(), episodeList->data(episodeList->index(offset + 1), serio::qt::EpisodeListModel::Role::TITLE).toString().toStdString());
    EXPECT_EQ(episodes[1].getVideoUrl(), episodeList->data(episodeList->index(offset + 1), serio::qt::EpisodeListModel::Role::SUBTITLE).toString().toStdString());
    EXPECT_EQ(QString("placeholder"), episodeList->data(episodeList->index(offset + 1), serio::qt::EpisodeListModel::Role::ICON));
}

TEST_F(TvShowViewModelTest, shouldCrawlSelectedTvShow) {
    ::testing::InSequence s;
    EXPECT_CALL(stack, pushView(serio::qt::crawlingInProgressView));
    EXPECT_CALL(viewer, crawlSelectedTvShow());
    EXPECT_CALL(stack, popCurrentView());
    viewModel.crawl();
}

TEST_F(TvShowViewModelTest, shouldPopCrawlingInProgressViewEvenIfCrawlFails) {
    ON_CALL(viewer, crawlSelectedTvShow()).WillByDefault(::testing::Throw(std::runtime_error("expected")));
    EXPECT_CALL(stack, popCurrentView());
    EXPECT_THROW(viewModel.crawl(), std::runtime_error);
}

TEST_F(TvShowViewModelTest, shouldConfirmIfUserWantsToClearWatchHistoryOfSelectedTvShow) {
    viewModel.load();
    serio::qt::DialogModel model("Clear Watch History",
                                 "You are about to clear your watch history of '" + QString::fromStdString(scrubs.getName()) + "'.");
    model.setTopButtonAction(serio::qt::ActionType::CLEAR_CURRENT_TV_SHOW_WATCH_HISTORY);
    EXPECT_CALL(dialog, display(model, false));
    viewModel.confirmClearWatchHistory();
}

TEST_F(TvShowViewModelTest, shouldClearWatchHistoryOfSelectedTvShow) {
    EXPECT_CALL(viewer, clearSelectedTvShowWatchHistory());
    EXPECT_CALL(snackbar, displayText(QString("Watch history cleared")));
    EXPECT_CALL(stack, popCurrentView());
    viewModel.clearWatchHistory();
}

TEST_F(TvShowViewModelTest, shouldConfirmIfUserWantsToDeleteSelectedTvShow) {
    viewModel.load();
    serio::qt::DialogModel model("Delete TV Show",
                                 "You are about to delete '" + QString::fromStdString(scrubs.getName()) + "'");
    model.setTopButtonAction(serio::qt::ActionType::DELETE_CURRENT_TV_SHOW);
    EXPECT_CALL(dialog, display(model, false));
    viewModel.confirmDeleteTvShow();
}

TEST_F(TvShowViewModelTest, shouldDeleteSelectedTvShow) {
    EXPECT_CALL(viewer, deleteSelectedTvShow());
    EXPECT_CALL(stack, popAllViews());
    viewModel.deleteTvShow();
}

TEST_F(TvShowViewModelTest, shouldOpenTvShowDetailsView) {
    EXPECT_CALL(stack, pushView(serio::qt::tvShowDetailsView));
    viewModel.openTvShowDetails();
}

TEST_F(TvShowViewModelTest, shouldHaveBackAndDetailsActionsByDefault) {
    expectActionsToBePopulated(0);
}

TEST_F(TvShowViewModelTest, shouldReturnListOfActionsWithoutPlayButtonSinceTvShowHasNoEpisodes) {
    auto offset = 0;
    serio::core::ListPage<serio::core::Episode> emptyPage(offset, 0, {});
    ON_CALL(viewer, getTvShowEpisodes(offset, pageSize)).WillByDefault(::testing::Return(emptyPage));
    viewModel.loadEpisodes(QVariantList({offset, pageSize}));
    viewModel.loadEpisodes(QVariantList({offset, pageSize}));
    expectActionsToBePopulated(2);
}

TEST_F(TvShowViewModelTest, shouldReturnListOfActionsWithPlayButtonSinceTvShowHasNotBeenWatchedYet) {
    viewModel.load();
    viewModel.loadEpisodes(QVariantList({offset, pageSize}));
    viewModel.loadEpisodes(QVariantList({offset, pageSize}));
    expectActionsToBePopulated(2, "play", scrubs.getName());
}

TEST_F(TvShowViewModelTest, shouldReturnListOfActionsWithResumeButtonSinceTvShowHasBeenWatchedInThePast) {
    ON_CALL(viewer, getSelectedTvShow()).WillByDefault(::testing::Return(friends));
    viewModel.load();
    viewModel.loadEpisodes(QVariantList({offset, pageSize}));
    expectActionsToBePopulated(1, "resume", friends.getName());
}
