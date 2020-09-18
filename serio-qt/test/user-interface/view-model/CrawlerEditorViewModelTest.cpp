#include <gtest/gtest.h>
#include <TvShowCrawlerEditorMock.h>
#include <user-interface/StackOfViews.h>
#include <user-interface/view-model/CrawlerEditorViewModel.h>
#include <QSignalSpy>

class CrawlerEditorViewModelTest : public ::testing::Test {
protected:
    TvShowCrawlerEditorMock editor = TvShowCrawlerEditorMock::create();
    serio::qt::StackOfViews stack;
    serio::qt::CrawlerEditorViewModel viewModel = serio::qt::CrawlerEditorViewModel(editor, stack);
    QSignalSpy stackPushSpy = QSignalSpy(&stack, &serio::qt::StackOfViews::push);
    QSignalSpy crawlerTypeSpy = QSignalSpy(&viewModel, &serio::qt::CrawlerEditorViewModel::crawlerTypeChanged);
    void expectCrawlerEditorViewToOpen(serio::core::CrawlerType type, int typeNumber, const QString& crawlerType) {
        EXPECT_CALL(editor, editCrawler(type));
        viewModel.openCrawlerEditor(static_cast<serio::core::CrawlerType>(typeNumber));
        EXPECT_EQ(crawlerType, viewModel.getCrawlerType());
        ASSERT_EQ(1, stackPushSpy.count());
        QVariantList args = stackPushSpy.takeFirst();
        EXPECT_EQ(QStringList("views/CrawlerEditorView.qml"), args[0].toStringList());
        EXPECT_EQ(1, crawlerTypeSpy.count());
    }
};

TEST_F(CrawlerEditorViewModelTest, shouldFailToGetCrawlerTypeSinceTheViewWasNotProperlyOpened) {
    EXPECT_THROW((void)viewModel.getCrawlerType(), std::logic_error);
}

TEST_F(CrawlerEditorViewModelTest, shouldOpenEpisodeVideoCrawlerEditorView) {
    expectCrawlerEditorViewToOpen(serio::core::CrawlerType::episodeVideoCrawler, viewModel.getEpisodeVideoCrawlerType(), "Episode video");
}

TEST_F(CrawlerEditorViewModelTest, shouldOpenThumbnailCrawlerEditorView) {
    expectCrawlerEditorViewToOpen(serio::core::CrawlerType::thumbnailCrawler, viewModel.getThumbnailCrawlerType(), "Thumbnail");
}

TEST_F(CrawlerEditorViewModelTest, shouldOpenEpisodeNameCrawlerEditorView) {
    expectCrawlerEditorViewToOpen(serio::core::CrawlerType::episodeNameCrawler, viewModel.getEpisodeNameCrawlerType(), "Episode name");
}

TEST_F(CrawlerEditorViewModelTest, shouldDisplayNoCrawlerStepsByDefault) {
    EXPECT_TRUE(viewModel.getCrawlerSteps().isEmpty());
}

TEST_F(CrawlerEditorViewModelTest, shouldLoadCrawlerStepsAndDisplayThem) {
    std::vector<serio::core::CrawlerStep> steps = {
            serio::core::CrawlerStep("value", {{"value", "a"}}),
            serio::core::CrawlerStep("fetch"),
            serio::core::CrawlerStep("some other", {{"a", "1"}, {"b", "false"}, {"c", "text"}})
    };
    EXPECT_CALL(editor, getCrawlerSteps()).WillRepeatedly(::testing::Return(steps));
    QSignalSpy crawlerStepsSpy(&viewModel, &serio::qt::CrawlerEditorViewModel::crawlerStepsChanged);
    viewModel.loadCrawlerSteps();
    viewModel.loadCrawlerSteps();
    QList<serio::qt::TileModel*> tiles = viewModel.getCrawlerSteps();
    EXPECT_EQ(steps.size(), tiles.size());
    EXPECT_EQ("Value step", tiles[0]->getTitle());
    EXPECT_EQ("value: a", tiles[0]->getSubtitle());
    EXPECT_EQ("Fetch step", tiles[1]->getTitle());
    EXPECT_TRUE(tiles[1]->getSubtitle().isEmpty());
    EXPECT_EQ("Some other step", tiles[2]->getTitle());
    EXPECT_EQ("a: 1, b: false, c: text", tiles[2]->getSubtitle());
    EXPECT_EQ(2, crawlerStepsSpy.count());
    qDeleteAll(tiles);
}

TEST_F(CrawlerEditorViewModelTest, shouldSaveEditedCrawlerAndPopCurrentViewFromStack) {
    EXPECT_CALL(editor, saveCrawler());
    QSignalSpy stackPopSpy(&stack, &serio::qt::StackOfViews::pop);
    viewModel.save();
    ASSERT_EQ(1, stackPopSpy.count());
    QVariantList args = stackPopSpy.takeFirst();
    EXPECT_TRUE(args[0].toBool());
}

TEST_F(CrawlerEditorViewModelTest, shouldPushCrawlerEditorHelpViewToStack) {
    viewModel.openHelp();
    ASSERT_EQ(1, stackPushSpy.count());
    QVariantList args = stackPushSpy.takeFirst();
    EXPECT_EQ(QStringList("views/CrawlerEditorHelpView.qml"), args[0].toStringList());
}
