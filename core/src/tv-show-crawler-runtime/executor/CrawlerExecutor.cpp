#include <tv-show-crawler-runtime/model/TvShowCrawler.h>
#include "tv-show-crawler-runtime/executor/CrawlerExecutor.h"

namespace serio::core {

CrawlerExecutor::CrawlerExecutor(unsigned int maxLogEntryDataSize) : maxLogEntryDataSize(maxLogEntryDataSize) {}

void CrawlerExecutor::registerCrawlerStepExecutor(const std::string& type, CrawlerStepExecutor executor) {
    crawlerStepTypeToExecutor[type] = std::move(executor);
}

std::string CrawlerExecutor::executeThumbnailCrawler(const Crawler &crawler, std::vector<CrawlLogEntry>& crawlLog) const {
    auto results = executeCrawlerAndSaveLog(crawler, "thumbnail", "Crawling thumbnail", crawlLog);
    return results.empty() ? "" : results[0];
}

std::vector<Episode> CrawlerExecutor::executeEpisodeCrawler(const Crawler& videoCrawler, const Crawler& nameCrawler,
                                                            std::vector<CrawlLogEntry>& crawlLog) const {
    auto videoUrls = crawlEpisodeVideos(videoCrawler, crawlLog);
    auto names = crawlEpisodeNamesIfNecessary(nameCrawler, videoUrls, crawlLog);
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

std::vector<std::string> CrawlerExecutor::crawlEpisodeVideos(const Crawler &crawler, std::vector<CrawlLogEntry>& crawlLog) const {
    return executeCrawlerAndSaveLog(crawler, "episode video", "Crawling episode videos", crawlLog);
}

std::vector<std::string> CrawlerExecutor::crawlEpisodeNamesIfNecessary(const Crawler &crawler,
                                                                       const std::vector<std::string>& videoUrls,
                                                                       std::vector<CrawlLogEntry>& crawlLog) const {
    if (crawler.hasSteps()) {
        return executeCrawlerAndSaveLog(crawler, "episode name", "Crawling episode names",
                                        crawlLog, videoUrls);
    } else {
        return std::vector<std::string>();
    }
}

CrawlResult CrawlerExecutor::executeCrawler(const Crawler &crawler, const std::string& crawlerType,
                                            std::vector<std::string> result) const {
    try {
        auto steps = crawler.getSteps();
        std::vector<CrawlLogEntry> log;
        log.reserve(steps.size());
        for (int i = 0; i < steps.size(); ++i) {
            CrawlLogEntry entry(steps[i]);
            entry.setStepInputData(result, maxLogEntryDataSize);
            result = executeCrawlerStep(result, steps[i], i + 1);
            entry.setStepOutputData(result, maxLogEntryDataSize);
            log.push_back(std::move(entry));
        }
        return {log, result};
    } catch (CrawlerStepExecutionError& e) {
        throw CrawlerExecutionError(crawlerType, e);
    }
}

std::vector<std::string> CrawlerExecutor::executeCrawlerStep(const std::vector<std::string>& previousStepResults,
                                                             const CrawlerStep &step,
                                                             unsigned int stepNumber) const {
    try {
        const auto& executor = crawlerStepTypeToExecutor.at(step.getType());
        return executor(step, previousStepResults);
    } catch (std::out_of_range& e) {
        throw CrawlerStepTypeError(step.getType());
    } catch (std::runtime_error& e) {
        throw CrawlerStepExecutionError(stepNumber, e);
    }
}

std::vector<std::string> CrawlerExecutor::executeCrawlerAndSaveLog(const Crawler &crawler,
                                                                   const std::string &crawlerType,
                                                                   const std::string &logSectionName,
                                                                   std::vector<CrawlLogEntry> &crawlLog,
                                                                   const std::vector<std::string>& result) const {
    auto execution = executeCrawler(crawler, crawlerType, result);
    crawlLog.reserve(execution.log.size() + crawlLog.capacity());
    crawlLog.emplace_back(logSectionName);
    for (const auto& entry: execution.log) {
        crawlLog.push_back(entry);
    }
    return execution.result;
}

}