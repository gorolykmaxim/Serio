#include "tv-show-crawler-runtime/CrawlerSerializer.h"

std::string serio::core::CrawlerSerializer::serialize(const TvShowCrawler& crawler) const {
    nlohmann::json serializedCrawler;
    serializedCrawler["showName"] = crawler.getTvShowName();
    serializedCrawler["episodeVideoCrawler"] = serialize(crawler.getEpisodeVideoCrawler());
    serializedCrawler["thumbnailCrawler"] = serialize(crawler.getThumbnailCrawler());
    serializedCrawler["episodeNameCrawler"] = serialize(crawler.getEpisodeNameCrawler());
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
