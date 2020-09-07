#include <gtest/gtest.h>
#include <database-storage/DatabaseStorage.h>

class DatabaseTvShowStorageTest : public ::testing::Test {
public:
    DatabaseTvShowStorageTest()
        : ::testing::Test(),
            fourthTvShow("How i met your mom", "", serio::core::LastWatchDate(std::chrono::system_clock::now() - std::chrono::hours(32))),
            thirdTvShow("Mandalorian", "", serio::core::LastWatchDate(std::chrono::system_clock::now())),
            secondTvShow("Friends"),
            firstTvShow("Clinic") {
        storage.initialize(":memory:");
    }
protected:
    serio::qt::DatabaseStorage storage;
    serio::core::TvShow fourthTvShow, thirdTvShow, secondTvShow, firstTvShow;
    std::vector<serio::core::Episode> firstTvShowEpisodes = {serio::core::Episode(1, "")};
    std::vector<serio::core::Episode> secondTvShowEpisodes = {serio::core::Episode(1, "", "Pilot episode"), serio::core::Episode(2, "")};
    const unsigned int LIMIT_ONE_ITEM = 1;
    void saveShows() {
        storage.saveTvShow(secondTvShow, secondTvShowEpisodes);
        storage.saveTvShow(firstTvShow, firstTvShowEpisodes);
    }
    void saveWatchedShows() {
        storage.saveTvShow(fourthTvShow, {});
        storage.saveTvShow(thirdTvShow, {});
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
    storage.saveTvShow(firstTvShow, {});
    serio::core::TvShow updatedFirstShow(firstTvShow.getName(), "https://thumbnail.jpg");
    storage.saveTvShow(updatedFirstShow, {});
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

TEST_F(DatabaseTvShowStorageTest, tvShowShouldNotHaveEpisodes) {
    storage.saveTvShow(firstTvShow, {});
    serio::core::ListPage<serio::core::Episode> page = storage.getEpisodesOfTvShowWithName(firstTvShow.getName(), 0, 10);
    EXPECT_EQ(0, page.getTotalSize());
}

TEST_F(DatabaseTvShowStorageTest, tvShowShouldHaveEpisodes) {
    saveShows();
    serio::core::ListPage<serio::core::Episode> page = storage.getEpisodesOfTvShowWithName(firstTvShow.getName(), 0, 10);
    EXPECT_EQ(1, page.getTotalSize());
}

TEST_F(DatabaseTvShowStorageTest, returnedListPageOfEpisodesShouldHaveSpecifiedOffset) {
    saveShows();
    unsigned int expectedOffset = 1;
    serio::core::ListPage<serio::core::Episode> page = storage.getEpisodesOfTvShowWithName(firstTvShow.getName(), expectedOffset, 10);
    EXPECT_EQ(expectedOffset, page.getOffset());
}

TEST_F(DatabaseTvShowStorageTest, returnedListPageShouldBeLimitedToTheSpecifiedCountOfEpisodes) {
    saveShows();
    serio::core::ListPage<serio::core::Episode> page = storage.getEpisodesOfTvShowWithName(secondTvShow.getName(), 0, LIMIT_ONE_ITEM);
    EXPECT_TRUE(page.containsItemWithGlobalIndex(0));
    EXPECT_FALSE(page.containsItemWithGlobalIndex(1));
}

TEST_F(DatabaseTvShowStorageTest, shouldReturnAllEpisodesOfTvShow) {
    saveShows();
    serio::core::ListPage<serio::core::Episode> page = storage.getEpisodesOfTvShowWithName(secondTvShow.getName(), 0, 10);
    EXPECT_EQ(secondTvShowEpisodes[0], page.getItemByGlobalIndex(0));
    EXPECT_EQ(secondTvShowEpisodes[1], page.getItemByGlobalIndex(1));
}

TEST_F(DatabaseTvShowStorageTest, shouldOverrideExistingTvShowEpisodesWithNewOnes) {
    saveShows();
    storage.saveTvShow(secondTvShow, firstTvShowEpisodes);
    serio::core::ListPage<serio::core::Episode> page = storage.getEpisodesOfTvShowWithName(secondTvShow.getName(), 0, 10);
    EXPECT_EQ(firstTvShowEpisodes[0], page.getItemByGlobalIndex(0));
    EXPECT_FALSE(page.containsItemWithGlobalIndex(1));
}

TEST_F(DatabaseTvShowStorageTest, shouldReturnEpisodesOrderedById) {
    std::vector<serio::core::Episode> episodesInWrongOrder = secondTvShowEpisodes;
    std::reverse(episodesInWrongOrder.begin(), episodesInWrongOrder.end());
    storage.saveTvShow(secondTvShow, episodesInWrongOrder);
    serio::core::ListPage<serio::core::Episode> page = storage.getEpisodesOfTvShowWithName(secondTvShow.getName(), 0, 10);
    EXPECT_EQ(secondTvShowEpisodes[0], page.getItemByGlobalIndex(0));
    EXPECT_EQ(secondTvShowEpisodes[1], page.getItemByGlobalIndex(1));
}