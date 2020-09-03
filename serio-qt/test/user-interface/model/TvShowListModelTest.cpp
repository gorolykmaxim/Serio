#include <gtest/gtest.h>
#include <QSignalSpy>
#include "user-interface/model/TvShowListModel.h"

class TvShowListModelTest : public ::testing::Test {
protected:
    const QModelIndex index;
    const unsigned int pageSize = 100;
    serio::qt::TvShowListModel model;
    std::vector<serio::core::TvShow> tvShows;
public:
    TvShowListModelTest() : model(pageSize, 2) {
        tvShows.reserve(pageSize);
        for (int i = 0; i < pageSize; i++) {
            tvShows.emplace_back("");
        }
    }
};

TEST_F(TvShowListModelTest, shouldReturnHashOfExistingRoles) {
    const QHash<int, QByteArray> roleNames = model.roleNames();
    EXPECT_EQ("name", roleNames[serio::qt::TvShowListModel::Role::NAME]);
    EXPECT_EQ("thumbnailUrl", roleNames[serio::qt::TvShowListModel::Role::THUMBNAIL_URL]);
    EXPECT_EQ("lastWatchDate", roleNames[serio::qt::TvShowListModel::Role::LAST_WATCH_DATE]);
}

TEST_F(TvShowListModelTest, shouldReturnSameCountOfColumnsAsRoleNames) {
    EXPECT_EQ(model.roleNames().count(), model.columnCount(index));
}

TEST_F(TvShowListModelTest, shouldHaveNoRowsByDefault) {
    EXPECT_EQ(0, model.rowCount(index));
}

TEST_F(TvShowListModelTest, shouldUpdateRowCountAccordingToLatestAddedPage) {
    unsigned int totalSize = 3;
    serio::core::ListPage<serio::core::TvShow> page(1, totalSize, {serio::core::TvShow("Clinic")});
    model.loadPage(page);
    EXPECT_EQ(totalSize, model.rowCount(index));
}

TEST_F(TvShowListModelTest, shouldInsertNewRowsOnLoadingNewPage) {
    QSignalSpy spy(&model, &serio::qt::TvShowListModel::rowsInserted);
    serio::core::ListPage<serio::core::TvShow> page(1, 5, {serio::core::TvShow("Clinic")});
    model.loadPage(page);
    ASSERT_EQ(1, spy.count());
    QVariantList args = spy.takeFirst();
    EXPECT_EQ(0, args[1].toUInt());
    EXPECT_EQ(page.getLastItemIndex(), args[2].toUInt());
}

TEST_F(TvShowListModelTest, shouldRemoveRowsOnLoadingNewPage) {
    QSignalSpy spy(&model, &serio::qt::TvShowListModel::rowsRemoved);
    model.loadPage(serio::core::ListPage<serio::core::TvShow>(1, 5, {}));
    model.loadPage(serio::core::ListPage<serio::core::TvShow>(1, 3, {}));
    ASSERT_EQ(1, spy.count());
    QVariantList args = spy.takeFirst();
    EXPECT_EQ(3, args[1].toUInt());
    EXPECT_EQ(4, args[2].toUInt());
}

TEST_F(TvShowListModelTest, shouldNeitherRemoveNorInsertRowsOnLoadingNewPageWithSameTotalSize) {
    model.loadPage(serio::core::ListPage<serio::core::TvShow>(1, 5, {}));
    QSignalSpy insertSpy(&model, &serio::qt::TvShowListModel::rowsInserted);
    QSignalSpy removeSpy(&model, &serio::qt::TvShowListModel::rowsRemoved);
    model.loadPage(serio::core::ListPage<serio::core::TvShow>(1, 5, {}));
    EXPECT_EQ(0, insertSpy.count());
    EXPECT_EQ(0, removeSpy.count());
}


TEST_F(TvShowListModelTest, shouldChangeDataOnLoadingNewPage) {
    QSignalSpy spy(&model, &serio::qt::TvShowListModel::dataChanged);
    model.loadPage(serio::core::ListPage<serio::core::TvShow>(300, 1000, tvShows));
    ASSERT_EQ(1, spy.count());
    QVariantList args = spy.takeFirst();
    EXPECT_EQ(300, args[0].toModelIndex().row());
    EXPECT_EQ(399, args[1].toModelIndex().row());
}

TEST_F(TvShowListModelTest, shouldReturnNullOnInvalidIndex) {
    EXPECT_TRUE(model.data(index, serio::qt::TvShowListModel::Role::NAME).isNull());
}

TEST_F(TvShowListModelTest, shouldReturnNameOfTvShowWithSpecifiedIndex) {
    std::string mandalorian = "Mandalorian";
    std::string friends = "Friends";
    model.loadPage(serio::core::ListPage<serio::core::TvShow>(0, 10, {serio::core::TvShow(mandalorian)}));
    model.loadPage(serio::core::ListPage<serio::core::TvShow>(1, 10, {serio::core::TvShow(friends)}));
    EXPECT_EQ(mandalorian, model.data(model.index(0), serio::qt::TvShowListModel::NAME).toString().toStdString());
    EXPECT_EQ(friends, model.data(model.index(1), serio::qt::TvShowListModel::NAME).toString().toStdString());
}

TEST_F(TvShowListModelTest, shouldReturnThumbnailOfTvShowWithSpecifiedIndex) {
    std::string mandalorian = "https://p8b5s7z5.rocketcdn.me/wp-content/uploads/The-Mandalorian-Poster.jpg";
    std::string friends = "https://m.media-amazon.com/images/M/MV5BNDVkYjU0MzctMWRmZi00NTkxLTgwZWEtOWVhYjZlYjllYmU4XkEyXkFqcGdeQXVyNTA4NzY1MzY@._V1_UY1200_CR93,0,630,1200_AL_.jpg";
    model.loadPage(serio::core::ListPage<serio::core::TvShow>(0, 10, {serio::core::TvShow("", mandalorian)}));
    model.loadPage(serio::core::ListPage<serio::core::TvShow>(1, 10, {serio::core::TvShow("", friends)}));
    EXPECT_EQ(mandalorian, model.data(model.index(0), serio::qt::TvShowListModel::THUMBNAIL_URL).toString().toStdString());
    EXPECT_EQ(friends, model.data(model.index(1), serio::qt::TvShowListModel::THUMBNAIL_URL).toString().toStdString());
}

TEST_F(TvShowListModelTest, shouldReturnLastWatchDateOfTvShowWithSpecifiedIndex) {
    serio::core::TvShow today("", "", serio::core::LastWatchDate(std::chrono::system_clock::now()));
    serio::core::TvShow yesterday("", "", serio::core::LastWatchDate(std::chrono::system_clock::now() - std::chrono::hours(25)));
    serio::core::TvShow notWatched("");
    model.loadPage(serio::core::ListPage<serio::core::TvShow>(0, 10, {today, yesterday, notWatched}));
    EXPECT_EQ(today.getLastWatchDate()->toString(), model.data(model.index(0), serio::qt::TvShowListModel::Role::LAST_WATCH_DATE).toString().toStdString());
    EXPECT_EQ(yesterday.getLastWatchDate()->toString(), model.data(model.index(1), serio::qt::TvShowListModel::Role::LAST_WATCH_DATE).toString().toStdString());
    EXPECT_TRUE(model.data(model.index(2), serio::qt::TvShowListModel::Role::LAST_WATCH_DATE).isNull());
}

TEST_F(TvShowListModelTest, shouldFailToGetDataWithUnknownRole) {
    model.loadPage(serio::core::ListPage<serio::core::TvShow>(0, 10, {serio::core::TvShow("")}));
    EXPECT_THROW(model.data(model.index(0), -1), std::invalid_argument);
}

TEST_F(TvShowListModelTest, shouldReturnNullTvShowIfSpecifiedIndexIsNotLoaded) {
    model.loadPage(serio::core::ListPage<serio::core::TvShow>(0, 10, {serio::core::TvShow("")}));
    EXPECT_EQ(QString(), model.data(model.index(1), serio::qt::TvShowListModel::Role::NAME).toString());
}

TEST_F(TvShowListModelTest, shouldRequestLoadOfPageContainingSpecifiedItem) {
    model.loadPage(serio::core::ListPage<serio::core::TvShow>(0, 1000, tvShows));
    QSignalSpy spy(&model, &serio::qt::TvShowListModel::requestPageLoad);
    model.data(model.index(505), serio::qt::TvShowListModel::Role::NAME);
    model.data(model.index(649), serio::qt::TvShowListModel::Role::NAME);
    ASSERT_EQ(2, spy.count());
    QVariantList args = spy.takeFirst();
    EXPECT_EQ(500, args[0].toUInt());
    EXPECT_EQ(pageSize, args[1].toUInt());
    args = spy.takeLast();
    EXPECT_EQ(600, args[0].toUInt());
}

TEST_F(TvShowListModelTest, shouldNotRequestLoadOfPageOnInvalidIndex) {
    QSignalSpy spy(&model, &serio::qt::TvShowListModel::requestPageLoad);
    model.data(index, serio::qt::TvShowListModel::Role::NAME);
    ASSERT_EQ(0, spy.count());
}

TEST_F(TvShowListModelTest, shouldNotRequestTheSamePageTwice) {
    model.loadPage(serio::core::ListPage<serio::core::TvShow>(100, 1000, tvShows));
    QSignalSpy spy(&model, &serio::qt::TvShowListModel::requestPageLoad);
    model.data(model.index(200), serio::qt::TvShowListModel::Role::NAME);
    model.data(model.index(200), serio::qt::TvShowListModel::Role::THUMBNAIL_URL);
    ASSERT_EQ(1, spy.count());
}
