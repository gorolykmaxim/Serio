#include <gtest/gtest.h>
#include <StackOfViewsMock.h>
#include <TvShowStorageMock.h>
#include <user-interface/view-model/TvShowViewModel.h>
#include <QSignalSpy>

class TvShowViewModelTest : public ::testing::Test {
protected:
    const unsigned int pageSize = 100;
    const std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    const serio::core::TvShow scrubs = serio::core::TvShow("Scrubs", "https://tv-show.com/thumbnail.jpg");
    const std::vector<serio::core::Episode> episodes = {
            serio::core::Episode(1, "https://tv-show.com/episodes/episode-1.mp4", "Episode 1", serio::core::LastWatchDate(now)),
            serio::core::Episode(2, "https://tv-show.com/episodes/episode-2.mp4")
    };
    ::testing::NiceMock<StackOfViewsMock> stack;
    TvShowStorageMock storage;
    serio::qt::TvShowViewModel viewModel = serio::qt::TvShowViewModel(pageSize, 2, storage, stack);
    void expectTvShowAndItsEpisodesToBeLoaded(const serio::core::TvShow& tvShow) {
        serio::core::ListPage<serio::core::Episode> page(0, 100, episodes);
        EXPECT_CALL(storage, getTvShowByName(tvShow.getName()))
                .WillOnce(::testing::Return(std::optional(tvShow)));
        EXPECT_CALL(storage, getEpisodesOfTvShowWithName(tvShow.getName(), 0, pageSize))
                .WillOnce(::testing::Return(page));
    }
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

TEST_F(TvShowViewModelTest, shouldOpenTvShowViewWithSpecifiedTvShow) {
    EXPECT_CALL(stack, pushView(QString("TvShowView.qml")));
    viewModel.openView(QVariantList({QString::fromStdString(scrubs.getName())}));
}

TEST_F(TvShowViewModelTest, shouldLoadCurrentlyDisplayedTvShowAndItsEpisodes) {
    expectTvShowAndItsEpisodesToBeLoaded(scrubs);
    QSignalSpy spy(&viewModel, &serio::qt::TvShowViewModel::selectedTvShowChanged);
    viewModel.openView(QVariantList({QString::fromStdString(scrubs.getName())}));
    viewModel.load();
    EXPECT_EQ(scrubs.getName(), viewModel.getTvShowName().toStdString());
    EXPECT_EQ(scrubs.getThumbnailUrl(), viewModel.getThumbnailUrl().toStdString());
    EXPECT_EQ(1, spy.count());
    expectEpisodesToBeLoaded();
}

TEST_F(TvShowViewModelTest, shouldHaveLastWatchDateSetToToday) {
    serio::core::TvShow friends("Friends", scrubs.getThumbnailUrl(), serio::core::LastWatchDate(now));
    expectTvShowAndItsEpisodesToBeLoaded(friends);
    viewModel.openView(QVariantList({QString::fromStdString(friends.getName())}));
    viewModel.load();
    EXPECT_EQ(QString("Last watched today"), viewModel.getLastWatchDate());
}

TEST_F(TvShowViewModelTest, shouldHaveLastWatchDateSetToYesterday) {
    serio::core::TvShow mandalorian("Mandalorian", scrubs.getThumbnailUrl(), serio::core::LastWatchDate(now - std::chrono::hours(24)));
    expectTvShowAndItsEpisodesToBeLoaded(mandalorian);
    viewModel.openView(QVariantList({QString::fromStdString(mandalorian.getName())}));
    viewModel.load();
    EXPECT_EQ(QString("Last watched yesterday"), viewModel.getLastWatchDate());
}

TEST_F(TvShowViewModelTest, shouldLoadSpecifiedPageOfEpisodes) {
    expectTvShowAndItsEpisodesToBeLoaded(scrubs);
    viewModel.openView(QVariantList({QString::fromStdString(scrubs.getName())}));
    viewModel.load();
    unsigned int offset = 100;
    serio::core::ListPage<serio::core::Episode> page(0, 100, episodes);
    EXPECT_CALL(storage, getEpisodesOfTvShowWithName(scrubs.getName(), offset, pageSize))
            .WillOnce(::testing::Return(page));
    viewModel.loadEpisodes(QVariantList({offset, pageSize}));
}
