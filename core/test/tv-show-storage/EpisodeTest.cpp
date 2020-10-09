#include <gtest/gtest.h>
#include <tv-show-storage/Episode.h>
#include <tv-show-storage/WatchProgress.h>

class EpisodeTest : public ::testing::Test {
protected:
    const serio::core::LastWatchDate lastWatchDate = serio::core::LastWatchDate(std::chrono::system_clock::now());
    std::string expectedVideoUrl = "https://tv-show.com/episode-1.mp4";
};

TEST_F(EpisodeTest, shouldFailToCreateEpisodeThatWithIdLessThan1) {
    EXPECT_THROW(serio::core::Episode(0, expectedVideoUrl), std::invalid_argument);
}

TEST_F(EpisodeTest, shouldCreateEpisodeWithSpecifiedId) {
    serio::core::Episode episode(1, expectedVideoUrl);
    EXPECT_EQ(1, episode.getId());
    episode = serio::core::Episode(2, expectedVideoUrl);
    EXPECT_EQ(2, episode.getId());
}

TEST_F(EpisodeTest, shouldCreateEpisodeWithDefaultName) {
    serio::core::Episode episode(1, expectedVideoUrl);
    EXPECT_EQ("Episode 1", episode.getName());
    episode = serio::core::Episode(2, expectedVideoUrl);
    EXPECT_EQ("Episode 2", episode.getName());
}

TEST_F(EpisodeTest, shouldCreateEpisodeWithSpecifiedName) {
    auto expectedName = "Going dark";
    serio::core::Episode episode(1, expectedVideoUrl, expectedName);
    EXPECT_EQ(expectedName, episode.getName());
}

TEST_F(EpisodeTest, shouldCreateEpisodeWithSpecifiedVideoUrl) {
    serio::core::Episode episode(1, expectedVideoUrl);
    EXPECT_EQ(expectedVideoUrl, episode.getVideoUrl());
    expectedVideoUrl = "https://tv-show.com/episode-2.mp4";
    episode = serio::core::Episode(2, expectedVideoUrl);
    EXPECT_EQ(expectedVideoUrl, episode.getVideoUrl());
}

TEST_F(EpisodeTest, twoEpisodesWithSameIdNameAndVideoUrlShouldBeEqual) {
    serio::core::Episode episode1(1, expectedVideoUrl);
    serio::core::Episode episode2(1, expectedVideoUrl);
    EXPECT_EQ(episode1, episode2);
}

TEST_F(EpisodeTest, twoEpisodesWithDifferentIdNameOrVideoUrlShouldNotBeEqual) {
    serio::core::Episode episode(1, expectedVideoUrl);
    EXPECT_NE(episode, serio::core::Episode(2, expectedVideoUrl));
    EXPECT_NE(episode, serio::core::Episode(1, ""));
    EXPECT_NE(episode, serio::core::Episode(1, expectedVideoUrl, ""));
    EXPECT_NE(episode, serio::core::Episode(1, expectedVideoUrl, "Episode 1", lastWatchDate));
    EXPECT_NE(episode, serio::core::Episode(1, expectedVideoUrl, "Episode 1", std::optional<serio::core::LastWatchDate>(), serio::core::WatchProgress(15)));
}

TEST_F(EpisodeTest, shouldNotHaveLastWatchDateIfNotSpecifiedExplicitly) {
    serio::core::Episode episode(1, expectedVideoUrl);
    EXPECT_FALSE(episode.getLastWatchDate());
}

TEST_F(EpisodeTest, shouldHaveSpecifiedLastWatchDate) {
    serio::core::Episode episode(2, expectedVideoUrl, "Episode 1", lastWatchDate);
    EXPECT_EQ(lastWatchDate, *episode.getLastWatchDate());
}

TEST_F(EpisodeTest, shouldHaveEmptyWatchProgressIfNotWatched) {
    serio::core::Episode episode(1, expectedVideoUrl);
    EXPECT_EQ(serio::core::WatchProgress(), episode.getWatchProgress());
}

TEST_F(EpisodeTest, shouldHaveSpecifiedWatchProgress) {
    serio::core::WatchProgress progress(32);
    serio::core::Episode episode(2, expectedVideoUrl, "Episode 2", lastWatchDate, progress);
    EXPECT_EQ(progress, episode.getWatchProgress());
}

TEST_F(EpisodeTest, shouldReturnIdOfTheNextEpisode) {
    EXPECT_EQ(2, serio::core::Episode(1, expectedVideoUrl).getNextEpisodeId());
    EXPECT_EQ(3, serio::core::Episode(2, expectedVideoUrl).getNextEpisodeId());
}

TEST_F(EpisodeTest, shouldBeFirstIfIdIsOne) {
    EXPECT_FALSE(serio::core::Episode(2, expectedVideoUrl).isFirst());
    EXPECT_TRUE(serio::core::Episode(1, expectedVideoUrl).isFirst());
}

TEST_F(EpisodeTest, shouldReturnIdOfThePreviousEpisode) {
    EXPECT_EQ(1, serio::core::Episode(1, expectedVideoUrl).getPreviousEpisodeId());
    EXPECT_EQ(2, serio::core::Episode(3, expectedVideoUrl).getPreviousEpisodeId());
}
