#include "tv-show-crawler-editor/TvShowCrawlerEditor.h"
#include <utility>

serio::core::TvShowCrawlerEditor::TvShowCrawlerEditor(serio::core::TvShowCrawlerRuntime &runtime)
    : runtime(runtime) {}

void serio::core::TvShowCrawlerEditor::createTvShowCrawler() {
    builder = TvShowCrawlerBuilder();
}

void serio::core::TvShowCrawlerEditor::importTvShowCrawler(const std::string &rawCrawler) {
    TvShowCrawler crawler = runtime.deserializeTvShowCrawler(rawCrawler);
    createTvShowCrawler();
    setTvShowName(crawler.getTvShowName());
    addCrawlerStepsFrom(CrawlerType::episodeVideoCrawler, crawler);
    addCrawlerStepsFrom(CrawlerType::thumbnailCrawler, crawler);
    addCrawlerStepsFrom(CrawlerType::episodeNameCrawler, crawler);
}

void serio::core::TvShowCrawlerEditor::setTvShowName(std::string name) {
    getBuilderOrFail().setTvShowName(std::move(name));
}

void serio::core::TvShowCrawlerEditor::editCrawler(serio::core::CrawlerType type) {
    getBuilderOrFail().editCrawler(type);
}

std::string serio::core::TvShowCrawlerEditor::getTvShowName() const {
    return getBuilderOrFail().getTvShowName();
}

void serio::core::TvShowCrawlerEditor::addCrawlerStep(serio::core::CrawlerStep step) {
    getBuilderOrFail().addCrawlerStep(std::move(step));
}

void serio::core::TvShowCrawlerEditor::replaceCrawlerStep(unsigned int stepIndex, serio::core::CrawlerStep newStep) {
    getBuilderOrFail().replaceCrawlerStep(stepIndex, std::move(newStep));
}

void serio::core::TvShowCrawlerEditor::removeCrawlerStep(unsigned int stepIndex) {
    getBuilderOrFail().removeCrawlerStep(stepIndex);
}

void serio::core::TvShowCrawlerEditor::saveCrawler() {
    getBuilderOrFail().saveCrawler();
}

void serio::core::TvShowCrawlerEditor::saveAndRunTvShowCrawler() {
    try {
        assertTvShowCrawlerIsEdited();
        runtime.crawlTvShowAndSaveCrawler(getBuilderOrFail().buildTvShowCrawler());
        builder.reset();
    } catch (std::runtime_error& e) {
        throw TvShowCrawlerEditorError(e);
    }
}

serio::core::CrawlResult serio::core::TvShowCrawlerEditor::previewCrawlerWithLogs() {
    serio::core::Crawler crawler(getBuilderOrFail().getCrawlerSteps());
    return runtime.executeCrawler(crawler);
}

bool serio::core::TvShowCrawlerEditor::willOverrideExistingTvShow() {
    return runtime.willOverrideExistingTvShow(getBuilderOrFail().buildTvShowCrawler());
}

std::vector<serio::core::CrawlerStep> serio::core::TvShowCrawlerEditor::getCrawlerSteps() const {
    return getBuilderOrFail().getCrawlerSteps();
}

std::vector<serio::core::CrawlerStepType> serio::core::TvShowCrawlerEditor::getCrawlerStepTypes() const {
    return runtime.getCrawlerStepTypes();
}

void serio::core::TvShowCrawlerEditor::assertTvShowCrawlerIsEdited() const {
    if (!builder) {
        throw NoTvShowCrawlerEditedError();
    }
}

serio::core::TvShowCrawlerBuilder &serio::core::TvShowCrawlerEditor::getBuilderOrFail() {
    assertTvShowCrawlerIsEdited();
    return *builder;
}

const serio::core::TvShowCrawlerBuilder &serio::core::TvShowCrawlerEditor::getBuilderOrFail() const {
    assertTvShowCrawlerIsEdited();
    return *builder;
}

void serio::core::TvShowCrawlerEditor::addCrawlerStepsFrom(serio::core::CrawlerType type,
                                                           const TvShowCrawler &crawler) {
    editCrawler(type);
    for (const auto& step: crawler.getCrawler(type).getSteps()) {
        addCrawlerStep(step);
    }
    saveCrawler();
}

serio::core::NoTvShowCrawlerEditedError::NoTvShowCrawlerEditedError()
    : std::logic_error("No tv-show crawler is being edited right now") {}

serio::core::TvShowCrawlerEditorError::TvShowCrawlerEditorError(const std::runtime_error &cause)
        : runtime_error(std::string("Failed to save and run edited tv-show crawler: ") + cause.what()) {}