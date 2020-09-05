#include "tv-show-crawler-runtime/CrawlerSerializer.h"

std::string serio::core::CrawlerSerializer::serialize(const TvShowCrawler& crawler) const {
    nlohmann::json serializedCrawler;
    serializedCrawler["showName"] = crawler.getTvShowName();
    serializedCrawler["episodeVideoCrawler"] = serialize(crawler.getCrawler(CrawlerType::episodeVideoCrawler));
    serializedCrawler["thumbnailCrawler"] = serialize(crawler.getCrawler(CrawlerType::thumbnailCrawler));
    serializedCrawler["episodeNameCrawler"] = serialize(crawler.getCrawler(CrawlerType::episodeNameCrawler));
    return serializedCrawler.dump();
}

nlohmann::json serio::core::CrawlerSerializer::serialize(const serio::core::Crawler &crawler) const {
    nlohmann::json serializedSteps = std::vector<nlohmann::json>();
    for (const CrawlerStep& step: crawler.getSteps()) {
        serializedSteps.push_back(serialize(step));
    }
    return {{"steps", serializedSteps}};
}

nlohmann::json serio::core::CrawlerSerializer::serialize(const CrawlerStep &step) const {
    nlohmann::json serializedStep = {{"type", step.getType()}};
    for (const auto& [k, v] : step.getProperties()) {
        serializedStep[k] = v;
    }
    return serializedStep;
}
