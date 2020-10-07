#include <gtest/gtest.h>
#include <tv-show-storage/LastWatchDate.h>

class LastWatchDateTest : public ::testing::Test {
protected:
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
};

TEST_F(LastWatchDateTest, createLastWatchDateThatIsToday) {
    serio::core::LastWatchDate lastWatchDate(now);
    EXPECT_EQ("today", lastWatchDate.toString());
}

TEST_F(LastWatchDateTest, createLastWatchDateThatIsYesterday) {
    serio::core::LastWatchDate lastWatchDate(now - std::chrono::hours(24));
    EXPECT_EQ("yesterday", lastWatchDate.toString());
}

TEST_F(LastWatchDateTest, createLastWatchDateThatIs2DaysAgo) {
    serio::core::LastWatchDate lastWatchDate(now - std::chrono::hours(48));
    EXPECT_EQ("2 days ago", lastWatchDate.toString());
}

TEST_F(LastWatchDateTest, createLastWatchDateThatIs3DaysAgo) {
    serio::core::LastWatchDate lastWatchDate(now - std::chrono::hours(72));
    EXPECT_EQ("3 days ago", lastWatchDate.toString());
}

TEST_F(LastWatchDateTest, createLastWatchDateThatIs1MonthAgo) {
    serio::core::LastWatchDate lastWatchDate(now - std::chrono::hours(24 * 30));
    EXPECT_EQ("1 month ago", lastWatchDate.toString());
}

TEST_F(LastWatchDateTest, createLastWatchDateThatIs1YearAgo) {
    serio::core::LastWatchDate lastWatchDate(now - std::chrono::hours(24 * 30 * 12));
    EXPECT_EQ("1 year ago", lastWatchDate.toString());
}

TEST_F(LastWatchDateTest, createLastWatchDateThatIs2YearsAgo) {
    serio::core::LastWatchDate lastWatchDate(now - std::chrono::hours(24 * 30 * 12 * 2));
    EXPECT_EQ("2 years ago", lastWatchDate.toString());
}

TEST_F(LastWatchDateTest, twoWatchDatesThatRepresentTheSameDateShouldBeEqual) {
    serio::core::LastWatchDate oneDate(now), anotherDate(now);
    EXPECT_EQ(oneDate, anotherDate);
}

TEST_F(LastWatchDateTest, twoWatchDatesThatRepresentDifferentDatesShouldNotBeEqual) {
    serio::core::LastWatchDate oneDate(now), anotherDate(now - std::chrono::seconds(2));
    EXPECT_NE(oneDate, anotherDate);
}

TEST_F(LastWatchDateTest, convertWatchDateIntoSinceEpoch) {
    serio::core::LastWatchDate date(now);
    EXPECT_EQ(now.time_since_epoch().count(), date.getSinceEpoch());
}

TEST_F(LastWatchDateTest, createWatchDateFromTimeSinceEpoch) {
    serio::core::LastWatchDate originalDate(now);
    serio::core::LastWatchDate recreated(originalDate.getSinceEpoch());
    EXPECT_EQ(originalDate, recreated);
}

TEST_F(LastWatchDateTest, shouldCreateLastWatchDateThatIsNow) {
    EXPECT_LE(serio::core::LastWatchDate(now), serio::core::LastWatchDate());
}
