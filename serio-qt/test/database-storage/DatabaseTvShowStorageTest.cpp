#include <gtest/gtest.h>
#include <database-storage/DatabaseStorage.h>

class DatabaseTvShowStorageTest : public ::testing::Test {
protected:
    serio::qt::DatabaseStorage storage;
    const std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    const serio::core::LastWatchDate watchDate;
    const serio::core::WatchProgress watchProgress = serio::core::WatchProgress(15);
    const serio::core::TvShow fourthTvShow = serio::core::TvShow("How i met your mom", "", serio::core::LastWatchDate(now - std::chrono::hours(32)));
    const serio::core::TvShow thirdTvShow = serio::core::TvShow("Mandalorian", "", serio::core::LastWatchDate(now - std::chrono::hours(1)));
    const serio::core::TvShow secondTvShow = serio::core::TvShow("Friends");
    const serio::core::TvShow firstTvShow = serio::core::TvShow("Clinic");
    const std::vector<serio::core::Episode> firstTvShowEpisodes = {serio::core::Episode(1, "")};
    const std::vector<serio::core::Episode> secondTvShowEpisodes = {
        serio::core::Episode(1, "", "Pilot episode"),
        serio::core::Episode(2, "")
    };
    const std::vector<serio::core::Episode> thirdTvShowEpisodes = {
            serio::core::Episode(1, "", "Pilot episode", thirdTvShow.getLastWatchDate(), watchProgress),
            serio::core::Episode(2, "")
    };
    const std::vector<serio::core::Episode> fourthTvShowEpisodes = {
            serio::core::Episode(1, "", "Pilot episode", fourthTvShow.getLastWatchDate(), watchProgress),
            serio::core::Episode(2, "")
    };
    const unsigned int LIMIT_ONE_ITEM = 1;
    virtual void SetUp() {
        storage.initialize(true);
    }
    void saveShows() {
        storage.saveTvShow(secondTvShow, secondTvShowEpisodes);
        storage.saveTvShow(firstTvShow, firstTvShowEpisodes);
    }
    void saveWatchedShows() {
        storage.saveTvShow(fourthTvShow, fourthTvShowEpisodes);
        storage.watchTvShowEpisode(fourthTvShow.getName(), fourthTvShowEpisodes[0].getId(),
                                   *fourthTvShow.getLastWatchDate(), watchProgress);
        storage.saveTvShow(thirdTvShow, thirdTvShowEpisodes);
        storage.watchTvShowEpisode(thirdTvShow.getName(), thirdTvShowEpisodes[0].getId(),
                                   *thirdTvShow.getLastWatchDate(), watchProgress);
    }
};

TEST_F(DatabaseTvShowStorageTest, returnedListOfAllTvShowsShouldBeEmptyIfThereAreNoTvShows) {
    auto tvShows = storage.getAllTvShows(0, 100);
    EXPECT_EQ(0, tvShows.getTotalSize());
}

TEST_F(DatabaseTvShowStorageTest, returnedListOfAllTvShowsShouldUseSpecifiedOffsetAndLimit) {
    saveShows();
    auto tvShows = storage.getAllTvShows(1, 1);
    EXPECT_EQ(1, tvShows.getOffset());
    EXPECT_EQ(tvShows.getFirstItemIndex(), tvShows.getLastItemIndex());
}

TEST_F(DatabaseTvShowStorageTest, returnedListOfAllTvShowsShouldContainLimitedAmountOfTvShows) {
    saveShows();
    auto tvShows = storage.getAllTvShows(0, LIMIT_ONE_ITEM);
    EXPECT_EQ(2, tvShows.getTotalSize());
    EXPECT_EQ(firstTvShow, tvShows.getItemByGlobalIndex(0));
}

TEST_F(DatabaseTvShowStorageTest, returnedListOfAllTvShowsShouldStartFromTheSecondTvShowInAlphabeticalOrder) {
    saveShows();
    auto tvShows = storage.getAllTvShows(1, LIMIT_ONE_ITEM);
    EXPECT_EQ(secondTvShow, tvShows.getItemByGlobalIndex(1));
}

TEST_F(DatabaseTvShowStorageTest, returnedListOfAllTvShowsShouldContainAllStoredTvShowsInTheirAplhabeticalOrder) {
    saveShows();
    auto tvShows = storage.getAllTvShows(0, 10);
    EXPECT_EQ(firstTvShow, tvShows.getItemByGlobalIndex(0));
    EXPECT_EQ(secondTvShow, tvShows.getItemByGlobalIndex(1));
}

TEST_F(DatabaseTvShowStorageTest, savingShowWithNameThatAlreadyExistsWillReplaceExistingShow) {
    storage.saveTvShow(firstTvShow, {});
    serio::core::TvShow updatedFirstShow(firstTvShow.getName(), "https://thumbnail.jpg");
    storage.saveTvShow(updatedFirstShow, {});
    auto tvShows = storage.getAllTvShows(0, 10);
    EXPECT_EQ(updatedFirstShow, tvShows.getItemByGlobalIndex(0));
}

TEST_F(DatabaseTvShowStorageTest, shouldReturnEmptyListIfNoTvShowsHaveBeenWatched) {
    saveShows();
    auto watchedTvShows = storage.getWatchedTvShows(0, 10);
    EXPECT_EQ(0, watchedTvShows.getTotalSize());
}

TEST_F(DatabaseTvShowStorageTest, shouldReturnTheOffsetUsedToQueryWatchedTvShows) {
    saveWatchedShows();
    auto offset = 1;
    auto watchedTvShows = storage.getWatchedTvShows(offset, 10);
    EXPECT_EQ(offset, watchedTvShows.getOffset());
}

TEST_F(DatabaseTvShowStorageTest, shouldReturnWatchedTvShowsWithTheLatestWatchedTvShowBeingFirstAndTheLatestLast) {
    saveWatchedShows();
    storage.watchTvShowEpisode(fourthTvShow.getName(), 2, watchDate, watchProgress);
    serio::core::TvShow fourthTvShowWithNewLastWatchDate(fourthTvShow.getName(), fourthTvShow.getThumbnailUrl(), watchDate);
    auto watchedTvShows = storage.getWatchedTvShows(0, 10);
    EXPECT_EQ(fourthTvShowWithNewLastWatchDate, watchedTvShows.getItemByGlobalIndex(0));
    EXPECT_EQ(thirdTvShow, watchedTvShows.getItemByGlobalIndex(1));
    EXPECT_EQ(2, watchedTvShows.getTotalSize());
}

TEST_F(DatabaseTvShowStorageTest, shouldLimitCountOfReturnedWatchedTvShows) {
    saveWatchedShows();
    auto watchedTvShow = storage.getWatchedTvShows(0, 1);
    EXPECT_TRUE(watchedTvShow.containsItemWithGlobalIndex(0));
    EXPECT_FALSE(watchedTvShow.containsItemWithGlobalIndex(1));
}

TEST_F(DatabaseTvShowStorageTest, tvShowShouldNotHaveEpisodes) {
    storage.saveTvShow(firstTvShow, {});
    auto page = storage.getEpisodesOfTvShowWithName(firstTvShow.getName(), 0, 10);
    EXPECT_EQ(0, page.getTotalSize());
}

TEST_F(DatabaseTvShowStorageTest, tvShowShouldHaveEpisodes) {
    saveShows();
    auto page = storage.getEpisodesOfTvShowWithName(firstTvShow.getName(), 0, 10);
    EXPECT_EQ(1, page.getTotalSize());
}

TEST_F(DatabaseTvShowStorageTest, returnedListPageOfEpisodesShouldHaveSpecifiedOffset) {
    saveShows();
    auto expectedOffset = 1;
    auto page = storage.getEpisodesOfTvShowWithName(firstTvShow.getName(), expectedOffset, 10);
    EXPECT_EQ(expectedOffset, page.getOffset());
}

TEST_F(DatabaseTvShowStorageTest, returnedListPageShouldBeLimitedToTheSpecifiedCountOfEpisodes) {
    saveShows();
    auto page = storage.getEpisodesOfTvShowWithName(secondTvShow.getName(), 0, LIMIT_ONE_ITEM);
    EXPECT_TRUE(page.containsItemWithGlobalIndex(0));
    EXPECT_FALSE(page.containsItemWithGlobalIndex(1));
}

TEST_F(DatabaseTvShowStorageTest, shouldReturnAllEpisodesOfTvShow) {
    saveShows();
    auto page = storage.getEpisodesOfTvShowWithName(secondTvShow.getName(), 0, 10);
    EXPECT_EQ(secondTvShowEpisodes[0], page.getItemByGlobalIndex(0));
    EXPECT_EQ(secondTvShowEpisodes[1], page.getItemByGlobalIndex(1));
}

TEST_F(DatabaseTvShowStorageTest, shouldOverrideExistingTvShowEpisodesWithNewOnes) {
    saveShows();
    storage.saveTvShow(secondTvShow, firstTvShowEpisodes);
    auto page = storage.getEpisodesOfTvShowWithName(secondTvShow.getName(), 0, 10);
    EXPECT_EQ(firstTvShowEpisodes[0], page.getItemByGlobalIndex(0));
    EXPECT_FALSE(page.containsItemWithGlobalIndex(1));
}

TEST_F(DatabaseTvShowStorageTest, shouldReturnEpisodesOrderedById) {
    auto episodesInWrongOrder = secondTvShowEpisodes;
    std::reverse(episodesInWrongOrder.begin(), episodesInWrongOrder.end());
    storage.saveTvShow(secondTvShow, episodesInWrongOrder);
    auto page = storage.getEpisodesOfTvShowWithName(secondTvShow.getName(), 0, 10);
    EXPECT_EQ(secondTvShowEpisodes[0], page.getItemByGlobalIndex(0));
    EXPECT_EQ(secondTvShowEpisodes[1], page.getItemByGlobalIndex(1));
}

TEST_F(DatabaseTvShowStorageTest, shouldNotFindTvShowWithName) {
    EXPECT_FALSE(storage.getTvShowByName(firstTvShow.getName()));
}

TEST_F(DatabaseTvShowStorageTest, shouldFindTvShowWithName) {
    saveShows();
    EXPECT_EQ(firstTvShow, *storage.getTvShowByName(firstTvShow.getName()));
}

TEST_F(DatabaseTvShowStorageTest, shouldClearLastWatchDatesOfSpecifiedTvShow) {
    saveWatchedShows();
    storage.clearTvShowWatchHistory(thirdTvShow.getName());
    EXPECT_FALSE(storage.getTvShowByName(thirdTvShow.getName())->getLastWatchDate());
    EXPECT_TRUE(storage.getTvShowByName(fourthTvShow.getName())->getLastWatchDate());
}

TEST_F(DatabaseTvShowStorageTest, shouldClearLastWatchDatesOfEpisodesOfSpecifiedTvShow) {
    saveWatchedShows();
    storage.clearTvShowWatchHistory(thirdTvShow.getName());
    auto affectedEpisodes = storage.getEpisodesOfTvShowWithName(thirdTvShow.getName(), 0, 10);
    EXPECT_FALSE(affectedEpisodes.getItemByGlobalIndex(0).getLastWatchDate());
    auto unaffectedEpisodes = storage.getEpisodesOfTvShowWithName(fourthTvShow.getName(), 0, 10);
    EXPECT_TRUE(unaffectedEpisodes.getItemByGlobalIndex(0).getLastWatchDate());
}

TEST_F(DatabaseTvShowStorageTest, shouldDeleteSpecifiedTvShow) {
    saveShows();
    storage.deleteTvShow(firstTvShow.getName());
    EXPECT_FALSE(storage.getTvShowByName(firstTvShow.getName()));
    EXPECT_TRUE(storage.getTvShowByName(secondTvShow.getName()));
}

TEST_F(DatabaseTvShowStorageTest, shouldDeleteEpisodesOfTvShow) {
    saveShows();
    storage.deleteTvShow(secondTvShow.getName());
    EXPECT_EQ(firstTvShowEpisodes.size(), storage.getEpisodesOfTvShowWithName(firstTvShow.getName(), 0, 10).getTotalSize());
    EXPECT_EQ(0, storage.getEpisodesOfTvShowWithName(secondTvShow.getName(), 0, 10).getTotalSize());
}

TEST_F(DatabaseTvShowStorageTest, shouldNotFindEpisodeSinceItDoesNotExist) {
    EXPECT_FALSE(storage.getEpisodeOfTvShowWithName(firstTvShow.getName(), 1));
}

TEST_F(DatabaseTvShowStorageTest, shouldFindSpecifiedEpisode) {
    saveShows();
    EXPECT_EQ(firstTvShowEpisodes[0], *storage.getEpisodeOfTvShowWithName(firstTvShow.getName(), 1));
}

TEST_F(DatabaseTvShowStorageTest, shouldUpdateLastWatchDateOfTvShow) {
    saveShows();
    storage.watchTvShowEpisode(firstTvShow.getName(), firstTvShowEpisodes[0].getId(), watchDate,
                               watchProgress);
    auto tvShow = storage.getTvShowByName(firstTvShow.getName());
    ASSERT_TRUE(tvShow->getLastWatchDate());
    EXPECT_EQ(watchDate, tvShow->getLastWatchDate());
    EXPECT_FALSE(storage.getTvShowByName(secondTvShow.getName())->getLastWatchDate());
}

TEST_F(DatabaseTvShowStorageTest, shouldUpdateLastWatchDateOfTvShowsEpisode) {
    saveShows();
    storage.watchTvShowEpisode(secondTvShow.getName(), secondTvShowEpisodes[1].getId(), watchDate,
                               watchProgress);
    auto episode = storage.getEpisodeOfTvShowWithName(secondTvShow.getName(), secondTvShowEpisodes[1].getId());
    ASSERT_TRUE(episode->getLastWatchDate());
    EXPECT_EQ(watchDate, episode->getLastWatchDate());
    EXPECT_FALSE(storage.getEpisodeOfTvShowWithName(secondTvShow.getName(), secondTvShowEpisodes[0].getId())->getLastWatchDate());
    EXPECT_FALSE(storage.getEpisodeOfTvShowWithName(firstTvShow.getName(), firstTvShowEpisodes[0].getId())->getLastWatchDate());
}

TEST_F(DatabaseTvShowStorageTest, shouldStartWatchProgressOfTvShowEpisode) {
    saveShows();
    storage.watchTvShowEpisode(secondTvShow.getName(), secondTvShowEpisodes[0].getId(), watchDate,
                               watchProgress);
    EXPECT_EQ(watchProgress, storage.getEpisodeOfTvShowWithName(secondTvShow.getName(), secondTvShowEpisodes[0].getId())->getWatchProgress());
    EXPECT_EQ(serio::core::WatchProgress(), storage.getEpisodeOfTvShowWithName(secondTvShow.getName(), secondTvShowEpisodes[1].getId())->getWatchProgress());
    EXPECT_EQ(serio::core::WatchProgress(), storage.getEpisodeOfTvShowWithName(firstTvShow.getName(), firstTvShowEpisodes[0].getId())->getWatchProgress());
}

TEST_F(DatabaseTvShowStorageTest, clearingWatchHistoryOfTvShowShouldAlsoResetWatchProgressOfAllEpisodes) {
    const auto& tvShow = secondTvShow;
    const auto& episodes = secondTvShowEpisodes;
    saveShows();
    storage.watchTvShowEpisode(tvShow.getName(), episodes[0].getId(), watchDate, watchProgress);
    storage.watchTvShowEpisode(tvShow.getName(), episodes[1].getId(), watchDate, watchProgress);
    storage.clearTvShowWatchHistory(tvShow.getName());
    EXPECT_EQ(serio::core::WatchProgress(), storage.getEpisodeOfTvShowWithName(tvShow.getName(), episodes[0].getId())->getWatchProgress());
    EXPECT_EQ(serio::core::WatchProgress(), storage.getEpisodeOfTvShowWithName(tvShow.getName(), episodes[1].getId())->getWatchProgress());
}

TEST_F(DatabaseTvShowStorageTest, watchHistoryOfTvShowAndItsEpisodesShouldNotBeClearedWhenDeletingTheTvShow) {
    saveWatchedShows();
    storage.deleteTvShow(thirdTvShow.getName());
    storage.saveTvShow(thirdTvShow, thirdTvShowEpisodes);
    EXPECT_EQ(thirdTvShow, *storage.getTvShowByName(thirdTvShow.getName()));
    EXPECT_EQ(thirdTvShowEpisodes[0], *storage.getEpisodeOfTvShowWithName(thirdTvShow.getName(), thirdTvShowEpisodes[0].getId()));
    EXPECT_EQ(thirdTvShowEpisodes[1], *storage.getEpisodeOfTvShowWithName(thirdTvShow.getName(), thirdTvShowEpisodes[1].getId()));
}

TEST_F(DatabaseTvShowStorageTest, shouldUpdateWatchProgressOfTvShowEpisode) {
    auto tvShowName = thirdTvShow.getName();
    auto episodeId = thirdTvShowEpisodes[0].getId();
    serio::core::WatchProgress completeWatchProgress(100);
    saveWatchedShows();
    storage.watchTvShowEpisode(tvShowName, episodeId, watchDate, completeWatchProgress);
    EXPECT_EQ(completeWatchProgress, storage.getEpisodeOfTvShowWithName(tvShowName, episodeId)->getWatchProgress());
}
