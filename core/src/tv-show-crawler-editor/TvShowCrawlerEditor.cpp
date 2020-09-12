#include "tv-show-crawler-editor/TvShowCrawlerEditor.h"
#include <utility>

serio::core::TvShowCrawlerEditor::TvShowCrawlerEditor(serio::core::TvShowCrawlerRuntime &runtime)
    : runtime(runtime) {}

void serio::core::TvShowCrawlerEditor::createTvShowCrawler() {
    tvShowName = "";
}

void serio::core::TvShowCrawlerEditor::setTvShowName(std::string name) {
    assertTvShowCrawlerIsEdited();
    tvShowName = std::move(name);
}

void serio::core::TvShowCrawlerEditor::editCrawler(serio::core::CrawlerType type) {
    assertTvShowCrawlerIsEdited();
    editedCrawlerSteps = crawlerTypeToSteps[type];
    editedCrawlerType = type;
}

std::string serio::core::TvShowCrawlerEditor::getTvShowName() const {
    assertTvShowCrawlerIsEdited();
    return *tvShowName;
}

void serio::core::TvShowCrawlerEditor::addCrawlerStep(serio::core::CrawlerStep step) {
    assertCrawlerIsEdited();
    editedCrawlerSteps.push_back(std::move(step));
}

void serio::core::TvShowCrawlerEditor::replaceCrawlerStep(unsigned int stepIndex, serio::core::CrawlerStep newStep) {
    assertCrawlerIsEdited();
    assertCrawlerStepExist(stepIndex);
    editedCrawlerSteps[stepIndex] = std::move(newStep);
}

void serio::core::TvShowCrawlerEditor::removeCrawlerStep(unsigned int stepIndex) {
    assertCrawlerIsEdited();
    assertCrawlerStepExist(stepIndex);
    editedCrawlerSteps.erase(editedCrawlerSteps.begin() + stepIndex);
}

void serio::core::TvShowCrawlerEditor::saveCrawler() {
    assertCrawlerIsEdited();
    crawlerTypeToSteps[*editedCrawlerType] = editedCrawlerSteps;
    editedCrawlerType.reset();
}

void serio::core::TvShowCrawlerEditor::saveAndRunTvShowCrawler() {
    try {
        assertTvShowCrawlerIsEdited();
        assertTvShowIsSpecified();
        TvShowCrawler tvShowCrawler(
                *tvShowName,
                Crawler(crawlerTypeToSteps[CrawlerType::episodeVideoCrawler]),
                Crawler(crawlerTypeToSteps[CrawlerType::thumbnailCrawler]),
                Crawler(crawlerTypeToSteps[CrawlerType::episodeNameCrawler]));
        runtime.crawlTvShowAndSaveCrawler(tvShowCrawler);
        tvShowName.reset();
    } catch (std::runtime_error& e) {
        throw TvShowCrawlerEditorError(e);
    }
}

std::vector<serio::core::CrawlerStep> serio::core::TvShowCrawlerEditor::getCrawlerSteps() const {
    assertCrawlerIsEdited();
    return editedCrawlerSteps;
}

std::vector<serio::core::CrawlerStepType> serio::core::TvShowCrawlerEditor::getCrawlerStepTypes() const {
    return runtime.getCrawlerStepTypes();
}

void serio::core::TvShowCrawlerEditor::assertTvShowCrawlerIsEdited() const {
    if (!tvShowName) {
        throw NoTvShowCrawlerEditedError();
    }
}

void serio::core::TvShowCrawlerEditor::assertCrawlerIsEdited() const {
    if (!editedCrawlerType) {
        throw NoCrawlerEditedError();
    }
}

void serio::core::TvShowCrawlerEditor::assertCrawlerStepExist(unsigned int stepIndex) const {
    if (stepIndex >= editedCrawlerSteps.size()) {
        throw CrawlerStepDoesNotExist(stepIndex);
    }
}

void serio::core::TvShowCrawlerEditor::assertTvShowIsSpecified() const {
    if (tvShowName->empty()) {
        throw TvShowNameNotSpecifiedError();
    }
}

serio::core::NoTvShowCrawlerEditedError::NoTvShowCrawlerEditedError()
    : std::logic_error("No tv-show crawler is being edited right now") {}

serio::core::NoCrawlerEditedError::NoCrawlerEditedError() : std::logic_error("No crawler is being edited right now") {}

serio::core::CrawlerStepDoesNotExist::CrawlerStepDoesNotExist(unsigned int stepIndex)
    : std::out_of_range("Edited crawler does not have a step with index " + std::to_string(stepIndex)) {}

serio::core::TvShowNameNotSpecifiedError::TvShowNameNotSpecifiedError()
    : std::logic_error("TV Show name for edited crawler was not specified") {}

serio::core::TvShowCrawlerEditorError::TvShowCrawlerEditorError(const std::runtime_error &cause)
    : runtime_error(std::string("Failed to save and run edited tv-show crawler: ") + cause.what()) {}
