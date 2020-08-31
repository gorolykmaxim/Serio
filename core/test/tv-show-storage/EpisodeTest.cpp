#include <gtest/gtest.h>
#include <tv-show-storage/Episode.h>

class EpisodeTest : public ::testing::Test {
protected:
    std::string expectedVideoUrl = "https://tv-show.com/episode-1.mp4";
};

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
    std::string expectedName = "Going dark";
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
}
