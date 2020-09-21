#include <gtest/gtest.h>
#include <tv-show-crawler-editor/TvShowCrawlerEditor.h>
#include <TvShowCrawlerRuntimeMock.h>

class TvShowCrawlerEditorTest : public ::testing::Test {
protected:
    const std::string mandalorian = "Mandalorian";
    const std::string rawTvShowCrawler = "tv-show-crawler";
    const std::vector<serio::core::CrawlerType> crawlerTypes = {
            serio::core::CrawlerType::episodeVideoCrawler,
            serio::core::CrawlerType::thumbnailCrawler,
            serio::core::CrawlerType::episodeNameCrawler
    };
    const serio::core::CrawlerStep fetch = serio::core::CrawlerStep("fetch");
    const serio::core::CrawlerStep value = serio::core::CrawlerStep("value", {{"value", "https://tv-show/"}});
    serio::core::TvShowCrawler tvShowCrawler = serio::core::TvShowCrawler(mandalorian, serio::core::Crawler({value}));
    TvShowCrawlerRuntimeMock runtime = TvShowCrawlerRuntimeMock::create();
    serio::core::TvShowCrawlerEditor editor = serio::core::TvShowCrawlerEditor(runtime);

    void createCrawler() {
        editor.createTvShowCrawler();
        editor.setTvShowName(mandalorian);
        editor.editCrawler(serio::core::CrawlerType::episodeVideoCrawler);
        editor.addCrawlerStep(value);
        editor.saveCrawler();
    }
    void saveAndRunCrawler() {
        EXPECT_CALL(runtime, crawlTvShowAndSaveCrawler(tvShowCrawler));
        createCrawler();
        editor.saveAndRunTvShowCrawler();
    }
    void expectCrawlerStepsToBeImported(serio::core::CrawlerType type) {
        EXPECT_CALL(runtime, deserializeTvShowCrawler(rawTvShowCrawler)).WillOnce(::testing::Return(tvShowCrawler));
        editor.importTvShowCrawler(rawTvShowCrawler);
        editor.editCrawler(type);
        EXPECT_EQ(tvShowCrawler.getCrawler(type).getSteps(), editor.getCrawlerSteps());
    }
};

TEST_F(TvShowCrawlerEditorTest, shouldFailToChangeTvShowNameWhenNoTvShowCrawlerBeingEdited) {
    EXPECT_THROW(editor.setTvShowName(""), std::logic_error);
}

TEST_F(TvShowCrawlerEditorTest, shouldFailToGetTvShowNameWhenNoTvShowCrawlerBeingEdited) {
    EXPECT_THROW((void)editor.getTvShowName(), std::logic_error);
}

TEST_F(TvShowCrawlerEditorTest, shouldSetTvShowNameToTheSpecifiedOne) {
    editor.createTvShowCrawler();
    editor.setTvShowName(mandalorian);
    EXPECT_EQ(mandalorian, editor.getTvShowName());
    editor.setTvShowName("Friends");
    EXPECT_EQ("Friends", editor.getTvShowName());
}

TEST_F(TvShowCrawlerEditorTest, shouldFailToEditCrawlerOfTvShowCrawlerIfNoTvShowCrawlerBeingEdited) {
    EXPECT_THROW(editor.editCrawler(serio::core::CrawlerType::episodeVideoCrawler), std::logic_error);
}

TEST_F(TvShowCrawlerEditorTest, shouldFailToGetVectorOfCrawlerStepsifNoCrawlerBeingEdited) {
    EXPECT_THROW((void)editor.getCrawlerSteps(), std::logic_error);
}

TEST_F(TvShowCrawlerEditorTest, shouldGetEmptyVectorOfCrawlerStepsForNewlyCreatedCrawlerBeingEdited) {
    editor.createTvShowCrawler();
    for (auto type: crawlerTypes) {
        editor.editCrawler(type);
        EXPECT_TRUE(editor.getCrawlerSteps().empty());
    }
}

TEST_F(TvShowCrawlerEditorTest, shouldFailToAddStepToCrawlerIfNoCrawlerBeingEdited) {
    EXPECT_THROW(editor.addCrawlerStep(fetch), std::logic_error);
}

TEST_F(TvShowCrawlerEditorTest, shouldAddStepToCrawler) {
    editor.createTvShowCrawler();
    for (auto type: crawlerTypes) {
        editor.editCrawler(type);
        editor.addCrawlerStep(fetch);
        EXPECT_EQ(std::vector({fetch}), editor.getCrawlerSteps());
    }
}

TEST_F(TvShowCrawlerEditorTest, shouldNotSaveAddedStepToTheActualCrawler) {
    editor.createTvShowCrawler();
    for (auto type: crawlerTypes) {
        editor.editCrawler(type);
        editor.addCrawlerStep(fetch);
        editor.editCrawler(type);
        EXPECT_TRUE(editor.getCrawlerSteps().empty());
    }
}

TEST_F(TvShowCrawlerEditorTest, shouldFailToSaveCrawlerThatIsNotBeingEdited) {
    EXPECT_THROW(editor.saveCrawler(), std::logic_error);
}

TEST_F(TvShowCrawlerEditorTest, shouldSaveAddedStepToTheActualCrawler) {
    editor.createTvShowCrawler();
    for (auto type: crawlerTypes) {
        editor.editCrawler(type);
        editor.addCrawlerStep(fetch);
        editor.saveCrawler();
        editor.editCrawler(type);
        EXPECT_EQ(std::vector({fetch}), editor.getCrawlerSteps());
    }
}

TEST_F(TvShowCrawlerEditorTest, shouldFailToAccessCrawlerThatHasBeenAlreadySaved) {
    editor.createTvShowCrawler();
    for (auto type: crawlerTypes) {
        editor.editCrawler(type);
        editor.addCrawlerStep(fetch);
        editor.saveCrawler();
        EXPECT_THROW((void)editor.getCrawlerSteps(), std::logic_error);
        EXPECT_THROW(editor.addCrawlerStep(fetch), std::logic_error);
    }
}

TEST_F(TvShowCrawlerEditorTest, shouldOnlyModifyStepsOfTheSpecifiedCrawler) {
    editor.createTvShowCrawler();
    editor.editCrawler(serio::core::CrawlerType::episodeVideoCrawler);
    editor.addCrawlerStep(fetch);
    editor.saveCrawler();
    editor.editCrawler(serio::core::CrawlerType::thumbnailCrawler);
    EXPECT_TRUE(editor.getCrawlerSteps().empty());
}

TEST_F(TvShowCrawlerEditorTest, shouldFailToReplaceStepOfCrawlerThatIsNotBeingEdited) {
    EXPECT_THROW(editor.replaceCrawlerStep(1, fetch), std::logic_error);
}

TEST_F(TvShowCrawlerEditorTest, shouldFailToReplaceStepOfEditedCrawlerThatDoesNotExist) {
    editor.createTvShowCrawler();
    for (auto type: crawlerTypes) {
        editor.editCrawler(type);
        EXPECT_THROW(editor.replaceCrawlerStep(1, fetch), std::out_of_range);
    }
}

TEST_F(TvShowCrawlerEditorTest, shouldReplaceStepOfEditedCrawler) {
    editor.createTvShowCrawler();
    for (auto type: crawlerTypes) {
        editor.editCrawler(type);
        editor.addCrawlerStep(value);
        editor.addCrawlerStep(value);
        editor.replaceCrawlerStep(1, fetch);
        EXPECT_EQ(std::vector({value, fetch}), editor.getCrawlerSteps());
    }
}

TEST_F(TvShowCrawlerEditorTest, shouldFailToRemoveStepOfCrawlerThatIsNotBeingEdited) {
    EXPECT_THROW(editor.removeCrawlerStep(1), std::logic_error);
}

TEST_F(TvShowCrawlerEditorTest, shouldFailToRemoveStepOfEditedCrawlerThatDoesNotExist) {
    editor.createTvShowCrawler();
    for (auto type: crawlerTypes) {
        editor.editCrawler(type);
        EXPECT_THROW(editor.removeCrawlerStep(1), std::out_of_range);
    }
}

TEST_F(TvShowCrawlerEditorTest, shouldRemoveStepOfEditedCrawler) {
    editor.createTvShowCrawler();
    for (auto type: crawlerTypes) {
        editor.editCrawler(type);
        editor.addCrawlerStep(fetch);
        editor.addCrawlerStep(fetch);
        editor.removeCrawlerStep(1);
        EXPECT_EQ(std::vector({fetch}), editor.getCrawlerSteps());
    }
}

TEST_F(TvShowCrawlerEditorTest, shouldFailToSaveAndRunTvShowCrawlerThatHasNotBeenCreated) {
    EXPECT_THROW(editor.saveAndRunTvShowCrawler(), std::logic_error);
}

TEST_F(TvShowCrawlerEditorTest, shouldFailToSaveAndRunCrawlerWithEmptyTvShowName) {
    editor.createTvShowCrawler();
    EXPECT_THROW(editor.saveAndRunTvShowCrawler(), std::logic_error);
}

TEST_F(TvShowCrawlerEditorTest, shouldSaveAndRunTvShowCrawlerWithNoStepsInAnyOfItsCrawlers) {
    saveAndRunCrawler();
}

TEST_F(TvShowCrawlerEditorTest, shouldSaveAndRunTvShowCrawlerWithNonEmptyCrawlers) {
    tvShowCrawler = serio::core::TvShowCrawler(
            mandalorian,
            serio::core::Crawler({value, fetch}),
            serio::core::Crawler({value}),
            serio::core::Crawler({fetch}));
    EXPECT_CALL(runtime, crawlTvShowAndSaveCrawler(tvShowCrawler));
    editor.createTvShowCrawler();
    editor.setTvShowName(mandalorian);
    editor.editCrawler(serio::core::CrawlerType::episodeVideoCrawler);
    editor.addCrawlerStep(value);
    editor.addCrawlerStep(fetch);
    editor.saveCrawler();
    editor.editCrawler(serio::core::CrawlerType::thumbnailCrawler);
    editor.addCrawlerStep(value);
    editor.saveCrawler();
    editor.editCrawler(serio::core::CrawlerType::episodeNameCrawler);
    editor.addCrawlerStep(fetch);
    editor.saveCrawler();
    editor.saveAndRunTvShowCrawler();
}

TEST_F(TvShowCrawlerEditorTest, shouldFailToSaveAndRunTvShowCrawlerDueToRuntimeError) {
    EXPECT_CALL(runtime, crawlTvShowAndSaveCrawler(::testing::_))
        .WillOnce(::testing::Throw(std::runtime_error("expected error")));
    editor.createTvShowCrawler();
    editor.setTvShowName(mandalorian);
    try {
        editor.saveAndRunTvShowCrawler();
        FAIL();
    } catch (std::runtime_error& e) {
        EXPECT_STREQ("Failed to save and run edited tv-show crawler: expected error", e.what());
    }
}

TEST_F(TvShowCrawlerEditorTest, shouldResetCurrentlyEditedTvShowCrawlerAfterSavingit) {
    saveAndRunCrawler();
    EXPECT_THROW(editor.setTvShowName(mandalorian), std::logic_error);
    EXPECT_THROW((void)editor.getTvShowName(), std::logic_error);
    EXPECT_THROW(editor.editCrawler(serio::core::CrawlerType::episodeVideoCrawler), std::logic_error);
}

TEST_F(TvShowCrawlerEditorTest, shouldCreateNewTvShowCrawlerWithEmptyNameAndCrawlers) {
    saveAndRunCrawler();
    editor.createTvShowCrawler();
    editor.editCrawler(serio::core::CrawlerType::episodeVideoCrawler);
    EXPECT_TRUE(editor.getTvShowName().empty());
    EXPECT_TRUE(editor.getCrawlerSteps().empty());
}

TEST_F(TvShowCrawlerEditorTest, shouldReturnAllExistingCrawlerStepTypesDefinedInRuntime) {
    std::vector<serio::core::CrawlerStepType> expectedTypes = {
            serio::core::CrawlerStepType("fetch", ""),
            serio::core::CrawlerStepType("value", "description", {"value"})
    };
    EXPECT_CALL(runtime, getCrawlerStepTypes()).WillOnce(::testing::Return(expectedTypes));
    EXPECT_EQ(expectedTypes, editor.getCrawlerStepTypes());
}

TEST_F(TvShowCrawlerEditorTest, shouldFailToCheckIfEditedCrawlerWillOverrideExistingTvShowSinceNoCrawlerIsBeingEdited) {
    EXPECT_THROW(editor.willOverrideExistingTvShow(), std::logic_error);
}

TEST_F(TvShowCrawlerEditorTest, shouldFailToCheckIfEditedCrawlerWillOverrideExistingTvShowSinceTvShowNameIsNotSpecified) {
    editor.createTvShowCrawler();
    EXPECT_THROW(editor.willOverrideExistingTvShow(), std::logic_error);
}

TEST_F(TvShowCrawlerEditorTest, shouldTellThatEditedCrawlerWillNotOverrideAnyOfTheExistingTvShows) {
    EXPECT_CALL(runtime, willOverrideExistingTvShow(tvShowCrawler)).WillOnce(::testing::Return(false));
    createCrawler();
    EXPECT_FALSE(editor.willOverrideExistingTvShow());
}

TEST_F(TvShowCrawlerEditorTest, shouldTellThatEditedCrawlerWillOverrideExistingTvShow) {
    EXPECT_CALL(runtime, willOverrideExistingTvShow(tvShowCrawler)).WillOnce(::testing::Return(true));
    createCrawler();
    EXPECT_TRUE(editor.willOverrideExistingTvShow());
}

TEST_F(TvShowCrawlerEditorTest, shouldTellThatEditedNonEmptyCrawlerWillOverrideExistingTvShow) {
    tvShowCrawler = serio::core::TvShowCrawler(
            mandalorian,
            serio::core::Crawler({value}),
            serio::core::Crawler(),
            serio::core::Crawler({value, fetch}));
    EXPECT_CALL(runtime, willOverrideExistingTvShow(tvShowCrawler)).WillOnce(::testing::Return(true));
    editor.createTvShowCrawler();
    editor.setTvShowName(mandalorian);
    editor.editCrawler(serio::core::CrawlerType::episodeVideoCrawler);
    editor.addCrawlerStep(value);
    editor.saveCrawler();
    editor.editCrawler(serio::core::CrawlerType::episodeNameCrawler);
    editor.addCrawlerStep(value);
    editor.addCrawlerStep(fetch);
    editor.saveCrawler();
    EXPECT_TRUE(editor.willOverrideExistingTvShow());
}

TEST_F(TvShowCrawlerEditorTest, shouldSetTvShowNameToTheOneSpecifiedInImportedCrawler) {
    EXPECT_CALL(runtime, deserializeTvShowCrawler(rawTvShowCrawler))
        .WillOnce(::testing::Return(tvShowCrawler));
    editor.importTvShowCrawler(rawTvShowCrawler);
    EXPECT_EQ(tvShowCrawler.getTvShowName(), editor.getTvShowName());
}

TEST_F(TvShowCrawlerEditorTest, shouldSetEpisodeVideoCrawlerStepsToTheOnesSpecifiedInImportedCrawler) {
    expectCrawlerStepsToBeImported(serio::core::CrawlerType::episodeVideoCrawler);
}

TEST_F(TvShowCrawlerEditorTest, shouldSetThumbnailCrawlerStepsToTheOnesSpecifiedInImportedCrawler) {
    tvShowCrawler = serio::core::TvShowCrawler(mandalorian, serio::core::Crawler(), serio::core::Crawler({value, fetch}));
    expectCrawlerStepsToBeImported(serio::core::CrawlerType::thumbnailCrawler);
}

TEST_F(TvShowCrawlerEditorTest, shouldSetEpisodeNameCrawlerStepsToTheOnesSpecifiedInImportedCrawler) {
    tvShowCrawler = serio::core::TvShowCrawler(mandalorian, serio::core::Crawler(), serio::core::Crawler(), serio::core::Crawler({value}));
    expectCrawlerStepsToBeImported(serio::core::CrawlerType::episodeNameCrawler);
}

TEST_F(TvShowCrawlerEditorTest, shouldFailToPreviewCrawlerIfNoTvShowCrawlerBeingEdited) {
    EXPECT_THROW((void)editor.previewCrawler(), std::logic_error);
}

TEST_F(TvShowCrawlerEditorTest, shouldFailToPreviewCrawlerIfNoCrawlerIsBeingEdited) {
    editor.createTvShowCrawler();
    EXPECT_THROW((void)editor.previewCrawler(), std::logic_error);
}

TEST_F(TvShowCrawlerEditorTest, shouldExecuteStepsOfCurrentlyEditedCrawlerAndReturnResults) {
    std::vector<std::string> results = {"result 1", "result 2", "result 3"};
    EXPECT_CALL(runtime, executeCrawler(serio::core::Crawler({value, fetch})))
        .Times(crawlerTypes.size())
        .WillRepeatedly(::testing::Return(results));
    editor.createTvShowCrawler();
    for (serio::core::CrawlerType type: crawlerTypes) {
        editor.editCrawler(type);
        editor.addCrawlerStep(value);
        editor.addCrawlerStep(fetch);
        EXPECT_EQ(results, editor.previewCrawler());
    }
}