#include <gtest/gtest.h>
#include <TvShowStorageMock.h>
#include <tv-show-player/TvShowPlayer.h>
#include <tv-show-player/Player.h>

class TvShowPlayerTest : public ::testing::Test {
protected:
    const std::string tvShowName = "Friends";
    const serio::core::LastWatchDate lastWatchDate;
    const serio::core::WatchProgress watchProgress = serio::core::WatchProgress(32);
    const serio::core::Episode episode = serio::core::Episode(3, "https://tv-show/episode-3.mp4", "Episode 3", lastWatchDate, watchProgress);
    const serio::core::Episode firstEpisode = serio::core::Episode(1, episode.getVideoUrl());
    const serio::core::Episode lastWatchedEpisode = serio::core::Episode(episode.getId() - 1,
                                                                         episode.getVideoUrl(),
                                                                         episode.getName(),
                                                                         episode.getLastWatchDate(),
                                                                         serio::core::WatchProgress(100));
    ::testing::NiceMock<TvShowStorageMock> storage;
    serio::core::TvShowPlayer tvShowPlayer = serio::core::TvShowPlayer(storage);
    virtual void SetUp() {
        ON_CALL(storage, getEpisodeOfTvShowWithName(tvShowName, episode.getId()))
            .WillByDefault(::testing::Return(std::optional(episode)));
        ON_CALL(storage, getEpisodeOfTvShowWithName(tvShowName, firstEpisode.getId()))
            .WillByDefault(::testing::Return(std::optional(firstEpisode)));
        ON_CALL(storage, getLastWatchedEpisodeOfTvShow(tvShowName))
            .WillByDefault(::testing::Return(std::optional(lastWatchedEpisode)));
    }
    void expectTvShowPlayerToBePlaying() {
        (void)tvShowPlayer.updatePlayingEpisodeWatchProgress(watchProgress);
    }
};

TEST_F(TvShowPlayerTest, shouldFailToPlayTvShowEpisodeThatDoesNotExist) {
    ON_CALL(storage, getEpisodeOfTvShowWithName(tvShowName, episode.getId()))
        .WillByDefault(::testing::Return(std::optional<serio::core::Episode>()));
    EXPECT_THROW((void)tvShowPlayer.playEpisodeOfTvShow(tvShowName, episode.getId()), std::logic_error);
}

TEST_F(TvShowPlayerTest, shouldPlaySpecifiedEpisodeOfTvShow) {
    serio::core::Episode nextEpisode(episode.getId() + 1, episode.getVideoUrl());
    for (const auto& episode: {episode, nextEpisode}) {
        ON_CALL(storage, getEpisodeOfTvShowWithName(tvShowName, episode.getId()))
            .WillByDefault(::testing::Return(std::optional(episode)));
        auto player = tvShowPlayer.playEpisodeOfTvShow(tvShowName, episode.getId());
        EXPECT_EQ(episode, player.getPlayingEpisode());
        EXPECT_EQ(tvShowName, player.getPlayingTvShowName());
        EXPECT_EQ(0, player.getStartPercentage());
    }
}

TEST_F(TvShowPlayerTest, shouldFailToWatchTvShowEpisodeThatIsNotCurrentlyPlaying) {
    EXPECT_THROW(tvShowPlayer.updatePlayingEpisodeWatchProgress(serio::core::WatchProgress(15)),
                 std::logic_error);
}

TEST_F(TvShowPlayerTest, shouldWatchCurrentlyPlayingTvShowEpisode) {
    serio::core::LastWatchDate now;
    serio::core::WatchProgress progress(15);
    EXPECT_CALL(storage, watchTvShowEpisode(tvShowName, episode.getId(), ::testing::Ge(now), progress));
    (void)tvShowPlayer.playEpisodeOfTvShow(tvShowName, episode.getId());
    tvShowPlayer.updatePlayingEpisodeWatchProgress(progress);
}

TEST_F(TvShowPlayerTest, shouldPlaySpecifiedTvShow) {
    auto player = tvShowPlayer.playTvShow(tvShowName);
    EXPECT_EQ(tvShowName, player.getPlayingTvShowName());
}

TEST_F(TvShowPlayerTest, shouldFailToPlayFirstEpisodeOfTvShowSinceTvShowHasNoEpisodes) {
    ON_CALL(storage, getLastWatchedEpisodeOfTvShow(tvShowName))
        .WillByDefault(::testing::Return(std::optional<serio::core::Episode>()));
    ON_CALL(storage, getEpisodeOfTvShowWithName(tvShowName, firstEpisode.getId()))
        .WillByDefault(::testing::Return(std::optional<serio::core::Episode>()));
    EXPECT_THROW((void)tvShowPlayer.playTvShow(tvShowName), std::logic_error);
}

TEST_F(TvShowPlayerTest, shouldPlayFirstEpisodeOfSpecifiedTvShowSinceTvShowHasNotBeenWatchedYet) {
    ON_CALL(storage, getLastWatchedEpisodeOfTvShow(tvShowName))
        .WillByDefault(::testing::Return(std::optional<serio::core::Episode>()));
    auto player = tvShowPlayer.playTvShow(tvShowName);
    EXPECT_EQ(firstEpisode, player.getPlayingEpisode());
    EXPECT_EQ(0, player.getStartPercentage());
    expectTvShowPlayerToBePlaying();
}

TEST_F(TvShowPlayerTest, shouldContinuePlayingLastWatchedEpisodeOfTvShow) {
    ON_CALL(storage, getLastWatchedEpisodeOfTvShow(tvShowName))
        .WillByDefault(::testing::Return(std::optional(episode)));
    auto player = tvShowPlayer.playTvShow(tvShowName);
    EXPECT_EQ(episode, player.getPlayingEpisode());
    EXPECT_EQ(episode.getWatchProgress().getPercentage(), player.getStartPercentage());
    expectTvShowPlayerToBePlaying();
}

TEST_F(TvShowPlayerTest, shouldFailToPlayEpisodeThatIsNextToTheLastWatchedEpisodeSinceTheLatterOneIsTheLastEpisodeOfTvShow) {
    ON_CALL(storage, getEpisodeOfTvShowWithName(tvShowName, lastWatchedEpisode.getId() + 1))
        .WillByDefault(::testing::Return(std::optional<serio::core::Episode>()));
    EXPECT_THROW((void)tvShowPlayer.playTvShow(tvShowName), std::logic_error);
}

TEST_F(TvShowPlayerTest, shouldPlayEpisodeNextToTheLastWatchedEpisodeIfTheLatterOneHasBeenWatchedCompletely) {
    auto player = tvShowPlayer.playTvShow(tvShowName);
    EXPECT_EQ(episode, player.getPlayingEpisode());
    EXPECT_EQ(0, player.getStartPercentage());
    expectTvShowPlayerToBePlaying();
}
