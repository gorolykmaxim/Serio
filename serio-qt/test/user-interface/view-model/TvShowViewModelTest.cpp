#include <gtest/gtest.h>
#include <TvShowStorageMock.h>
#include <user-interface/view-model/TvShowViewModel.h>
#include <QSignalSpy>
#include <TvShowViewerMock.h>
#include <user-interface/view-model/SnackbarViewModel.h>

class TvShowViewModelTest : public ::testing::Test {
protected:
    const unsigned int pageSize = 100;
    const std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    const serio::core::TvShow scrubs = serio::core::TvShow("Scrubs", "https://tv-show.com/thumbnail.jpg");
    const std::vector<serio::core::Episode> episodes = {
            serio::core::Episode(1, "https://tv-show.com/episodes/episode-1.mp4", "Episode 1", serio::core::LastWatchDate(now)),
            serio::core::Episode(2, "https://tv-show.com/episodes/episode-2.mp4")
    };
    TvShowViewerMock viewer = TvShowViewerMock::create();
    serio::qt::SnackbarViewModel snackbarViewModel;
    serio::qt::TvShowViewModel viewModel = serio::qt::TvShowViewModel(pageSize, 2, viewer, snackbarViewModel);
    void expectEpisodesToBeLoaded() {
        serio::qt::EpisodeListModel* episodeList = viewModel.getEpisodeList();
        EXPECT_EQ(episodes[0].getName(), episodeList->data(episodeList->index(0), serio::qt::EpisodeListModel::Role::TITLE).toString().toStdString());
        EXPECT_EQ(episodes[0].getVideoUrl(), episodeList->data(episodeList->index(0), serio::qt::EpisodeListModel::Role::SUBTITLE).toString().toStdString());
        EXPECT_EQ(QString("eye"), episodeList->data(episodeList->index(0), serio::qt::EpisodeListModel::Role::ICON));
        EXPECT_EQ(episodes[1].getName(), episodeList->data(episodeList->index(1), serio::qt::EpisodeListModel::Role::TITLE).toString().toStdString());
        EXPECT_EQ(episodes[1].getVideoUrl(), episodeList->data(episodeList->index(1), serio::qt::EpisodeListModel::Role::SUBTITLE).toString().toStdString());
        EXPECT_EQ(QString("placeholder"), episodeList->data(episodeList->index(1), serio::qt::EpisodeListModel::Role::ICON));
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
    EXPECT_CALL(viewer, getSelectedTvShow()).WillOnce(::testing::Return(scrubs));
    QSignalSpy tvShowChangedSpy(&viewModel, &serio::qt::TvShowViewModel::selectedTvShowChanged);
    QSignalSpy requestEpisodePageSpy(viewModel.getEpisodeList(), &serio::qt::EpisodeListModel::requestPageLoad);
    viewModel.load();
    EXPECT_EQ(scrubs.getName(), viewModel.getTvShowName().toStdString());
    EXPECT_EQ(scrubs.getThumbnailUrl(), viewModel.getThumbnailUrl().toStdString());
    EXPECT_EQ(1, tvShowChangedSpy.count());
    QVariantList args = requestEpisodePageSpy.takeFirst();
    EXPECT_EQ(0, args[0].toUInt());
    EXPECT_EQ(pageSize, args[1].toUInt());
}

TEST_F(TvShowViewModelTest, shouldHaveLastWatchDateSetToToday) {
    serio::core::TvShow friends("Friends", scrubs.getThumbnailUrl(), serio::core::LastWatchDate(now));
    EXPECT_CALL(viewer, getSelectedTvShow()).WillOnce(::testing::Return(friends));
    viewModel.load();
    EXPECT_EQ(QString("Last watched today"), viewModel.getLastWatchDate());
}

TEST_F(TvShowViewModelTest, shouldHaveLastWatchDateSetToYesterday) {
    serio::core::TvShow mandalorian("Mandalorian", scrubs.getThumbnailUrl(), serio::core::LastWatchDate(now - std::chrono::hours(24)));
    EXPECT_CALL(viewer, getSelectedTvShow()).WillOnce(::testing::Return(mandalorian));
    viewModel.load();
    EXPECT_EQ(QString("Last watched yesterday"), viewModel.getLastWatchDate());
}

TEST_F(TvShowViewModelTest, shouldLoadSpecifiedPageOfEpisodes) {
    EXPECT_CALL(viewer, getSelectedTvShow()).WillOnce(::testing::Return(scrubs));
    viewModel.load();
    unsigned int offset = 100;
    serio::core::ListPage<serio::core::Episode> page(0, 100, episodes);
    EXPECT_CALL(viewer, getTvShowEpisodes(offset, pageSize)).WillOnce(::testing::Return(page));
    viewModel.loadEpisodes(QVariantList({offset, pageSize}));
    expectEpisodesToBeLoaded();
}
