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
    builder = CrawlerBuilder(type, crawlerTypeToSteps[type]);
}

std::string serio::core::TvShowCrawlerBuilder::getTvShowName() const {
    return tvShowName;
}

void serio::core::TvShowCrawlerBuilder::addCrawlerStep(serio::core::CrawlerStep step) {
    getBuilderOrFail().addCrawlerStep(std::move(step));
}

void serio::core::TvShowCrawlerBuilder::replaceCrawlerStep(unsigned int stepIndex, serio::core::CrawlerStep newStep) {
    getBuilderOrFail().replaceCrawlerStep(stepIndex, std::move(newStep));
}

void serio::core::TvShowCrawlerBuilder::removeCrawlerStep(unsigned int stepIndex) {
    getBuilderOrFail().removeCrawlerStep(stepIndex);
}

void serio::core::TvShowCrawlerBuilder::saveCrawler() {
    assertCrawlerIsEdited();
    crawlerTypeToSteps[builder->getEditedCrawlerType()] = builder->getCrawlerSteps();
    builder.reset();
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
    return getBuilderOrFail().getCrawlerSteps();
}

void serio::core::TvShowCrawlerBuilder::assertCrawlerIsEdited() const {
    if (!builder) {
        throw NoCrawlerEditedError();
    }
}

serio::core::CrawlerBuilder &serio::core::TvShowCrawlerBuilder::getBuilderOrFail() {
    assertCrawlerIsEdited();
    return *builder;
}

const serio::core::CrawlerBuilder &serio::core::TvShowCrawlerBuilder::getBuilderOrFail() const {
    assertCrawlerIsEdited();
    return *builder;
}

void serio::core::TvShowCrawlerBuilder::assertTvShowIsSpecified() const {
    if (tvShowName.empty()) {
        throw TvShowNameNotSpecifiedError();
    }
}

serio::core::NoCrawlerEditedError::NoCrawlerEditedError() : std::logic_error("No crawler is being edited right now") {}

serio::core::TvShowNameNotSpecifiedError::TvShowNameNotSpecifiedError()
        : std::logic_error("TV Show name for edited crawler was not specified") {}
