#include <gtest/gtest.h>
#include <tv-show-storage/WatchProgress.h>

TEST(WatchProgressTest, shouldNotHaveProgressByDefault) {
    EXPECT_EQ(0, serio::core::WatchProgress().getPercentage());
}

TEST(WatchProgressTest, shouldFailToCreateWatchProgressWithOutOf0To100Range) {
    EXPECT_THROW(serio::core::WatchProgress(-0.1), std::invalid_argument);
    EXPECT_THROW(serio::core::WatchProgress(100.1), std::invalid_argument);
}

TEST(WatchProgressTest, shouldHaveSpecifiedProgressInPercentage) {
    EXPECT_EQ(55.6, serio::core::WatchProgress(55.6).getPercentage());
}

TEST(WatchProgressTest, twoWatchProgressesWithTheSamePercentageShouldBeEqual) {
    EXPECT_EQ(serio::core::WatchProgress(15.3), serio::core::WatchProgress(15.3));
}

TEST(WatchProgressTest, twoWatchProgressesWithDifferentPercentagesShouldNotBeEqual) {
    EXPECT_NE(serio::core::WatchProgress(42), serio::core::WatchProgress());
}
