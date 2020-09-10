#include "tv-show-crawler-runtime/TvShowCrawlerRuntime.h"
#include <tv-show-crawler-runtime/executor/ValueStepExecutor.h>
#include <tv-show-crawler-runtime/executor/TransformStepExecutor.h>
#include <tv-show-crawler-runtime/executor/FetchStepExecutor.h>
#include <tv-show-crawler-runtime/executor/RegExpStepExecutor.h>

serio::core::TvShowCrawlerRuntime::TvShowCrawlerRuntime(TvShowCrawlerStorage& crawlerStorage, TvShowStorage& tvShowStorage, HttpClient& httpClient)
    : crawlerStorage(crawlerStorage), tvShowStorage(tvShowStorage) {
    registerCrawlerStepType(
            "value",
            std::make_unique<ValueStepExecutor>(),
                    R"(Step with type "value" ignores the input data and returns an array with a single string, specified in the "value" property.)",
                    {"value"});
    registerCrawlerStepType(
            "transform",
            std::make_unique<TransformStepExecutor>(),
                    R"(Step with type "transform" will transform each string from the input array using the "template" property and return the results. Let's say that you have a following array of input strings: ['1', '2', '3']. A transform step with "template"="Episode %s" will turn that array into ['Episode 1', 'Episode 2', 'Episode 3'].)",
                    {"template"});
    registerCrawlerStepType(
            "fetch",
            std::make_unique<FetchStepExecutor>(httpClient),
                    R"(Step with type "fetch" treats every string from the input array as a link, fetches content of each link and returns an array of fetched contents.)");
    registerCrawlerStepType(
            "regExp",
            std::make_unique<RegExpStepExecutor>(),
                    "Step with type \"regExp\" applies the regular expression, specified in \"regExp\" property, to every string from the input array, and returns an array of all found strings across all the input strings. Regular expressions can return groups of results, if you specify \"(.*)\" as a part of an expression. Normally this step returns all matches from the group 0 but if the aforementioned construction is present in the expression - the group 0 will be ignored and only the other groups (that correspond to such expressions) will be returned.",
                    {"regExp"});
}

void serio::core::TvShowCrawlerRuntime::crawlTvShowAndSaveCrawler(const TvShowCrawler &crawler) {
    try {
        TvShow tvShow(crawler.getTvShowName(), crawlerExecutor.executeThumbnailCrawler(crawler.getCrawler(CrawlerType::thumbnailCrawler)));
        std::vector<Episode> episodes = crawlerExecutor.executeEpisodeCrawler(crawler.getCrawler(CrawlerType::episodeVideoCrawler), crawler.getCrawler(CrawlerType::episodeNameCrawler));
        tvShowStorage.saveTvShow(tvShow, episodes);
        crawlerStorage.saveTvShowCrawler(crawler.getTvShowName(), serializer.serialize(crawler));
    } catch (std::runtime_error& e) {
        throw TvShowCrawlerExecutionError(crawler.getTvShowName(), e);
    }
}

std::vector<serio::core::CrawlerStepType> serio::core::TvShowCrawlerRuntime::getCrawlerStepTypes() const {
    return crawlerStepTypes;
}

void serio::core::TvShowCrawlerRuntime::registerCrawlerStepType(const std::string &type,
                                                                std::unique_ptr<CrawlerStepExecutor> executor,
                                                                const std::string& description,
                                                                const std::vector<std::string>& mandatoryProperties) {
    crawlerExecutor.registerCrawlerStepExecutor(type, std::move(executor));
    crawlerStepTypes.emplace_back(type, description, mandatoryProperties);
}
