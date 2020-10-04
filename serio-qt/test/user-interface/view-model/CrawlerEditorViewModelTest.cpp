#include <gtest/gtest.h>
#include <TvShowCrawlerEditorMock.h>
#include <user-interface/StackOfViews.h>
#include <user-interface/view-model/CrawlerEditorViewModel.h>
#include <QSignalSpy>
#include <StackOfViewsMock.h>
#include <user-interface/ViewNames.h>
#include <user-interface/model/ButtonModel.h>

class CrawlerEditorViewModelTest : public ::testing::Test {
protected:
    const std::vector<serio::core::CrawlerStep> steps = {
            serio::core::CrawlerStep("value", {{"value", "a"}}),
            serio::core::CrawlerStep("fetch"),
            serio::core::CrawlerStep("some other", {{"a", "1"}, {"b", "false"}, {"c", "text"}})
    };
    const std::vector<std::string> previewResults = {"result 1", "result 2"};
    ::testing::NiceMock<TvShowCrawlerEditorMock> editor;
    ::testing::NiceMock<StackOfViewsMock> stack;
    serio::qt::CrawlerEditorViewModel viewModel = serio::qt::CrawlerEditorViewModel(editor, stack);
    QSignalSpy crawlerTypeSpy = QSignalSpy(&viewModel, &serio::qt::CrawlerEditorViewModel::crawlerTypeChanged);
    virtual void SetUp() {
        ON_CALL(editor, getCrawlerSteps()).WillByDefault(::testing::Return(steps));
        ON_CALL(editor, previewCrawler()).WillByDefault(::testing::Return(serio::core::CrawlResult{{}, previewResults}));
    }
    void expectCrawlerEditorViewToOpen(serio::core::CrawlerType type, int typeNumber, const QString& crawlerType) {
        EXPECT_CALL(editor, editCrawler(type));
        EXPECT_CALL(stack, pushView(serio::qt::crawlerEditorView));
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
    viewModel.loadCrawlerSteps();
    QList<serio::qt::TileModel*> tiles = viewModel.getCrawlerSteps();
    EXPECT_EQ(steps.size(), tiles.size());
    EXPECT_EQ("Value step", tiles[0]->getTitle());
    EXPECT_EQ("value: a", tiles[0]->getSubtitle());
    EXPECT_TRUE(tiles[0]->getIcon().isEmpty());
    EXPECT_EQ("Fetch step", tiles[1]->getTitle());
    EXPECT_TRUE(tiles[1]->getSubtitle().isEmpty());
    EXPECT_TRUE(tiles[1]->getIcon().isEmpty());
    EXPECT_EQ("Some other step", tiles[2]->getTitle());
    EXPECT_EQ("a: 1, b: false, c: text", tiles[2]->getSubtitle());
    EXPECT_TRUE(tiles[2]->getIcon().isEmpty());
}

TEST_F(CrawlerEditorViewModelTest, shouldReloadCrawlerSteps) {
    viewModel.loadCrawlerSteps();
    viewModel.loadCrawlerSteps();
    EXPECT_EQ(3, viewModel.getCrawlerSteps().size());
}

TEST_F(CrawlerEditorViewModelTest, shouldNotifyWatchersAboutCrawlerStepsListChange) {
    QSignalSpy crawlerStepsSpy(&viewModel, &serio::qt::CrawlerEditorViewModel::crawlerStepsChanged);
    viewModel.loadCrawlerSteps();
    EXPECT_EQ(1, crawlerStepsSpy.count());
}

TEST_F(CrawlerEditorViewModelTest, shouldSaveEditedCrawlerAndPopCurrentViewFromStack) {
    EXPECT_CALL(editor, saveCrawler());
    EXPECT_CALL(stack, popCurrentView());
    viewModel.save();
}

TEST_F(CrawlerEditorViewModelTest, shouldPushCrawlerEditorHelpViewToStack) {
    EXPECT_CALL(stack, pushView(serio::qt::crawlerEditorHelpView));
    viewModel.openHelp();
}

TEST_F(CrawlerEditorViewModelTest, shouldPreviewEditedCrawlerAndOpenCrawlerPreviewViewWithTheResults) {
    EXPECT_CALL(editor, previewCrawler());
    viewModel.openCrawlerEditor(serio::core::CrawlerType::episodeVideoCrawler);
    viewModel.openCrawlerPreview();
    QList<serio::qt::TileModel*> tiles = viewModel.getPreviewResults();
    for (int i = 0; i < previewResults.size(); i++) {
        EXPECT_EQ(previewResults[i], tiles[i]->getTitle().toStdString());
        EXPECT_TRUE(tiles[i]->getSubtitle().isEmpty());
        EXPECT_TRUE(tiles[i]->getIcon().isEmpty());
    }
}

TEST_F(CrawlerEditorViewModelTest, shouldOpenCrawlerPreviewAfterDisplayingCrawlingInProgress) {
    viewModel.openCrawlerEditor(serio::core::CrawlerType::episodeVideoCrawler);
    ::testing::InSequence s;
    EXPECT_CALL(stack, pushView(serio::qt::crawlingInProgressView));
    EXPECT_CALL(stack, replaceCurrentViewWith(serio::qt::crawlerPreviewView));
    viewModel.openCrawlerPreview();
}

TEST_F(CrawlerEditorViewModelTest, shouldNotifyWatchersAboutChangedPreviewResults) {
    QSignalSpy previewResultsSpy(&viewModel, &serio::qt::CrawlerEditorViewModel::previewResultsChanged);
    viewModel.openCrawlerEditor(serio::core::CrawlerType::episodeVideoCrawler);
    viewModel.openCrawlerPreview();
    EXPECT_EQ(1, previewResultsSpy.count());
}

TEST_F(CrawlerEditorViewModelTest, shouldOverridePreviousCrawlerPreviewResultsWhenPreviewCrawlerSecondTime) {
    viewModel.openCrawlerEditor(serio::core::CrawlerType::thumbnailCrawler);
    viewModel.openCrawlerPreview();
    viewModel.openCrawlerPreview();
    QList<serio::qt::TileModel*> tiles = viewModel.getPreviewResults();
    EXPECT_EQ(previewResults.size(), tiles.size());
}

TEST_F(CrawlerEditorViewModelTest, shouldCloseCrawingInProgressViewIfCrawlerPreviewHasFailed) {
    ON_CALL(editor, previewCrawler()).WillByDefault(::testing::Throw(std::runtime_error("expected")));
    ::testing::InSequence s;
    EXPECT_CALL(stack, pushView(serio::qt::crawlingInProgressView));
    EXPECT_CALL(stack, popCurrentView());
    EXPECT_THROW(viewModel.openCrawlerPreview(), std::runtime_error);
}

TEST_F(CrawlerEditorViewModelTest, shouldReturnListOfCrawlerEditorActions) {
    QList<serio::qt::ButtonModel*> actions = viewModel.getCrawlerEditorActions();
    EXPECT_EQ(serio::qt::ButtonModel("cancel", serio::qt::ActionType::BACK, {}, false), *actions[0]);
    EXPECT_EQ(serio::qt::ButtonModel("add step", serio::qt::ActionType::OPEN_NEW_CRAWLER_STEP_EDITOR), *actions[1]);
    EXPECT_EQ(serio::qt::ButtonModel("save", serio::qt::ActionType::SAVE_CRAWLER), *actions[2]);
    EXPECT_EQ(serio::qt::ButtonModel("preview", serio::qt::ActionType::PREVIEW_CRAWLER), *actions[3]);
    EXPECT_EQ(serio::qt::ButtonModel("help", serio::qt::ActionType::OPEN_CRAWLER_EDITOR_HELP), *actions[4]);
}

TEST_F(CrawlerEditorViewModelTest, shouldReturnListOfCrawlerPreviewActions) {
    for (auto type: {serio::core::CrawlerType::episodeVideoCrawler, serio::core::CrawlerType::thumbnailCrawler}) {
        viewModel.openCrawlerEditor(type);
        viewModel.openCrawlerPreview();
        QList<serio::qt::ButtonModel*> actions = viewModel.getCrawlerPreviewActions();
        EXPECT_EQ(serio::qt::ButtonModel("back", serio::qt::ActionType::BACK, {}, false), *actions[0]);
        EXPECT_EQ(serio::qt::ButtonModel("view log", serio::qt::ActionType::OPEN_PREVIEWED_CRAWLER_LOG, QVariantList({viewModel.getCrawlerType()})), *actions[1]);
    }
}

TEST_F(CrawlerEditorViewModelTest, shouldNotifyWatchersAboutChangedCrawlerPreviewActionsList) {
    viewModel.openCrawlerEditor(serio::core::CrawlerType::thumbnailCrawler);
    QSignalSpy spy(&viewModel, &serio::qt::CrawlerEditorViewModel::crawlerPreviewActionsChanged);
    viewModel.openCrawlerPreview();
    EXPECT_EQ(2, spy.count());
}