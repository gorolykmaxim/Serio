#include <gtest/gtest.h>
#include <database-storage/DatabaseStorage.h>

class DatabaseTvShowStorageTest : public ::testing::Test {
public:
    DatabaseTvShowStorageTest()
        : ::testing::Test(),
            fourthTvShow("How i met your mom", "", std::chrono::system_clock::now() - std::chrono::hours(32)),
            thirdTvShow("Mandalorian", "", std::chrono::system_clock::now()),
            secondTvShow("Friends"),
            firstTvShow("Clinic") {
        storage.initialize(":memory:");
    }
protected:
    serio::qt::DatabaseStorage storage;
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

TEST_F(DatabaseTvShowStorageTest, returnedListOfAllTvShowsShouldBeEmptyIfThereAreNoTvShows) {
    serio::core::ListPage<serio::core::TvShow> tvShows = storage.getAllTvShows(0, 100);
    EXPECT_EQ(0, tvShows.getTotalSize());
}

TEST_F(DatabaseTvShowStorageTest, returnedListOfAllTvShowsShouldUseSpecifiedOffsetAndLimit) {
    saveShows();
    serio::core::ListPage<serio::core::TvShow> tvShows = storage.getAllTvShows(1, 1);
    EXPECT_EQ(1, tvShows.getOffset());
    EXPECT_EQ(tvShows.getFirstItemIndex(), tvShows.getLastItemIndex());
}

TEST_F(DatabaseTvShowStorageTest, returnedListOfAllTvShowsShouldContainLimitedAmountOfTvShows) {
    saveShows();
    serio::core::ListPage<serio::core::TvShow> tvShows = storage.getAllTvShows(0, LIMIT_ONE_ITEM);
    EXPECT_EQ(2, tvShows.getTotalSize());
    EXPECT_EQ(firstTvShow, tvShows.getItemByGlobalIndex(0));
}

TEST_F(DatabaseTvShowStorageTest, returnedListOfAllTvShowsShouldStartFromTheSecondTvShowInAlphabeticalOrder) {
    saveShows();
    serio::core::ListPage<serio::core::TvShow> tvShows = storage.getAllTvShows(1, LIMIT_ONE_ITEM);
    EXPECT_EQ(secondTvShow, tvShows.getItemByGlobalIndex(1));
}

TEST_F(DatabaseTvShowStorageTest, returnedListOfAllTvShowsShouldContainAllStoredTvShowsInTheirAplhabeticalOrder) {
    saveShows();
    serio::core::ListPage<serio::core::TvShow> tvShows = storage.getAllTvShows(0, 10);
    EXPECT_EQ(firstTvShow, tvShows.getItemByGlobalIndex(0));
    EXPECT_EQ(secondTvShow, tvShows.getItemByGlobalIndex(1));
}

TEST_F(DatabaseTvShowStorageTest, savingShowWithNameThatAlreadyExistsWillReplaceExistingShow) {
    storage.saveTvShow(firstTvShow);
    serio::core::TvShow updatedFirstShow(firstTvShow.getName(), "https://thumbnail.jpg");
    storage.saveTvShow(updatedFirstShow);
    serio::core::ListPage<serio::core::TvShow> tvShows = storage.getAllTvShows(0, 10);
    EXPECT_EQ(updatedFirstShow, tvShows.getItemByGlobalIndex(0));
}

TEST_F(DatabaseTvShowStorageTest, shouldReturnEmptyListIfNoTvShowsHaveBeenWatched) {
    saveShows();
    serio::core::ListPage<serio::core::TvShow> watchedTvShows = storage.getWatchedTvShows(0, 10);
    EXPECT_EQ(0, watchedTvShows.getTotalSize());
}

TEST_F(DatabaseTvShowStorageTest, shouldReturnTheOffsetUsedToQueryWatchedTvShows) {
    saveWatchedShows();
    unsigned int offset = 1;
    serio::core::ListPage<serio::core::TvShow> watchedTvShows = storage.getWatchedTvShows(offset, 10);
    EXPECT_EQ(offset, watchedTvShows.getOffset());
}

TEST_F(DatabaseTvShowStorageTest, shouldReturnWatchedTvShowsWithTheLatestWatchedTvShowBeingFirstAndTheLatestLast) {
    saveWatchedShows();
    serio::core::ListPage<serio::core::TvShow> watchedTvShows = storage.getWatchedTvShows(0, 10);
    EXPECT_EQ(thirdTvShow, watchedTvShows.getItemByGlobalIndex(0));
    EXPECT_EQ(fourthTvShow, watchedTvShows.getItemByGlobalIndex(1));
}

TEST_F(DatabaseTvShowStorageTest, shouldLimitCountOfReturnedWatchedTvShows) {
    saveWatchedShows();
    serio::core::ListPage<serio::core::TvShow> watchedTvShow = storage.getWatchedTvShows(0, 1);
    EXPECT_TRUE(watchedTvShow.containsItemWithGlobalIndex(0));
    EXPECT_FALSE(watchedTvShow.containsItemWithGlobalIndex(1));
}
