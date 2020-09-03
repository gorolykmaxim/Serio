#ifndef SERIO_CRAWLEREXECUTOR_H
#define SERIO_CRAWLEREXECUTOR_H

#include <http-client/HttpClient.h>
#include <tv-show-storage/Episode.h>
#include "tv-show-crawler-runtime/model/Crawler.h"
#include "tv-show-crawler-runtime/model/CrawlerStep.h"
#include "CrawlerStepExecutor.h"

namespace serio::core {

class CrawlerExecutor {
public:
    void registerCrawlerStepExecutor(const std::string& type, std::unique_ptr<CrawlerStepExecutor> executor);
    [[nodiscard]] std::string executeThumbnailCrawler(const Crawler& crawler) const;
    [[nodiscard]] std::vector<Episode> executeEpisodeCrawler(const Crawler& videoCrawler, const Crawler& nameCrawler) const;
private:
    std::map<std::string, std::unique_ptr<CrawlerStepExecutor>> crawlerStepTypeToExecutor;
    [[nodiscard]] std::vector<std::string> crawlEpisodeVideos(const Crawler& crawler) const;
    [[nodiscard]] std::vector<std::string> crawlEpisodeNamesIfNecessary(const Crawler& crawler, const std::vector<std::string>& videoUrls) const;
    [[nodiscard]] std::vector<std::string> executeCrawler(const Crawler& crawler, const std::string& crawlerType, std::vector<std::string> result = {}) const;
    [[nodiscard]] std::vector<std::string> executeCrawlerStep(const std::vector<std::string>& previousStepResults, const CrawlerStep& step, unsigned int stepNumber) const;
};

}

#endif //SERIO_CRAWLEREXECUTOR_H
