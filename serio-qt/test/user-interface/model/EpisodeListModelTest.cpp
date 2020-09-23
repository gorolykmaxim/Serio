#include <gtest/gtest.h>
#include <user-interface/model/EpisodeListModel.h>
#include <QByteArray>
#include <tv-show-storage/Episode.h>
#include <tv-show-storage/ListPage.h>
#include <QSignalSpy>

class EpisodeListModelTest : public ::testing::Test {
protected:
    const QModelIndex index;
    const unsigned int pageSize = 100;
    std::vector<serio::core::Episode> episodes;
    const serio::core::Episode episode = serio::core::Episode(
            1,
            "https://video.com/episode-1.mp4",
            "Episode 1",
            serio::core::LastWatchDate(std::chrono::system_clock::now()));
    serio::qt::EpisodeListModel model = serio::qt::EpisodeListModel(pageSize, 2);
    virtual void SetUp() {
        episodes.reserve(pageSize);
        for (int i = 0; i < pageSize; i++) {
            episodes.emplace_back(i + 1, "video-url");
        }
    }
};

TEST_F(EpisodeListModelTest, shouldReturnHashOfExistingRoles) {
    const QHash<int, QByteArray> rolesNames = model.roleNames();
    EXPECT_EQ("name", rolesNames[serio::qt::EpisodeListModel::Role::NAME]);
    EXPECT_EQ("videoUrl", rolesNames[serio::qt::EpisodeListModel::Role::VIDEO_URL]);
    EXPECT_EQ("lastWatchDate", rolesNames[serio::qt::EpisodeListModel::Role::LAST_WATCH_DATE]);
}

TEST_F(EpisodeListModelTest, shouldReturnSameCountOfColumnsAsRoleNames) {
    EXPECT_EQ(model.roleNames().count(), model.columnCount(index));
}

TEST_F(EpisodeListModelTest, shouldHaveNoRowsByDefault) {
    EXPECT_EQ(0, model.rowCount(index));
}

TEST_F(EpisodeListModelTest, shouldUpdateRowCountAccordingToLatestAddedPage) {
    unsigned int totalSize = 3;
    serio::core::ListPage<serio::core::Episode> page(1, totalSize, {episode});
    model.loadPage(page);
    EXPECT_EQ(totalSize, model.rowCount(index));
}

TEST_F(EpisodeListModelTest, shouldInsertNewRowsOnLoadingNewPage) {
    QSignalSpy spy(&model, &serio::qt::EpisodeListModel::rowsInserted);
    serio::core::ListPage<serio::core::Episode> page(1, 5, {episode});
    model.loadPage(page);
    ASSERT_EQ(1, spy.count());
    QVariantList args = spy.takeFirst();
    EXPECT_EQ(0, args[1].toUInt());
    EXPECT_EQ(page.getLastItemIndex(), args[2].toUInt());
}

TEST_F(EpisodeListModelTest, shouldRemoveRowsOnLoadingNewPage) {
    QSignalSpy spy(&model, &serio::qt::EpisodeListModel::rowsRemoved);
    model.loadPage(serio::core::ListPage<serio::core::Episode>(1, 5, {}));
    model.loadPage(serio::core::ListPage<serio::core::Episode>(1, 3, {}));
    ASSERT_EQ(1, spy.count());
    QVariantList args = spy.takeFirst();
    EXPECT_EQ(3, args[1].toUInt());
    EXPECT_EQ(4, args[2].toUInt());
}

TEST_F(EpisodeListModelTest, shouldNeitherRemoveNorInsertRowsOnLoadingNewPageWithSameTotalSize) {
    model.loadPage(serio::core::ListPage<serio::core::Episode>(1, 5, {}));
    QSignalSpy insertSpy(&model, &serio::qt::EpisodeListModel::rowsInserted);
    QSignalSpy removeSpy(&model, &serio::qt::EpisodeListModel::rowsRemoved);
    model.loadPage(serio::core::ListPage<serio::core::Episode>(1, 5, {}));
    EXPECT_EQ(0, insertSpy.count());
    EXPECT_EQ(0, removeSpy.count());
}

TEST_F(EpisodeListModelTest, shouldChangeDataOnLoadingNewPage) {
    QSignalSpy spy(&model, &serio::qt::EpisodeListModel::dataChanged);
    model.loadPage(serio::core::ListPage<serio::core::Episode>(300, 1000, episodes));
    ASSERT_EQ(1, spy.count());
    QVariantList args = spy.takeFirst();
    EXPECT_EQ(300, args[0].toModelIndex().row());
    EXPECT_EQ(399, args[1].toModelIndex().row());
}

TEST_F(EpisodeListModelTest, shouldReturnNullOnInvalidIndex) {
    EXPECT_TRUE(model.data(index, serio::qt::EpisodeListModel::Role::NAME).isNull());
}

TEST_F(EpisodeListModelTest, shouldReturnNameOfEpisodeWithSpecifiedIndex) {
    model.loadPage(serio::core::ListPage<serio::core::Episode>(0, 10, {episodes[0]}));
    model.loadPage(serio::core::ListPage<serio::core::Episode>(1, 10, {episodes[1]}));
    EXPECT_EQ(episodes[0].getName(), model.data(model.index(0), serio::qt::EpisodeListModel::NAME).toString().toStdString());
    EXPECT_EQ(episodes[1].getName(), model.data(model.index(1), serio::qt::EpisodeListModel::NAME).toString().toStdString());
}

TEST_F(EpisodeListModelTest, shouldReturnVideoUrlOfEpisodeWithSpecifiedIndex) {
    model.loadPage(serio::core::ListPage<serio::core::Episode>(0, 10, {episode}));
    model.loadPage(serio::core::ListPage<serio::core::Episode>(1, 10, {episodes[0]}));
    EXPECT_EQ(episode.getVideoUrl(), model.data(model.index(0), serio::qt::EpisodeListModel::Role::VIDEO_URL).toString().toStdString());
    EXPECT_EQ(episodes[0].getVideoUrl(), model.data(model.index(1), serio::qt::EpisodeListModel::Role::VIDEO_URL).toString().toStdString());
}

TEST_F(EpisodeListModelTest, shouldReturnLastWatchDateOfEpisodeWithSpecifiedIndex) {
    model.loadPage(serio::core::ListPage<serio::core::Episode>(0, 10, {episode}));
    model.loadPage(serio::core::ListPage<serio::core::Episode>(1, 10, {episodes[0]}));
    EXPECT_EQ(episode.getLastWatchDate()->toString(), model.data(model.index(0), serio::qt::EpisodeListModel::Role::LAST_WATCH_DATE).toString().toStdString());
    EXPECT_TRUE(model.data(model.index(1), serio::qt::EpisodeListModel::Role::LAST_WATCH_DATE).isNull());
}

TEST_F(EpisodeListModelTest, shouldFailToGetDataWithUnknownRole) {
    model.loadPage(serio::core::ListPage<serio::core::Episode>(0, 10, {episode}));
    EXPECT_THROW((void)model.data(model.index(0), -1), std::invalid_argument);
}

TEST_F(EpisodeListModelTest, shouldReturnNullTvShowIfSpecifiedIndexIsNotLoaded) {
    model.loadPage(serio::core::ListPage<serio::core::Episode>(0, 10, {episode}));
    EXPECT_EQ(QString(), model.data(model.index(1), serio::qt::EpisodeListModel::Role::NAME).toString());
}

TEST_F(EpisodeListModelTest, shouldRequestLoadOfPageContainingSpecifiedItem) {
    model.loadPage(serio::core::ListPage<serio::core::Episode>(0, 1000, episodes));
    QSignalSpy spy(&model, &serio::qt::EpisodeListModel::requestPageLoad);
    (void)model.data(model.index(505), serio::qt::EpisodeListModel::Role::NAME);
    (void)model.data(model.index(649), serio::qt::EpisodeListModel::Role::NAME);
    ASSERT_EQ(2, spy.count());
    QVariantList args = spy.takeFirst();
    EXPECT_EQ(500, args[0].toUInt());
    EXPECT_EQ(pageSize, args[1].toUInt());
    args = spy.takeLast();
    EXPECT_EQ(600, args[0].toUInt());
}

TEST_F(EpisodeListModelTest, shouldNotRequestLoadOfPageOnInvalidIndex) {
    QSignalSpy spy(&model, &serio::qt::EpisodeListModel::requestPageLoad);
    (void)model.data(index, serio::qt::EpisodeListModel::Role::NAME);
    ASSERT_EQ(0, spy.count());
}

TEST_F(EpisodeListModelTest, shouldNotRequestTheSamePageTwice) {
    model.loadPage(serio::core::ListPage<serio::core::Episode>(100, 1000, episodes));
    QSignalSpy spy(&model, &serio::qt::EpisodeListModel::requestPageLoad);
    (void)model.data(model.index(200), serio::qt::EpisodeListModel::Role::NAME);
    (void)model.data(model.index(200), serio::qt::EpisodeListModel::Role::VIDEO_URL);
    ASSERT_EQ(1, spy.count());
}
