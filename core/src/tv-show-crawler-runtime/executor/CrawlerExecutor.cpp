#include <tv-show-crawler-runtime/model/TvShowCrawler.h>
#include "tv-show-crawler-runtime/executor/CrawlerExecutor.h"

serio::core::CrawlerExecutor::CrawlerExecutor(unsigned int maxLogEntryDataSize) : maxLogEntryDataSize(maxLogEntryDataSize) {}

void serio::core::CrawlerExecutor::registerCrawlerStepExecutor(const std::string& type, CrawlerStepExecutor executor) {
    crawlerStepTypeToExecutor[type] = std::move(executor);
}

std::string serio::core::CrawlerExecutor::executeThumbnailCrawler(const serio::core::Crawler &crawler, std::vector<serio::core::CrawlLogEntry>& crawlLog) const {
    std::vector<std::string> results = executeCrawlerAndSaveLog(crawler, "thumbnail", "Crawling thumbnail", crawlLog);
    return results.empty() ? "" : results[0];
}

std::vector<serio::core::Episode> serio::core::CrawlerExecutor::executeEpisodeCrawler(const Crawler& videoCrawler, const Crawler& nameCrawler, std::vector<serio::core::CrawlLogEntry>& crawlLog) const {
    std::vector<std::string> videoUrls = crawlEpisodeVideos(videoCrawler, crawlLog);
    std::vector<std::string> names = crawlEpisodeNamesIfNecessary(nameCrawler, videoUrls, crawlLog);
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

std::vector<std::string> serio::core::CrawlerExecutor::crawlEpisodeVideos(const serio::core::Crawler &crawler, std::vector<CrawlLogEntry>& crawlLog) const {
    return executeCrawlerAndSaveLog(crawler, "episode video", "Crawling episode videos", crawlLog);
}

std::vector<std::string> serio::core::CrawlerExecutor::crawlEpisodeNamesIfNecessary(const serio::core::Crawler &crawler,
                                                                                    const std::vector<std::string>& videoUrls,
                                                                                    std::vector<CrawlLogEntry>& crawlLog) const {
    if (crawler.hasSteps()) {
        return executeCrawlerAndSaveLog(crawler, "episode name", "Crawling episode names", crawlLog, videoUrls);
    } else {
        return std::vector<std::string>();
    }
}

serio::core::CrawlResult serio::core::CrawlerExecutor::executeCrawler(const serio::core::Crawler &crawler, const std::string& crawlerType, std::vector<std::string> result) const {
    try {
        std::vector<CrawlerStep> steps = crawler.getSteps();
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

std::vector<std::string> serio::core::CrawlerExecutor::executeCrawlerStep(const std::vector<std::string>& previousStepResults,
                                                                          const serio::core::CrawlerStep &step,
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

std::vector<std::string> serio::core::CrawlerExecutor::executeCrawlerAndSaveLog(const serio::core::Crawler &crawler,
                                                                                const std::string &crawlerType,
                                                                                const std::string &logSectionName,
                                                                                std::vector<CrawlLogEntry> &crawlLog,
                                                                                const std::vector<std::string>& result) const {
    serio::core::CrawlResult execution = executeCrawler(crawler, crawlerType, result);
    crawlLog.reserve(execution.log.size() + crawlLog.capacity());
    crawlLog.emplace_back(logSectionName);
    for (const auto& entry: execution.log) {
        crawlLog.push_back(entry);
    }
    return execution.result;
}
