#include "tv-show-crawler-editor/TvShowCrawlerBuilder.h"

serio::core::TvShowCrawlerBuilder::TvShowCrawlerBuilder() : crawlerTypeToSteps({
    {CrawlerType::episodeVideoCrawler, {}},
    {CrawlerType::thumbnailCrawler, {}},
    {CrawlerType::episodeNameCrawler, {}}
}) {}

void serio::core::TvShowCrawlerBuilder::setTvShowName(std::string name) {
    tvShowName = std::move(name);
}

void serio::core::TvShowCrawlerBuilder::editCrawler(serio::core::CrawlerType type) {
    editedCrawlerSteps = crawlerTypeToSteps[type];
    editedCrawlerType = type;
}

std::string serio::core::TvShowCrawlerBuilder::getTvShowName() const {
    return tvShowName;
}

void serio::core::TvShowCrawlerBuilder::addCrawlerStep(serio::core::CrawlerStep step) {
    assertCrawlerIsEdited();
    editedCrawlerSteps.push_back(std::move(step));
}

void serio::core::TvShowCrawlerBuilder::replaceCrawlerStep(unsigned int stepIndex, serio::core::CrawlerStep newStep) {
    assertCrawlerIsEdited();
    assertCrawlerStepExist(stepIndex);
    editedCrawlerSteps[stepIndex] = std::move(newStep);
}

void serio::core::TvShowCrawlerBuilder::removeCrawlerStep(unsigned int stepIndex) {
    assertCrawlerIsEdited();
    assertCrawlerStepExist(stepIndex);
    editedCrawlerSteps.erase(editedCrawlerSteps.begin() + stepIndex);
}

void serio::core::TvShowCrawlerBuilder::saveCrawler() {
    assertCrawlerIsEdited();
    crawlerTypeToSteps[*editedCrawlerType] = editedCrawlerSteps;
    editedCrawlerType.reset();
}

serio::core::TvShowCrawler serio::core::TvShowCrawlerBuilder::buildTvShowCrawler() const {
    assertTvShowIsSpecified();
    return TvShowCrawler(
            tvShowName,
            Crawler(crawlerTypeToSteps.at(CrawlerType::episodeVideoCrawler)),
            Crawler(crawlerTypeToSteps.at(CrawlerType::thumbnailCrawler)),
            Crawler(crawlerTypeToSteps.at(CrawlerType::episodeNameCrawler)));
}

std::vector<serio::core::CrawlerStep> serio::core::TvShowCrawlerBuilder::getCrawlerSteps() const {
    assertCrawlerIsEdited();
    return editedCrawlerSteps;
}

void serio::core::TvShowCrawlerBuilder::assertCrawlerIsEdited() const {
    if (!editedCrawlerType) {
        throw NoCrawlerEditedError();
    }
}

void serio::core::TvShowCrawlerBuilder::assertCrawlerStepExist(unsigned int stepIndex) const {
    if (stepIndex >= editedCrawlerSteps.size()) {
        throw CrawlerStepDoesNotExist(stepIndex);
    }
}

void serio::core::TvShowCrawlerBuilder::assertTvShowIsSpecified() const {
    if (tvShowName.empty()) {
        throw TvShowNameNotSpecifiedError();
    }
}

serio::core::NoCrawlerEditedError::NoCrawlerEditedError() : std::logic_error("No crawler is being edited right now") {}

serio::core::CrawlerStepDoesNotExist::CrawlerStepDoesNotExist(unsigned int stepIndex)
        : std::out_of_range("Edited crawler does not have a step with index " + std::to_string(stepIndex)) {}

serio::core::TvShowNameNotSpecifiedError::TvShowNameNotSpecifiedError()
        : std::logic_error("TV Show name for edited crawler was not specified") {}

serio::core::TvShowCrawlerEditorError::TvShowCrawlerEditorError(const std::runtime_error &cause)
        : runtime_error(std::string("Failed to save and run edited tv-show crawler: ") + cause.what()) {}
