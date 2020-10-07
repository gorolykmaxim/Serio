#include <gtest/gtest.h>
#include <StackOfViewsMock.h>
#include <user-interface/view-model/TvShowPlayerViewModel.h>
#include <tv-show-storage/Episode.h>
#include <TvShowPlayerMock.h>
#include <user-interface/ViewNames.h>
#include <QSignalSpy>

class TvShowPlayerViewModelTest : public ::testing::Test {
protected:
    QString tvShowName = "Mandalorian";
    serio::core::Episode episode = serio::core::Episode(3, "https://mandalorian.com/episode-3.mp4");
    ::testing::NiceMock<StackOfViewsMock> stack;
    ::testing::NiceMock<TvShowPlayerMock> tvShowPlayer;
    serio::qt::TvShowPlayerViewModel viewModel = serio::qt::TvShowPlayerViewModel(tvShowPlayer, stack);
    virtual void SetUp() {
        ON_CALL(tvShowPlayer, playEpisodeOfTvShow(tvShowName.toStdString(), episode.getId()))
            .WillByDefault(::testing::Return(serio::core::Player(tvShowName.toStdString(), episode)));
    }
};

TEST_F(TvShowPlayerViewModelTest, shouldOpenTvShowPlayerView) {
    EXPECT_CALL(stack, pushView(serio::qt::tvShowPlayerView));
    viewModel.playEpisodeOfTvShow(QVariantList({tvShowName, episode.getId()}));
}

TEST_F(TvShowPlayerViewModelTest, shouldSetEpisodeVideoUrlToUrlOfTheSpecifiedEpisode) {
    serio::core::Episode nextEpisode(4, "https://mandalorian.com/episode-4.mp4");
    for (const auto& episode: {episode, nextEpisode}) {
        ON_CALL(tvShowPlayer, playEpisodeOfTvShow(tvShowName.toStdString(), episode.getId()))
            .WillByDefault(::testing::Return(serio::core::Player(tvShowName.toStdString(), episode)));
        viewModel.playEpisodeOfTvShow(QVariantList({tvShowName, episode.getId()}));
        EXPECT_EQ(QString::fromStdString(episode.getVideoUrl()), viewModel.getEpisodeVideoUrl());
    }
}

TEST_F(TvShowPlayerViewModelTest, shouldReturnEmptyEpisodeVideoUrlByDefault) {
    EXPECT_TRUE(viewModel.getEpisodeVideoUrl().isEmpty());
}

TEST_F(TvShowPlayerViewModelTest, shouldReturnEmptyTvShowNameByDefault) {
    EXPECT_TRUE(viewModel.getTvShowName().isEmpty());
}

TEST_F(TvShowPlayerViewModelTest, shouldSetTvShowNameToTheSpecifiedOne) {
    viewModel.playEpisodeOfTvShow(QVariantList({tvShowName, episode.getId()}));
    EXPECT_EQ(tvShowName, viewModel.getTvShowName());
}

TEST_F(TvShowPlayerViewModelTest, shouldReturnEmptyEpisodeNameByDefault) {
    EXPECT_TRUE(viewModel.getEpisodeName().isEmpty());
}

TEST_F(TvShowPlayerViewModelTest, shouldSetEpisodeNameToTheSpecifiedOne) {
    viewModel.playEpisodeOfTvShow(QVariantList({tvShowName, episode.getId()}));
    EXPECT_EQ(QString::fromStdString(episode.getName()), viewModel.getEpisodeName());
}

TEST_F(TvShowPlayerViewModelTest, shouldNotifyWatchersAboutPlayingEpisodeChange) {
    QSignalSpy playingEpisodeSpy(&viewModel, &serio::qt::TvShowPlayerViewModel::playingEpisodeChanged);
    viewModel.playEpisodeOfTvShow(QVariantList({tvShowName, episode.getId()}));
    EXPECT_EQ(1, playingEpisodeSpy.count());
}
