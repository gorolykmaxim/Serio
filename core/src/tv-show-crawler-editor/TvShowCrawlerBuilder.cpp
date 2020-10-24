#include "tv-show-crawler-editor/TvShowCrawlerBuilder.h"

namespace serio::core {

TvShowCrawlerBuilder::TvShowCrawlerBuilder() : crawlerTypeToSteps({
    {CrawlerType::episodeVideoCrawler, {}},
    {CrawlerType::thumbnailCrawler, {}},
    {CrawlerType::episodeNameCrawler, {}}
}) {}

void TvShowCrawlerBuilder::setTvShowName(std::string name) {
    tvShowName = std::move(name);
}

void TvShowCrawlerBuilder::editCrawler(CrawlerType type) {
    builder = CrawlerBuilder(type, crawlerTypeToSteps[type]);
}

std::string TvShowCrawlerBuilder::getTvShowName() const {
    return tvShowName;
}

void TvShowCrawlerBuilder::addCrawlerStep(CrawlerStep step) {
    getBuilderOrFail().addCrawlerStep(std::move(step));
}

void TvShowCrawlerBuilder::replaceCrawlerStep(unsigned int stepIndex, CrawlerStep newStep) {
    getBuilderOrFail().replaceCrawlerStep(stepIndex, std::move(newStep));
}

void TvShowCrawlerBuilder::removeCrawlerStep(unsigned int stepIndex) {
    getBuilderOrFail().removeCrawlerStep(stepIndex);
}

void TvShowCrawlerBuilder::setPreviewResult(const CrawlResult &result) {
    getBuilderOrFail().setPreviewResult(result);
}

void TvShowCrawlerBuilder::saveCrawler() {
    assertCrawlerIsEdited();
    crawlerTypeToSteps[builder->getEditedCrawlerType()] = builder->getCrawlerSteps();
    builder.reset();
}

TvShowCrawler TvShowCrawlerBuilder::buildTvShowCrawler() const {
    assertTvShowIsSpecified();
    return TvShowCrawler(
            tvShowName,
            Crawler(crawlerTypeToSteps.at(CrawlerType::episodeVideoCrawler)),
            Crawler(crawlerTypeToSteps.at(CrawlerType::thumbnailCrawler)),
            Crawler(crawlerTypeToSteps.at(CrawlerType::episodeNameCrawler)));
}

std::vector<CrawlerStep> TvShowCrawlerBuilder::getCrawlerSteps() const {
    return getBuilderOrFail().getCrawlerSteps();
}

std::vector<std::string> TvShowCrawlerBuilder::getPreviewedCrawlerResults() const {
    return getBuilderOrFail().getPreviewedCrawlerResults();
}

std::vector<CrawlLogEntry> TvShowCrawlerBuilder::getPreviewedCrawlerLog() const {
    return getBuilderOrFail().getPreviewedCrawlerLog();
}

void TvShowCrawlerBuilder::assertCrawlerIsEdited() const {
    if (!builder) {
        throw NoCrawlerEditedError();
    }
}

CrawlerBuilder &TvShowCrawlerBuilder::getBuilderOrFail() {
    assertCrawlerIsEdited();
    return *builder;
}

const CrawlerBuilder &TvShowCrawlerBuilder::getBuilderOrFail() const {
    assertCrawlerIsEdited();
    return *builder;
}

void TvShowCrawlerBuilder::assertTvShowIsSpecified() const {
    if (tvShowName.empty()) {
        throw TvShowNameNotSpecifiedError();
    }
}

NoCrawlerEditedError::NoCrawlerEditedError() : std::logic_error("No crawler is being edited right now") {}

TvShowNameNotSpecifiedError::TvShowNameNotSpecifiedError()
    : std::logic_error("TV Show name for edited crawler was not specified") {}

}