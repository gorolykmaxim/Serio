#ifndef SERIO_TVSHOWCRAWLERRUNTIME_H
#define SERIO_TVSHOWCRAWLERRUNTIME_H

#include <tv-show-crawler-storage/TvShowCrawlerStorage.h>
#include <tv-show-storage/TvShowStorage.h>
#include <http-client/HttpClient.h>
#include <tv-show-crawler-runtime/model/CrawlResult.h>
#include "tv-show-crawler-runtime/model/TvShowCrawler.h"
#include "tv-show-crawler-runtime/model/CrawlerStepType.h"
#include "tv-show-crawler-runtime/executor/CrawlerExecutor.h"
#include "CrawlerSerializer.h"

namespace serio::core {

class TvShowCrawlerRuntime {
public:
    TvShowCrawlerRuntime(TvShowCrawlerStorage& crawlerStorage, TvShowStorage& tvShowStorage, HttpClient& httpClient,
                         unsigned int maxCrawlLogEntryDataSize = 500);
    virtual void crawlTvShowAndSaveCrawler(const TvShowCrawler &crawler);
    [[nodiscard]] virtual std::vector<CrawlerStepType> getCrawlerStepTypes() const;
    [[nodiscard]] virtual TvShowCrawler deserializeTvShowCrawler(const std::string &rawCrawler) const;
    virtual bool willOverrideExistingTvShow(const TvShowCrawler &crawler);
    [[nodiscard]] virtual CrawlResult executeCrawlerForResult(const Crawler& crawler);
private:
    TvShowCrawlerStorage& crawlerStorage;
    TvShowStorage& tvShowStorage;
    CrawlerExecutor crawlerExecutor;
    CrawlerSerializer serializer;
    std::vector<CrawlerStepType> crawlerStepTypes;
    void registerCrawlerStepType(const std::string& type, std::unique_ptr<CrawlerStepExecutor> executor, const std::string& description, const std::vector<std::string>& mandatoryProperties = {});
};

}

#endif //SERIO_TVSHOWCRAWLERRUNTIME_H
