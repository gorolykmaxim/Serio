#include <utility>
#include "tv-show-crawler-editor/TvShowCrawlerEditor.h"

serio::core::TvShowCrawlerEditor::TvShowCrawlerEditor(serio::core::TvShowCrawlerRuntime &runtime)
    : runtime(runtime) {}

void serio::core::TvShowCrawlerEditor::createTvShowCrawler() {
    builder = TvShowCrawlerBuilder();
}

void serio::core::TvShowCrawlerEditor::setTvShowName(const std::string& name) {
    assertBuilderWasCreated();
    builder->setTvShowName(name);
}

void serio::core::TvShowCrawlerEditor::saveAndRunCrawler() {
    try {
        assertBuilderWasCreated();
        runtime.crawlTvShowAndSaveCrawler(builder->build());
        builder.reset();
    } catch (std::runtime_error& e) {
        throw CrawlerEditorError(e);
    }
}

void serio::core::TvShowCrawlerEditor::addStepToCrawler(serio::core::CrawlerType type, const serio::core::CrawlerStep& step) {
    assertBuilderWasCreated();
    builder->addStepToCrawler(type, step);
}

void serio::core::TvShowCrawlerEditor::replaceStepInCrawler(serio::core::CrawlerType type, unsigned int stepIndex,
                                                            const serio::core::CrawlerStep& step) {
    assertBuilderWasCreated();
    builder->replaceStepInCrawler(type, stepIndex, step);
}

void serio::core::TvShowCrawlerEditor::removeStepFromCrawler(serio::core::CrawlerType type, unsigned int stepIndex) {
    assertBuilderWasCreated();
    builder->removeStepFromCrawler(type, stepIndex);
}

std::string serio::core::TvShowCrawlerEditor::getTvShowName() const {
    assertBuilderWasCreated();
    return builder->getTvShowName();
}

std::vector<serio::core::CrawlerStep> serio::core::TvShowCrawlerEditor::getStepsOfCrawler(serio::core::CrawlerType type) const {
    assertBuilderWasCreated();
    return builder->getStepsOfCrawler(type);
}

std::vector<serio::core::CrawlerStepType> serio::core::TvShowCrawlerEditor::getCrawlerStepTypes() const {
    return runtime.getCrawlerStepTypes();
}

void serio::core::TvShowCrawlerEditor::assertBuilderWasCreated() const {
    if (!builder) {
        throw NoCrawlerEditedError();
    }
}

serio::core::CrawlerEditorError::CrawlerEditorError(const std::exception &cause)
    : std::runtime_error(std::string("Failed to save and run edited crawler: ") + cause.what()) {}

serio::core::NoCrawlerEditedError::NoCrawlerEditedError() : std::logic_error("No crawler is being edited right now") {}
