#ifndef SERIO_TVSHOWCRAWLERRUNTIME_H
#define SERIO_TVSHOWCRAWLERRUNTIME_H

#include <tv-show-crawler-storage/TvShowCrawlerStorage.h>
#include <tv-show-storage/TvShowStorage.h>
#include <http-client/HttpClient.h>
#include "tv-show-crawler-runtime/model/TvShowCrawler.h"
#include "tv-show-crawler-runtime/model/CrawlerStepType.h"
#include "tv-show-crawler-runtime/executor/CrawlerExecutor.h"
#include "CrawlerSerializer.h"

namespace serio::core {

class TvShowCrawlerRuntime {
public:
    TvShowCrawlerRuntime(TvShowCrawlerStorage& crawlerStorage, TvShowStorage& tvShowStorage, HttpClient& httpClient);
    void crawlTvShowAndSaveCrawler(const TvShowCrawler &crawler);
    [[nodiscard]] std::vector<CrawlerStepType> getCrawlerStepTypes() const;
private:
    TvShowCrawlerStorage& crawlerStorage;
    TvShowStorage& tvShowStorage;
    CrawlerExecutor crawlerExecutor;
    CrawlerSerializer serializer;
    std::vector<CrawlerStepType> crawlerStepTypes;
    void registerCrawlerStepType(const std::string& type, std::unique_ptr<CrawlerStepExecutor> executor, const std::vector<std::string>& mandatoryProperties = {});
};

}

#endif //SERIO_TVSHOWCRAWLERRUNTIME_H
