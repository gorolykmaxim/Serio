#include <gtest/gtest.h>
#include <TvShowStorageMock.h>
#include <tv-show-player/TvShowPlayer.h>
#include <tv-show-player/Player.h>

class TvShowPlayerTest : public ::testing::Test {
protected:
    const std::string tvShowName = "Friends";
    const serio::core::Episode episode = serio::core::Episode(2, "https://tv-show/episode-1.mp4");
    ::testing::NiceMock<TvShowStorageMock> storage;
    serio::core::TvShowPlayer tvShowPlayer = serio::core::TvShowPlayer(storage);
    virtual void SetUp() {
        ON_CALL(storage, getEpisodeOfTvShowWithName(tvShowName, episode.getId()))
            .WillByDefault(::testing::Return(std::optional(episode)));
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
        serio::core::Player player = tvShowPlayer.playEpisodeOfTvShow(tvShowName, episode.getId());
        EXPECT_EQ(episode, player.getPlayingEpisode());
        EXPECT_EQ(tvShowName, player.getPlayingTvShowName());
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
