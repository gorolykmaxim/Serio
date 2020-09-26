#include <gtest/gtest.h>
#include <user-interface/view-model/AllTvShowsViewModel.h>
#include <TvShowStorageMock.h>
#include <StackOfViewsMock.h>
#include <TvShowViewerMock.h>
#include <QSignalSpy>
#include <user-interface/ViewNames.h>

class AllTvShowsViewModelTest : public ::testing::Test {
protected:
    unsigned int pageSize = 100;
    TvShowViewerMock viewer = TvShowViewerMock::create();
    StackOfViewsMock stack;
    TvShowStorageMock storage;
    QModelIndex index;
    serio::core::ListPage<serio::core::TvShow> page = serio::core::ListPage<serio::core::TvShow>(0, 300, {});
    serio::qt::AllTvShowsViewModel viewModel = serio::qt::AllTvShowsViewModel(pageSize, 2, storage, viewer, stack);
};

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

TEST_F(AllTvShowsViewModelTest, shouldOpenTvShowViewWhileSelectingSpecifiedTvShowInViewer) {
    std::string tvShowName = "Breaking Bad";
    EXPECT_CALL(viewer, openTvShowWithName(tvShowName));
    EXPECT_CALL(stack, pushView(serio::qt::tvShowView));
    viewModel.openTvShowView(QVariantList({QString::fromStdString(tvShowName)}));
}

TEST_F(AllTvShowsViewModelTest, shouldLoadFirstPageOfWatchShowsAndAllShows) {
    QSignalSpy allTvShowsSpy(viewModel.getAllShows(), &serio::qt::TvShowListModel::requestPageLoad);
    QSignalSpy watchedTvShowsSpy(viewModel.getWatchedShows(), &serio::qt::TvShowListModel::requestPageLoad);
    viewModel.loadFirstPage();
    for (const auto& args: {allTvShowsSpy.takeFirst(), watchedTvShowsSpy.takeFirst()}) {
        EXPECT_EQ(0, args[0].toUInt());
        EXPECT_EQ(pageSize, args[1].toUInt());
    }
}
