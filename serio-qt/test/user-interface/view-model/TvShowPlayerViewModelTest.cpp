#include <gtest/gtest.h>
#include <StackOfViewsMock.h>
#include <user-interface/view-model/TvShowPlayerViewModel.h>
#include <tv-show-storage/Episode.h>
#include <TvShowPlayerMock.h>
#include <user-interface/ViewNames.h>
#include <QSignalSpy>

class TvShowPlayerViewModelTest : public ::testing::Test {
protected:
    const QString tvShowName = "Mandalorian";
    const serio::core::LastWatchDate lastWatchDate;
    const serio::core::WatchProgress progress = serio::core::WatchProgress(56);
    const serio::core::Episode episode = serio::core::Episode(3,
                                                              "https://mandalorian.com/episode-3.mp4",
                                                              "Episode 3",
                                                              lastWatchDate,
                                                              progress);
    const serio::core::Player player = serio::core::Player(tvShowName.toStdString(), episode, true, false);
    const serio::core::Episode anotherEpisode = serio::core::Episode(2, episode.getVideoUrl(), "Episode 2");
    const serio::core::Player anotherPlayer = serio::core::Player(tvShowName.toStdString(), anotherEpisode, true);
    ::testing::NiceMock<StackOfViewsMock> stack;
    ::testing::NiceMock<TvShowPlayerMock> tvShowPlayer;
    serio::qt::TvShowPlayerViewModel viewModel = serio::qt::TvShowPlayerViewModel(tvShowPlayer, stack);
    QSignalSpy playingEpisodeSpy = QSignalSpy(&viewModel, &serio::qt::TvShowPlayerViewModel::playingEpisodeChanged);
    virtual void SetUp() {
        ON_CALL(tvShowPlayer, playEpisodeOfTvShow(tvShowName.toStdString(), episode.getId()))
            .WillByDefault(::testing::Return(player));
        ON_CALL(tvShowPlayer, playTvShow(tvShowName.toStdString()))
            .WillByDefault(::testing::Return(player));
    }
    void expectEpisodeToStartPlaying(const serio::core::Episode& expectedEpisode, const serio::core::Player& expectedPlayer) {
        EXPECT_EQ(QString::fromStdString(expectedEpisode.getVideoUrl()), viewModel.getEpisodeVideoUrl());
        EXPECT_EQ(tvShowName, viewModel.getTvShowName());
        EXPECT_EQ(QString::fromStdString(expectedEpisode.getName()), viewModel.getEpisodeName());
        EXPECT_EQ(expectedPlayer.getStartPercentage(), viewModel.getOffsetPercentage());
        EXPECT_EQ(expectedPlayer.hasPreviousEpisode(), viewModel.hasPreviousEpisode());
        EXPECT_EQ(expectedPlayer.hasNextEpisode(), viewModel.hasNextEpisode());
        EXPECT_EQ(1, playingEpisodeSpy.count());
    }
};

TEST_F(TvShowPlayerViewModelTest, shouldNotPlayAnythingByDefault) {
    EXPECT_TRUE(viewModel.getEpisodeVideoUrl().isEmpty());
    EXPECT_TRUE(viewModel.getTvShowName().isEmpty());
    EXPECT_TRUE(viewModel.getEpisodeName().isEmpty());
    EXPECT_FALSE(viewModel.hasPreviousEpisode());
    EXPECT_FALSE(viewModel.hasNextEpisode());
    EXPECT_EQ(0, viewModel.getOffsetPercentage());
}

TEST_F(TvShowPlayerViewModelTest, shouldPlayerSpecifiedEpisodeOfTvShow) {
    EXPECT_CALL(stack, pushView(serio::qt::tvShowPlayerView));
    viewModel.playEpisodeOfTvShow(QVariantList({tvShowName, episode.getId()}));
    expectEpisodeToStartPlaying(episode, player);
}

TEST_F(TvShowPlayerViewModelTest, shouldUpdateWatchProgressOfCurrentlyPlayingEpisode) {
    auto duration = 100000;
    auto position = 25000;
    EXPECT_CALL(tvShowPlayer, updatePlayingEpisodeWatchProgress(serio::core::WatchProgress(25)));
    viewModel.setProgress(QVariantList({position, duration}));
}

TEST_F(TvShowPlayerViewModelTest, shouldNotUpdateWatchProgressIfDurationIsZero) {
    EXPECT_CALL(tvShowPlayer, updatePlayingEpisodeWatchProgress(::testing::_)).Times(0);
    viewModel.setProgress(QVariantList({0, 0}));
}

TEST_F(TvShowPlayerViewModelTest, shouldPlaySpecifiedTvShow) {
    EXPECT_CALL(stack, pushView(serio::qt::tvShowPlayerView));
    viewModel.playTvShow(QVariantList({tvShowName}));
    expectEpisodeToStartPlaying(episode, player);
}

TEST_F(TvShowPlayerViewModelTest, shouldConvertDurationInMillisecondsIntoHumanReadableString) {
    EXPECT_EQ("15:02", viewModel.formatDuration(std::chrono::milliseconds(std::chrono::minutes(15) + std::chrono::seconds(2)).count()).toStdString());
    EXPECT_EQ("3:45", viewModel.formatDuration(std::chrono::milliseconds(std::chrono::minutes(3) + std::chrono::seconds(45)).count()).toStdString());
    EXPECT_EQ("0:12", viewModel.formatDuration(std::chrono::milliseconds(std::chrono::seconds(12)).count()).toStdString());
}

TEST_F(TvShowPlayerViewModelTest, shouldPlayPreviousEpisode) {
    ON_CALL(tvShowPlayer, playPreviousEpisode()).WillByDefault(::testing::Return(anotherPlayer));
    EXPECT_CALL(stack, pushView(serio::qt::tvShowPlayerView)).Times(0);
    viewModel.playPreviousEpisode();
    expectEpisodeToStartPlaying(anotherEpisode, anotherPlayer);
}

TEST_F(TvShowPlayerViewModelTest, shouldPlayNextEpisode) {
    ON_CALL(tvShowPlayer, playNextEpisode()).WillByDefault(::testing::Return(anotherPlayer));
    EXPECT_CALL(stack, pushView(serio::qt::tvShowPlayerView)).Times(0);
    viewModel.playNextEpisode();
    expectEpisodeToStartPlaying(anotherEpisode, anotherPlayer);
}
