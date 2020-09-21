#include <gtest/gtest.h>
#include <TvShowCrawlerEditorMock.h>
#include <user-interface/StackOfViews.h>
#include <user-interface/view-model/CrawlerEditorViewModel.h>
#include <QSignalSpy>
#include <StackOfViewsMock.h>

class CrawlerEditorViewModelTest : public ::testing::Test {
protected:
    const std::vector<std::string> previewResults = {"result 1", "result 2"};
    TvShowCrawlerEditorMock editor = TvShowCrawlerEditorMock::create();
    ::testing::NiceMock<StackOfViewsMock> stack;
    serio::qt::CrawlerEditorViewModel viewModel = serio::qt::CrawlerEditorViewModel(editor, stack);
    QSignalSpy crawlerTypeSpy = QSignalSpy(&viewModel, &serio::qt::CrawlerEditorViewModel::crawlerTypeChanged);
    void expectCrawlerEditorViewToOpen(serio::core::CrawlerType type, int typeNumber, const QString& crawlerType) {
        EXPECT_CALL(editor, editCrawler(type));
        EXPECT_CALL(stack, pushView(QString("CrawlerEditorView.qml")));
        viewModel.openCrawlerEditor(static_cast<serio::core::CrawlerType>(typeNumber));
        EXPECT_EQ(crawlerType, viewModel.getCrawlerType());
        EXPECT_EQ(1, crawlerTypeSpy.count());
    }
};

TEST_F(CrawlerEditorViewModelTest, shouldFailToGetCrawlerTypeSinceTheViewWasNotProperlyOpened) {
    EXPECT_THROW((void)viewModel.getCrawlerType(), std::logic_error);
}

TEST_F(CrawlerEditorViewModelTest, shouldOpenEpisodeVideoCrawlerEditorView) {
    expectCrawlerEditorViewToOpen(serio::core::CrawlerType::episodeVideoCrawler, viewModel.getEpisodeVideoCrawlerType(), "Episode Video");
}

TEST_F(CrawlerEditorViewModelTest, shouldOpenThumbnailCrawlerEditorView) {
    expectCrawlerEditorViewToOpen(serio::core::CrawlerType::thumbnailCrawler, viewModel.getThumbnailCrawlerType(), "Thumbnail");
}

TEST_F(CrawlerEditorViewModelTest, shouldOpenEpisodeNameCrawlerEditorView) {
    expectCrawlerEditorViewToOpen(serio::core::CrawlerType::episodeNameCrawler, viewModel.getEpisodeNameCrawlerType(), "Episode Name");
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
}

TEST_F(CrawlerEditorViewModelTest, shouldSaveEditedCrawlerAndPopCurrentViewFromStack) {
    EXPECT_CALL(editor, saveCrawler());
    EXPECT_CALL(stack, popCurrentView());
    viewModel.save();
}

TEST_F(CrawlerEditorViewModelTest, shouldPushCrawlerEditorHelpViewToStack) {
    EXPECT_CALL(stack, pushView(QString("CrawlerEditorHelpView.qml")));
    viewModel.openHelp();
}

TEST_F(CrawlerEditorViewModelTest, shouldPreviewEditedCrawlerAndOpenCrawlerPreviewViewWithTheResults) {
    EXPECT_CALL(editor, previewCrawler()).WillOnce(::testing::Return(previewResults));
    ::testing::InSequence s;
    EXPECT_CALL(stack, pushView(QString("CrawlingInProgressView.qml")));
    EXPECT_CALL(stack, replaceCurrentViewWith(QString("CrawlerPreviewView.qml")));
    QSignalSpy previewResultsSpy(&viewModel, &serio::qt::CrawlerEditorViewModel::previewResultsChanged);
    viewModel.openCrawlerPreview();
    QList<serio::qt::TileModel*> tiles = viewModel.getPreviewResults();
    for (int i = 0; i < previewResults.size(); i++) {
        EXPECT_EQ(previewResults[i], tiles[i]->getTitle().toStdString());
        EXPECT_TRUE(tiles[i]->getSubtitle().isEmpty());
    }
    EXPECT_EQ(1, previewResultsSpy.count());
}

TEST_F(CrawlerEditorViewModelTest, shouldOverridePreviousCrawlerPreviewResultsWhenPreviewCrawlerSecondTime) {
    EXPECT_CALL(editor, previewCrawler()).WillRepeatedly(::testing::Return(previewResults));
    viewModel.openCrawlerPreview();
    viewModel.openCrawlerPreview();
    QList<serio::qt::TileModel*> tiles = viewModel.getPreviewResults();
    EXPECT_EQ(previewResults.size(), tiles.size());
}

TEST_F(CrawlerEditorViewModelTest, shouldCloseCrawingInProgressViewIfCrawlerPreviewHasFailed) {
    EXPECT_CALL(editor, previewCrawler()).WillOnce(::testing::Throw(std::runtime_error("expected")));
    ::testing::InSequence s;
    EXPECT_CALL(stack, pushView(QString("CrawlingInProgressView.qml")));
    EXPECT_CALL(stack, popCurrentView());
    EXPECT_THROW(viewModel.openCrawlerPreview(), std::runtime_error);
}
