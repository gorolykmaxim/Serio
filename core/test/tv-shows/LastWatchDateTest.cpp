#include <gtest/gtest.h>
#include <tv-show-storage/LastWatchDate.h>

class LastWatchDateTest : public ::testing::Test {
protected:
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
};

TEST_F(LastWatchDateTest, createLastWatchDateThatIsToday) {
    serio::core::LastWatchDate lastWatchDate(now);
    ASSERT_EQ("Today", lastWatchDate.toString());
}

TEST_F(LastWatchDateTest, createLastWatchDateThatIsYesterday) {
    serio::core::LastWatchDate lastWatchDate(now - std::chrono::hours(24));
    ASSERT_EQ("Yesterday", lastWatchDate.toString());
}

TEST_F(LastWatchDateTest, createLastWatchDateThatIs2DaysAgo) {
    serio::core::LastWatchDate lastWatchDate(now - std::chrono::hours(48));
    ASSERT_EQ("2 days ago", lastWatchDate.toString());
}

TEST_F(LastWatchDateTest, createLastWatchDateThatIs3DaysAgo) {
    serio::core::LastWatchDate lastWatchDate(now - std::chrono::hours(72));
    ASSERT_EQ("3 days ago", lastWatchDate.toString());
}

TEST_F(LastWatchDateTest, twoWatchDatesThatRepresentTheSameDateShouldBeEqual) {
    serio::core::LastWatchDate oneDate(now), anotherDate(now);
    ASSERT_EQ(oneDate, anotherDate);
}

TEST_F(LastWatchDateTest, twoWatchDatesThatRepresentDifferentDatesShouldNotBeEqual) {
    serio::core::LastWatchDate oneDate(now), anotherDate(now - std::chrono::seconds(2));
    ASSERT_NE(oneDate, anotherDate);
}

TEST_F(LastWatchDateTest, convertWatchDateIntoSinceEpoch) {
    serio::core::LastWatchDate date(now);
    ASSERT_EQ(now.time_since_epoch().count(), date.getSinceEpoch());
}

TEST_F(LastWatchDateTest, createWatchDateFromTimeSinceEpoch) {
    serio::core::LastWatchDate originalDate(now);
    serio::core::LastWatchDate recreated(originalDate.getSinceEpoch());
    ASSERT_EQ(originalDate, recreated);
}
