#include <tv-show-crawler-runtime/model/TvShowCrawler.h>
#include "tv-show-crawler-runtime/executor/CrawlerExecutor.h"

void serio::core::CrawlerExecutor::registerCrawlerStepExecutor(const std::string& type,
                                                               std::unique_ptr<CrawlerStepExecutor> executor) {
    crawlerStepTypeToExecutor[type] = std::move(executor);
}

std::string serio::core::CrawlerExecutor::executeThumbnailCrawler(const serio::core::Crawler &crawler) const {
    std::vector<std::string> results = executeCrawler(crawler, "thumbnail");
    return results.empty() ? "" : results[0];
}

std::vector<serio::core::Episode> serio::core::CrawlerExecutor::executeEpisodeCrawler(const Crawler& videoCrawler, const Crawler& nameCrawler) const {
    std::vector<std::string> videoUrls = crawlEpisodeVideos(videoCrawler);
    std::vector<std::string> names = crawlEpisodeNamesIfNecessary(nameCrawler, videoUrls);
    std::vector<Episode> episodes;
    episodes.reserve(videoUrls.size());
    if (!names.empty() && videoUrls.size() != names.size()) {
        throw EpisodeVideosToNamesMismatchError(videoUrls.size(), names.size());
    }
    for (int i = 0; i < videoUrls.size(); i++) {
        episodes.emplace_back(i + 1, videoUrls[i], names.empty() ? std::optional<std::string>() : names[i]);
    }
    return episodes;
}

std::vector<std::string> serio::core::CrawlerExecutor::crawlEpisodeVideos(const serio::core::Crawler &crawler) const {
    return executeCrawler(crawler, "episode video");
}

std::vector<std::string> serio::core::CrawlerExecutor::crawlEpisodeNamesIfNecessary(const serio::core::Crawler &crawler,
                                                                                    const std::vector<std::string>& videoUrls) const {
    return crawler.hasSteps() ? executeCrawler(crawler, "episode name", videoUrls) : std::vector<std::string>();
}

std::vector<std::string> serio::core::CrawlerExecutor::executeCrawler(const serio::core::Crawler &crawler, const std::string& crawlerType, std::vector<std::string> result) const {
    try {
        std::vector<CrawlerStep> steps = crawler.getSteps();
        for (int i = 0; i < steps.size(); ++i) {
            result = executeCrawlerStep(result, steps[i], i + 1);
        }
        return result;
    } catch (CrawlerStepExecutionError& e) {
        throw CrawlerExecutionError(crawlerType, e);
    }
}

std::vector<std::string> serio::core::CrawlerExecutor::executeCrawlerStep(const std::vector<std::string>& previousStepResults,
                                                                          const serio::core::CrawlerStep &step,
                                                                          unsigned int stepNumber) const {
    try {
        auto executor = crawlerStepTypeToExecutor.find(step.getType());
        if (executor == crawlerStepTypeToExecutor.cend()) {
            throw CrawlerStepTypeError(step.getType());
        }
        return executor->second->execute(step, previousStepResults);
    } catch (std::runtime_error& e) {
        throw CrawlerStepExecutionError(stepNumber, e);
    }
}