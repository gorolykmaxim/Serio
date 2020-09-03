#include <gtest/gtest.h>
#include <tv-show-storage/TvShow.h>
#include <tv-show-storage/LastWatchDate.h>

class TvShowTest : public ::testing::Test {
protected:
    const std::string SHOW_NAME = "Friends";
    const std::string THUMBNAIL_URL = "https://vignette.wikia.nocookie.net/sitcom/images/6/6e/Friends1.jpg/revision/latest?cb=20180225103536&path-prefix=ru";
    const serio::core::LastWatchDate lastWatchDate = serio::core::LastWatchDate(std::chrono::system_clock::now());
};

TEST_F(TvShowTest, createdTvShowShouldHaveSpecifiedName) {
    serio::core::TvShow tvShow(SHOW_NAME);
    ASSERT_EQ(SHOW_NAME, tvShow.getName());
}

TEST_F(TvShowTest, tvShowCreatedWithoutThumbnailUrlShouldHaveItAsEmptyString) {
    serio::core::TvShow tvShow(SHOW_NAME);
    ASSERT_TRUE(tvShow.getThumbnailUrl().empty());
}

TEST_F(TvShowTest, tvShowCreatedWithThumbnailUrlShouldUseSpecifiedValue) {
    serio::core::TvShow tvShow(SHOW_NAME, THUMBNAIL_URL);
    ASSERT_EQ(THUMBNAIL_URL, tvShow.getThumbnailUrl());
}

TEST_F(TvShowTest, tvShowCreatedWithoutLastWatchDateShouldReturnEmptyOptionalAsLastWatchDate) {
    serio::core::TvShow tvShow(SHOW_NAME, THUMBNAIL_URL);
    ASSERT_FALSE(tvShow.getLastWatchDate());
}

TEST_F(TvShowTest, tvShowCreatedWithLastWatchDateShouldReturnItAsLastWatchDate) {
    serio::core::TvShow tvShow(SHOW_NAME, THUMBNAIL_URL, lastWatchDate);
    ASSERT_EQ(lastWatchDate, *tvShow.getLastWatchDate());
}

TEST_F(TvShowTest, twoTvShowsWithTheSameNameThumbnailUrlAndLastWatchDateShouldBeEqual) {
    serio::core::TvShow oneShow(SHOW_NAME, THUMBNAIL_URL, lastWatchDate), anotherShow(SHOW_NAME, THUMBNAIL_URL, lastWatchDate);
    ASSERT_EQ(oneShow, anotherShow);
}

TEST_F(TvShowTest, twoTvShowsWithDifferentNamesShouldNotBeEqual) {
    serio::core::TvShow oneShow(SHOW_NAME, THUMBNAIL_URL, lastWatchDate), anotherShow("Clinic", THUMBNAIL_URL, lastWatchDate);
    ASSERT_NE(oneShow, anotherShow);
}

TEST_F(TvShowTest, twoTvShowsWithDifferentThumnbailUrlsShouldNotBeEqual) {
    serio::core::TvShow oneShow(SHOW_NAME, THUMBNAIL_URL, lastWatchDate), anotherShow(SHOW_NAME, "", lastWatchDate);
    ASSERT_NE(oneShow, anotherShow);
}

TEST_F(TvShowTest, twoTvShowsWithDifferentLastWatchDatesShouldNotBeEqual) {
    serio::core::LastWatchDate oneDate = serio::core::LastWatchDate(std::chrono::system_clock::now());
    serio::core::LastWatchDate anotherDate = serio::core::LastWatchDate(std::chrono::system_clock::now() + std::chrono::hours(2));
    serio::core::TvShow oneShow(SHOW_NAME, THUMBNAIL_URL, oneDate), anotherShow(SHOW_NAME, THUMBNAIL_URL, anotherDate);
    ASSERT_NE(oneShow, anotherShow);
}

TEST_F(TvShowTest, tvShowWithLastWatchDateNotSpecifiedShouldNotBeEqualToTvShowWithLastWatchDateSpecified) {
    serio::core::TvShow oneShow(SHOW_NAME, THUMBNAIL_URL, lastWatchDate), anotherShow(SHOW_NAME);
    ASSERT_NE(oneShow, anotherShow);
}

TEST_F(TvShowTest, twoTvShowWithSameNameThumbnailUrlAndNoLastWatchDateShouldBeEqual) {
    serio::core::TvShow oneShow(SHOW_NAME, THUMBNAIL_URL), anotherShow(SHOW_NAME, THUMBNAIL_URL);
    ASSERT_EQ(oneShow, anotherShow);
}