#include <gtest/gtest.h>
#include <storage/DatabaseTvShowStorage.h>

class TvShowStorageTest : public ::testing::Test {
public:
    TvShowStorageTest()
        : ::testing::Test(),
            fourthTvShow("How i met your mom", "", std::chrono::system_clock::now() - std::chrono::hours(32)),
            thirdTvShow("Mandalorian", "", std::chrono::system_clock::now()),
            secondTvShow("Friends"),
            firstTvShow("Clinic") {
        storage.initialize(":memory:");
    }
protected:
    serio::qt::DatabaseTvShowStorage storage;
    serio::core::TvShow fourthTvShow, thirdTvShow, secondTvShow, firstTvShow;
    const unsigned int LIMIT_ONE_ITEM = 1;
    void saveShows() {
        storage.saveTvShow(secondTvShow);
        storage.saveTvShow(firstTvShow);
    }
    void saveWatchedShows() {
        storage.saveTvShow(fourthTvShow);
        storage.saveTvShow(thirdTvShow);
    }
};

TEST_F(TvShowStorageTest, returnedListOfAllTvShowsShouldBeEmptyIfThereAreNoTvShows) {
    serio::core::ListPage<serio::core::TvShow> tvShows = storage.getAllTvShows(0, 100);
    EXPECT_EQ(0, tvShows.getTotalSize());
}

TEST_F(TvShowStorageTest, returnedListOfAllTvShowsShouldUseSpecifiedOffsetAndLimit) {
    saveShows();
    serio::core::ListPage<serio::core::TvShow> tvShows = storage.getAllTvShows(1, 1);
    EXPECT_EQ(1, tvShows.getOffset());
    EXPECT_EQ(tvShows.getFirstItemIndex(), tvShows.getLastItemIndex());
}

TEST_F(TvShowStorageTest, returnedListOfAllTvShowsShouldContainLimitedAmountOfTvShows) {
    saveShows();
    serio::core::ListPage<serio::core::TvShow> tvShows = storage.getAllTvShows(0, LIMIT_ONE_ITEM);
    EXPECT_EQ(2, tvShows.getTotalSize());
    EXPECT_EQ(firstTvShow, tvShows.getItemByGlobalIndex(0));
}

TEST_F(TvShowStorageTest, returnedListOfAllTvShowsShouldStartFromTheSecondTvShowInAlphabeticalOrder) {
    saveShows();
    serio::core::ListPage<serio::core::TvShow> tvShows = storage.getAllTvShows(1, LIMIT_ONE_ITEM);
    EXPECT_EQ(secondTvShow, tvShows.getItemByGlobalIndex(1));
}

TEST_F(TvShowStorageTest, returnedListOfAllTvShowsShouldContainAllStoredTvShowsInTheirAplhabeticalOrder) {
    saveShows();
    serio::core::ListPage<serio::core::TvShow> tvShows = storage.getAllTvShows(0, 10);
    EXPECT_EQ(firstTvShow, tvShows.getItemByGlobalIndex(0));
    EXPECT_EQ(secondTvShow, tvShows.getItemByGlobalIndex(1));
}

TEST_F(TvShowStorageTest, savingShowWithNameThatAlreadyExistsWillReplaceExistingShow) {
    storage.saveTvShow(firstTvShow);
    serio::core::TvShow updatedFirstShow(firstTvShow.getName(), "https://thumbnail.jpg");
    storage.saveTvShow(updatedFirstShow);
    serio::core::ListPage<serio::core::TvShow> tvShows = storage.getAllTvShows(0, 10);
    EXPECT_EQ(updatedFirstShow, tvShows.getItemByGlobalIndex(0));
}

TEST_F(TvShowStorageTest, shouldReturnEmptyListIfNoTvShowsHaveBeenWatched) {
    saveShows();
    serio::core::ListPage<serio::core::TvShow> watchedTvShows = storage.getWatchedTvShows(0, 10);
    EXPECT_EQ(0, watchedTvShows.getTotalSize());
}

TEST_F(TvShowStorageTest, shouldReturnTheOffsetUsedToQueryWatchedTvShows) {
    saveWatchedShows();
    unsigned int offset = 1;
    serio::core::ListPage<serio::core::TvShow> watchedTvShows = storage.getWatchedTvShows(offset, 10);
    EXPECT_EQ(offset, watchedTvShows.getOffset());
}

TEST_F(TvShowStorageTest, shouldReturnWatchedTvShowsWithTheLatestWatchedTvShowBeingFirstAndTheLatestLast) {
    saveWatchedShows();
    serio::core::ListPage<serio::core::TvShow> watchedTvShows = storage.getWatchedTvShows(0, 10);
    EXPECT_EQ(thirdTvShow, watchedTvShows.getItemByGlobalIndex(0));
    EXPECT_EQ(fourthTvShow, watchedTvShows.getItemByGlobalIndex(1));
}

TEST_F(TvShowStorageTest, shouldLimitCountOfReturnedWatchedTvShows) {
    saveWatchedShows();
    serio::core::ListPage<serio::core::TvShow> watchedTvShow = storage.getWatchedTvShows(0, 1);
    EXPECT_TRUE(watchedTvShow.containsItemWithGlobalIndex(0));
    EXPECT_FALSE(watchedTvShow.containsItemWithGlobalIndex(1));
}
