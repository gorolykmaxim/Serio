#include <gtest/gtest.h>
#include <user-interface/view-model/AllTvShowsViewModel.h>
#include <TvShowStorageMock.h>

class AllTvShowsViewModelTest : public ::testing::Test {
protected:
    unsigned int pageSize = 100;
    TvShowStorageMock storage;
    QModelIndex index;
    serio::core::ListPage<serio::core::TvShow> page = serio::core::ListPage<serio::core::TvShow>(0, 300, {});
    serio::qt::AllTvShowsViewModel viewModel = serio::qt::AllTvShowsViewModel(pageSize, 2, storage);
};

TEST_F(AllTvShowsViewModelTest, shouldHaveSpecifiedPageSize) {
    EXPECT_EQ(pageSize, viewModel.getPageSize());
}

TEST_F(AllTvShowsViewModelTest, shouldHaveEmptyAllTvShowsListModelOnCreation) {
    EXPECT_EQ(0, viewModel.getAllShows()->rowCount(index));
}

TEST_F(AllTvShowsViewModelTest, shouldHaveEmptyWatchedTvShowsListModelOnCreation) {
    EXPECT_EQ(0, viewModel.getWatchedShows()->rowCount(index));
}

TEST_F(AllTvShowsViewModelTest, shouldLoadPageOfAllTvShowsWithSpecifiedOffsetAndLimit) {
    EXPECT_CALL(storage, getAllTvShows(0, pageSize))
        .WillOnce(::testing::Return(page));
    viewModel.loadAllShows(QVariantList({0, pageSize}));
    EXPECT_EQ(page.getTotalSize(), viewModel.getAllShows()->rowCount(index));
}

TEST_F(AllTvShowsViewModelTest, shouldLoadPageOfWatchedTvShowsWithSpecifiedOffsetAndLimit) {
    EXPECT_CALL(storage, getWatchedTvShows(0, pageSize))
            .WillOnce(::testing::Return(page));
    viewModel.loadWatchedShows(QVariantList({0, pageSize}));
    EXPECT_EQ(page.getTotalSize(), viewModel.getWatchedShows()->rowCount(index));
}
