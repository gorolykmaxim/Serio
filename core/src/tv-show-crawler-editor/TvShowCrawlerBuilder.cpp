#include "tv-show-crawler-editor/TvShowCrawlerBuilder.h"

serio::core::TvShowCrawlerBuilder::TvShowCrawlerBuilder()
    : crawlerTypeToSteps({
        {CrawlerType::thumbnailCrawler, {}},
        {CrawlerType::episodeVideoCrawler, {}},
        {CrawlerType::episodeNameCrawler, {}},
    }) {}

void serio::core::TvShowCrawlerBuilder::setTvShowName(std::string name) {
    tvShowName = std::move(name);
}

void serio::core::TvShowCrawlerBuilder::addStepToCrawler(serio::core::CrawlerType type, serio::core::CrawlerStep step) {
    crawlerTypeToSteps[type].push_back(std::move(step));
}

void serio::core::TvShowCrawlerBuilder::replaceStepInCrawler(serio::core::CrawlerType type, unsigned int stepIndex,
                                                             serio::core::CrawlerStep step) {
    assertStepWithIndexExists(type, stepIndex);
    crawlerTypeToSteps[type][stepIndex] = std::move(step);
}

void serio::core::TvShowCrawlerBuilder::removeStepFromCrawler(serio::core::CrawlerType type, unsigned int stepIndex) {
    assertStepWithIndexExists(type, stepIndex);
    std::vector<CrawlerStep>& steps = crawlerTypeToSteps[type];
    steps.erase(steps.begin() + stepIndex);
}

serio::core::TvShowCrawler serio::core::TvShowCrawlerBuilder::build() const {
    if (tvShowName.empty()) {
        throw TvShowNameNotSpecifiedError();
    }
    Crawler episodeVideoCrawler = Crawler(crawlerTypeToSteps.at(CrawlerType::episodeVideoCrawler));
    Crawler thumbnailCrawler = Crawler(crawlerTypeToSteps.at(CrawlerType::thumbnailCrawler));
    Crawler episodeNameCrawler = Crawler(crawlerTypeToSteps.at(CrawlerType::episodeNameCrawler));
    return TvShowCrawler(tvShowName, episodeVideoCrawler, thumbnailCrawler, episodeNameCrawler);
}

std::string serio::core::TvShowCrawlerBuilder::getTvShowName() const {
    return tvShowName;
}

std::vector<serio::core::CrawlerStep> serio::core::TvShowCrawlerBuilder::getStepsOfCrawler(serio::core::CrawlerType type) const {
    return crawlerTypeToSteps.at(type);
}

void serio::core::TvShowCrawlerBuilder::assertStepWithIndexExists(CrawlerType type, unsigned int stepIndex) const {
    const std::vector<CrawlerStep>& steps = crawlerTypeToSteps.at(type);
    if (steps.size() <= stepIndex) {
        throw CrawlerStepDoesNotExistError(stepIndex);
    }
}

serio::core::TvShowNameNotSpecifiedError::TvShowNameNotSpecifiedError() : std::logic_error("tv-show name not specified") {}

serio::core::CrawlerStepDoesNotExistError::CrawlerStepDoesNotExistError(unsigned int stepIndex) : std::out_of_range("Step with index " + std::to_string(stepIndex) + " does not exist") {}
