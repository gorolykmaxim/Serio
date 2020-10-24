#include "tv-show-crawler-editor/TvShowCrawlerEditor.h"
#include <utility>

namespace serio::core {

TvShowCrawlerEditor::TvShowCrawlerEditor(TvShowCrawlerRuntime &runtime)
    : runtime(runtime) {}

void TvShowCrawlerEditor::createTvShowCrawler() {
    builder = TvShowCrawlerBuilder();
}

void TvShowCrawlerEditor::importTvShowCrawler(const std::string &rawCrawler) {
    setTvShowCrawler(runtime.deserializeTvShowCrawler(rawCrawler));
}

void TvShowCrawlerEditor::editTvShowCrawler(const std::string &tvShowName) {
    setTvShowCrawler(runtime.getTvShowCrawlerByTvShowNameOrFail(tvShowName));
}

void TvShowCrawlerEditor::setTvShowName(std::string name) {
    getBuilderOrFail().setTvShowName(std::move(name));
}

void TvShowCrawlerEditor::editCrawler(CrawlerType type) {
    getBuilderOrFail().editCrawler(type);
}

std::string TvShowCrawlerEditor::getTvShowName() const {
    return getBuilderOrFail().getTvShowName();
}

void TvShowCrawlerEditor::addCrawlerStep(CrawlerStep step) {
    getBuilderOrFail().addCrawlerStep(std::move(step));
}

void TvShowCrawlerEditor::replaceCrawlerStep(unsigned int stepIndex, CrawlerStep newStep) {
    getBuilderOrFail().replaceCrawlerStep(stepIndex, std::move(newStep));
}

void TvShowCrawlerEditor::removeCrawlerStep(unsigned int stepIndex) {
    getBuilderOrFail().removeCrawlerStep(stepIndex);
}

void TvShowCrawlerEditor::saveCrawler() {
    getBuilderOrFail().saveCrawler();
}

void TvShowCrawlerEditor::saveAndRunTvShowCrawler() {
    try {
        assertTvShowCrawlerIsEdited();
        runtime.crawlTvShowAndSaveCrawler(getBuilderOrFail().buildTvShowCrawler());
        builder.reset();
    } catch (std::runtime_error& e) {
        throw TvShowCrawlerEditorError(e);
    }
}

CrawlResult TvShowCrawlerEditor::previewCrawler() {
    Crawler crawler(getBuilderOrFail().getCrawlerSteps());
    auto result = runtime.executeCrawler(crawler);
    getBuilderOrFail().setPreviewResult(result);
    return result;
}

bool TvShowCrawlerEditor::willOverrideExistingTvShow() {
    return runtime.willOverrideExistingTvShow(getBuilderOrFail().buildTvShowCrawler());
}

std::vector<CrawlerStep> TvShowCrawlerEditor::getCrawlerSteps() const {
    return getBuilderOrFail().getCrawlerSteps();
}

std::vector<CrawlerStepType> TvShowCrawlerEditor::getCrawlerStepTypes() const {
    return runtime.getCrawlerStepTypes();
}

std::vector<std::string> TvShowCrawlerEditor::getPreviewedCrawlerResults() const {
    return getBuilderOrFail().getPreviewedCrawlerResults();
}

std::vector<CrawlLogEntry> TvShowCrawlerEditor::getPreviewedCrawlerLog() const {
    return getBuilderOrFail().getPreviewedCrawlerLog();
}

void TvShowCrawlerEditor::assertTvShowCrawlerIsEdited() const {
    if (!builder) {
        throw NoTvShowCrawlerEditedError();
    }
}

TvShowCrawlerBuilder &TvShowCrawlerEditor::getBuilderOrFail() {
    assertTvShowCrawlerIsEdited();
    return *builder;
}

const TvShowCrawlerBuilder &TvShowCrawlerEditor::getBuilderOrFail() const {
    assertTvShowCrawlerIsEdited();
    return *builder;
}

void TvShowCrawlerEditor::addCrawlerStepsFrom(CrawlerType type, const TvShowCrawler &crawler) {
    editCrawler(type);
    for (const auto& step: crawler.getCrawler(type).getSteps()) {
        addCrawlerStep(step);
    }
    saveCrawler();
}

void TvShowCrawlerEditor::setTvShowCrawler(const TvShowCrawler &crawler) {
    createTvShowCrawler();
    setTvShowName(crawler.getTvShowName());
    addCrawlerStepsFrom(CrawlerType::episodeVideoCrawler, crawler);
    addCrawlerStepsFrom(CrawlerType::thumbnailCrawler, crawler);
    addCrawlerStepsFrom(CrawlerType::episodeNameCrawler, crawler);
}

NoTvShowCrawlerEditedError::NoTvShowCrawlerEditedError()
    : std::logic_error("No tv-show crawler is being edited right now") {}

TvShowCrawlerEditorError::TvShowCrawlerEditorError(const std::runtime_error &cause)
    : runtime_error(std::string("Failed to save and run edited tv-show crawler: ") + cause.what()) {}

}