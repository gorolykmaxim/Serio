#include "tv-show-crawler-runtime/TvShowCrawlerRuntime.h"
#include <tv-show-crawler-runtime/executor/ValueStepExecutor.h>
#include <tv-show-crawler-runtime/executor/TransformStepExecutor.h>
#include <tv-show-crawler-runtime/executor/FetchStepExecutor.h>
#include <tv-show-crawler-runtime/executor/RegExpStepExecutor.h>

serio::core::TvShowCrawlerRuntime::TvShowCrawlerRuntime(TvShowCrawlerStorage& crawlerStorage, TvShowStorage& tvShowStorage, HttpClient& httpClient)
    : crawlerStorage(crawlerStorage), tvShowStorage(tvShowStorage) {
    registerCrawlerStepType("value", std::make_unique<ValueStepExecutor>(), {"value"});
    registerCrawlerStepType("transform", std::make_unique<TransformStepExecutor>(), {"template"});
    registerCrawlerStepType("fetch", std::make_unique<FetchStepExecutor>(httpClient));
    registerCrawlerStepType("regExp", std::make_unique<RegExpStepExecutor>(), {"regExp"});
}

void serio::core::TvShowCrawlerRuntime::crawlTvShowAndSaveCrawler(const TvShowCrawler &crawler) {
    try {
        TvShow tvShow(crawler.getTvShowName(), crawlerExecutor.executeThumbnailCrawler(crawler.getCrawler(CrawlerType::thumbnailCrawler)));
        std::vector<Episode> episodes = crawlerExecutor.executeEpisodeCrawler(crawler.getCrawler(CrawlerType::episodeVideoCrawler), crawler.getCrawler(CrawlerType::episodeNameCrawler));
        tvShowStorage.saveTvShow(tvShow, episodes);
        crawlerStorage.saveTvShowCrawler(crawler.getTvShowName(), serializer.serialize(crawler));
    } catch (std::exception& e) {
        throw TvShowCrawlerExecutionError(crawler.getTvShowName(), e);
    }
}

std::vector<serio::core::CrawlerStepType> serio::core::TvShowCrawlerRuntime::getCrawlerStepTypes() const {
    return crawlerStepTypes;
}

void serio::core::TvShowCrawlerRuntime::registerCrawlerStepType(const std::string &type,
                                                                std::unique_ptr<CrawlerStepExecutor> executor,
                                                                const std::vector<std::string>& mandatoryProperties) {
    crawlerExecutor.registerCrawlerStepExecutor(type, std::move(executor));
    crawlerStepTypes.emplace_back(type, mandatoryProperties);
}
