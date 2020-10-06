#ifndef SERIO_CRAWLEREXECUTOR_H
#define SERIO_CRAWLEREXECUTOR_H

#include <http-client/HttpClient.h>
#include <tv-show-storage/Episode.h>
#include <tv-show-crawler-runtime/model/CrawlResult.h>
#include "tv-show-crawler-runtime/model/Crawler.h"
#include "tv-show-crawler-runtime/model/CrawlerStep.h"

namespace serio::core {

typedef std::function<std::vector<std::string>(const CrawlerStep&, const std::vector<std::string>&)> CrawlerStepExecutor;

class CrawlerExecutor {
public:
    explicit CrawlerExecutor(unsigned int maxLogEntryDataSize);
    void registerCrawlerStepExecutor(const std::string& type, CrawlerStepExecutor executor);
    [[nodiscard]] std::string executeThumbnailCrawler(const Crawler& crawler, std::vector<CrawlLogEntry>& crawlLog) const;
    [[nodiscard]] std::vector<Episode> executeEpisodeCrawler(const Crawler& videoCrawler, const Crawler& nameCrawler, std::vector<CrawlLogEntry>& crawlLog) const;
    [[nodiscard]] CrawlResult executeCrawler(const Crawler& crawler, const std::string& crawlerType, std::vector<std::string> result = {}) const;
private:
    const unsigned int maxLogEntryDataSize;
    std::map<std::string, CrawlerStepExecutor> crawlerStepTypeToExecutor;
    [[nodiscard]] std::vector<std::string> crawlEpisodeVideos(const Crawler& crawler, std::vector<CrawlLogEntry>& crawlLog) const;
    [[nodiscard]] std::vector<std::string> crawlEpisodeNamesIfNecessary(const Crawler& crawler, const std::vector<std::string>& videoUrls, std::vector<CrawlLogEntry>& crawlLog) const;
    [[nodiscard]] std::vector<std::string> executeCrawlerStep(const std::vector<std::string>& previousStepResults, const CrawlerStep& step, unsigned int stepNumber) const;
    [[nodiscard]] std::vector<std::string> executeCrawlerAndSaveLog(const Crawler& crawler,
                                                                    const std::string& crawlerType,
                                                                    const std::string& logSectionName,
                                                                    std::vector<CrawlLogEntry>& crawlLog,
                                                                    const std::vector<std::string>& result = {}) const;
};

}

#endif //SERIO_CRAWLEREXECUTOR_H
