#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <TvShowCrawlerRuntimeMock.h>
#include <tv-show-crawler-editor/TvShowCrawlerEditor.h>

class TvShowCrawlerEditorTest : public ::testing::Test {
protected:
    const std::string mandalorian = "Mandalorian";
    const std::string friends = "Friends";
    serio::core::CrawlerStep valueStep = serio::core::CrawlerStep("value", {{"value", "42"}});
    serio::core::CrawlerStep fetchStep = serio::core::CrawlerStep("fetch");
    TvShowCrawlerRuntimeMock runtime = TvShowCrawlerRuntimeMock::create();
    serio::core::TvShowCrawlerEditor editor = serio::core::TvShowCrawlerEditor(runtime);
    serio::core::TvShowCrawler emptyCrawler = serio::core::TvShowCrawler(friends, serio::core::Crawler());
};

TEST_F(TvShowCrawlerEditorTest, shouldReturnAllCrawlerTypesDefinedInRuntime) {
    std::vector<serio::core::CrawlerStepType> types = {
            serio::core::CrawlerStepType("first type"),
            serio::core::CrawlerStepType("second type", {"a", "b"})
    };
    EXPECT_CALL(runtime, getCrawlerStepTypes()).WillOnce(::testing::Return(types));
    EXPECT_EQ(types, editor.getCrawlerStepTypes());
}

TEST_F(TvShowCrawlerEditorTest, shouldNotAllowRunningCrawlerWithoutCreatingItFirst) {
    EXPECT_CALL(runtime, crawlTvShowAndSaveCrawler(::testing::_)).Times(0);
    EXPECT_THROW(editor.saveAndRunCrawler(), std::logic_error);
}

TEST_F(TvShowCrawlerEditorTest, shouldNotAllowRunningCrawlerWithoutTvShowNameSpecified) {
    EXPECT_CALL(runtime, crawlTvShowAndSaveCrawler(::testing::_)).Times(0);
    editor.createTvShowCrawler();
    EXPECT_THROW(editor.saveAndRunCrawler(), std::logic_error);
}

TEST_F(TvShowCrawlerEditorTest, shouldNotAllowSettingTvShowNameIfNoCrawlerIsBeingEdited) {
    EXPECT_THROW(editor.setTvShowName(mandalorian), std::logic_error);
}

TEST_F(TvShowCrawlerEditorTest, shouldFailToGetNameOfTvShowSinceTvShowCrawlerHasNotBeenCreatedYet) {
    EXPECT_THROW((void)editor.getTvShowName(), std::logic_error);
}

TEST_F(TvShowCrawlerEditorTest, shouldReturnPreviouslySetNameOfTvShowCrawledByEditedCrawler) {
    editor.createTvShowCrawler();
    editor.setTvShowName(friends);
    EXPECT_EQ(friends, editor.getTvShowName());
    editor.setTvShowName(mandalorian);
    EXPECT_EQ(mandalorian, editor.getTvShowName());
}

TEST_F(TvShowCrawlerEditorTest, shouldFailToSaveAndRunCrawlerThatHasTvShowNameAndEmptyCrawlersDueToExpectedError) {
    EXPECT_CALL(runtime, crawlTvShowAndSaveCrawler(emptyCrawler))
        .WillOnce(::testing::Throw(serio::core::TvShowCrawlerExecutionError(emptyCrawler.getTvShowName(), std::runtime_error("expected error"))));
    try {
        editor.createTvShowCrawler();
        editor.setTvShowName(emptyCrawler.getTvShowName());
        editor.saveAndRunCrawler();
        FAIL();
    } catch (std::runtime_error& e) {
        EXPECT_STREQ("Failed to save and run edited crawler: Failed to crawl 'Friends': expected error", e.what());
    }
}

TEST_F(TvShowCrawlerEditorTest, shouldResetEditedCrawlerAfterSavingIt) {
    EXPECT_CALL(runtime, crawlTvShowAndSaveCrawler(emptyCrawler));
    editor.createTvShowCrawler();
    editor.setTvShowName(emptyCrawler.getTvShowName());
    editor.saveAndRunCrawler();
    EXPECT_THROW(editor.setTvShowName(mandalorian), std::logic_error);
}

TEST_F(TvShowCrawlerEditorTest, shouldResetEditedCrawlerTvShowNameAfterSavingIt) {
    EXPECT_CALL(runtime, crawlTvShowAndSaveCrawler(emptyCrawler));
    editor.createTvShowCrawler();
    editor.setTvShowName(emptyCrawler.getTvShowName());
    editor.saveAndRunCrawler();
    editor.createTvShowCrawler();
    EXPECT_THROW(editor.saveAndRunCrawler(), std::logic_error);
}

TEST_F(TvShowCrawlerEditorTest, shouldFailToGetStepsOfCrawlerIfNoCrawlerIsBeingEdited) {
    EXPECT_THROW((void)editor.getStepsOfCrawler(serio::core::CrawlerType::episodeVideoCrawler), std::logic_error);
}

TEST_F(TvShowCrawlerEditorTest, shouldReturnEmptyVectorOfStepsOfCrawlerThatHasJustBeenCreated) {
    editor.createTvShowCrawler();
    EXPECT_TRUE(editor.getStepsOfCrawler(serio::core::CrawlerType::episodeVideoCrawler).empty());
}

TEST_F(TvShowCrawlerEditorTest, shouldReturnVectorContainingPreviouslyAddedStep) {
    editor.createTvShowCrawler();
    editor.addStepToCrawler(serio::core::CrawlerType::episodeVideoCrawler, valueStep);
    EXPECT_EQ(std::vector({valueStep}), editor.getStepsOfCrawler(serio::core::CrawlerType::episodeVideoCrawler));
    EXPECT_TRUE(editor.getStepsOfCrawler(serio::core::CrawlerType::thumbnailCrawler).empty());
}

TEST_F(TvShowCrawlerEditorTest, shouldFailToAddStepToCrawlerThatIsNotBeingEdited) {
    EXPECT_THROW(editor.addStepToCrawler(serio::core::CrawlerType::episodeVideoCrawler, valueStep), std::logic_error);
}

TEST_F(TvShowCrawlerEditorTest, shouldFailToRemoveCrawlerStepIfNoCrawlerIsBeingEdited) {
    EXPECT_THROW(editor.removeStepFromCrawler(serio::core::CrawlerType::episodeVideoCrawler, 1), std::logic_error);
}

TEST_F(TvShowCrawlerEditorTest, shouldFailToRemoveCrawlerStepThatDoesNotExist) {
    editor.createTvShowCrawler();
    EXPECT_THROW(editor.removeStepFromCrawler(serio::core::CrawlerType::episodeVideoCrawler, 1), std::out_of_range);
}

TEST_F(TvShowCrawlerEditorTest, shouldRemoveSpecifiedStepFromEditedCrawler) {
    editor.createTvShowCrawler();
    editor.addStepToCrawler(serio::core::CrawlerType::episodeVideoCrawler, valueStep);
    editor.addStepToCrawler(serio::core::CrawlerType::episodeVideoCrawler, fetchStep);
    editor.removeStepFromCrawler(serio::core::CrawlerType::episodeVideoCrawler, 0);
    EXPECT_EQ(std::vector({fetchStep}), editor.getStepsOfCrawler(serio::core::CrawlerType::episodeVideoCrawler));
}

TEST_F(TvShowCrawlerEditorTest, shouldFailToReplaceStepInCrawlerThatIsNotBeingEdited) {
    EXPECT_THROW(editor.replaceStepInCrawler(serio::core::CrawlerType::episodeVideoCrawler, 1, valueStep), std::logic_error);
}

TEST_F(TvShowCrawlerEditorTest, shouldFailToReplaceCrawlerStepThatDoesNotExist) {
    editor.createTvShowCrawler();
    EXPECT_THROW(editor.replaceStepInCrawler(serio::core::CrawlerType::episodeVideoCrawler, 0, valueStep), std::out_of_range);
}

TEST_F(TvShowCrawlerEditorTest, shouldReplaceSpecifiedStepInEditedCrawler) {
    editor.createTvShowCrawler();
    editor.addStepToCrawler(serio::core::CrawlerType::episodeVideoCrawler, valueStep);
    editor.addStepToCrawler(serio::core::CrawlerType::episodeVideoCrawler, valueStep);
    editor.replaceStepInCrawler(serio::core::CrawlerType::episodeVideoCrawler, 1, fetchStep);
    EXPECT_EQ(std::vector({valueStep, fetchStep}), editor.getStepsOfCrawler(serio::core::CrawlerType::episodeVideoCrawler));
}

TEST_F(TvShowCrawlerEditorTest, shouldSaveAndRunCreatedTvShowCrawler) {
    serio::core::TvShowCrawler tvShowCrawler(
            friends,
            serio::core::Crawler({valueStep, fetchStep}),
            serio::core::Crawler({valueStep}),
            serio::core::Crawler({fetchStep}));
    EXPECT_CALL(runtime, crawlTvShowAndSaveCrawler(tvShowCrawler));
    editor.createTvShowCrawler();
    editor.setTvShowName(friends);
    editor.addStepToCrawler(serio::core::CrawlerType::episodeVideoCrawler, valueStep);
    editor.addStepToCrawler(serio::core::CrawlerType::episodeVideoCrawler, fetchStep);
    editor.addStepToCrawler(serio::core::CrawlerType::episodeNameCrawler, fetchStep);
    editor.addStepToCrawler(serio::core::CrawlerType::thumbnailCrawler, valueStep);
    editor.saveAndRunCrawler();
}
