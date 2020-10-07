#include "tv-show-crawler-runtime/TvShowCrawlerRuntime.h"
#include <tv-show-crawler-runtime/executor/ValueStep.h>
#include <tv-show-crawler-runtime/executor/TransformStep.h>
#include <tv-show-crawler-runtime/executor/FetchStep.h>
#include <tv-show-crawler-runtime/executor/RegExpStep.h>

serio::core::TvShowCrawlerRuntime::TvShowCrawlerRuntime(TvShowCrawlerStorage& crawlerStorage, TvShowStorage& tvShowStorage,
                                                        TvShowCrawlerLogStorage& logStorage, HttpClient& httpClient,
                                                        unsigned int maxCrawlLogEntryDataSize)
    : crawlerStorage(crawlerStorage),
      tvShowStorage(tvShowStorage),
      logStorage(logStorage),
      crawlerExecutor(maxCrawlLogEntryDataSize),
      serializer(crawlerStepTypes) {
    registerCrawlerStepType(
            "value",
            [] (const serio::core::CrawlerStep& step, const std::vector<std::string>& results) { return executeValueStep(step, results); },
            R"(Step with type "value" ignores the input data and returns an array with a single string, specified in the "value" property.)",
            {"value"});
    registerCrawlerStepType(
            "transform",
            [] (const serio::core::CrawlerStep& step, const std::vector<std::string>& results) { return executeTransformStep(step, results); },
            R"(Step with type "transform" will transform each string from the input array using the "template" property and return the results. Let's say that you have a following array of input strings: ['1', '2', '3']. A transform step with "template"="Episode %s" will turn that array into ['Episode 1', 'Episode 2', 'Episode 3'].)",
            {"template"});
    registerCrawlerStepType(
            "fetch",
            [&httpClient] (const serio::core::CrawlerStep& step, const std::vector<std::string>& results) { return executeFetchStep(step, results, httpClient); },
            R"(Step with type "fetch" treats every string from the input array as a link, fetches content of each link and returns an array of fetched contents.)");
    registerCrawlerStepType(
            "regExp",
            [] (const serio::core::CrawlerStep& step, const std::vector<std::string>& results) { return executeRegExpStep(step, results); },
            "Step with type \"regExp\" applies the regular expression, specified in \"regExp\" property, to every string from the input array, and returns an array of all found strings across all the input strings. Regular expressions can return groups of results, if you specify \"(.*)\" as a part of an expression. Normally this step returns all matches from the group 0 but if the aforementioned construction is present in the expression - the group 0 will be ignored and only the other groups (that correspond to such expressions) will be returned.",
            {"regExp"});
}

void serio::core::TvShowCrawlerRuntime::crawlTvShowAndSaveCrawler(const TvShowCrawler &crawler) {
    try {
        executeCrawler(crawler);
        crawlerStorage.saveTvShowCrawler(crawler.getTvShowName(), serializer.serialize(crawler));
    } catch (std::runtime_error& e) {
        throw TvShowCrawlerExecutionError(crawler.getTvShowName(), e);
    }
}

std::vector<serio::core::CrawlerStepType> serio::core::TvShowCrawlerRuntime::getCrawlerStepTypes() const {
    return crawlerStepTypes;
}

void serio::core::TvShowCrawlerRuntime::registerCrawlerStepType(const std::string &type,
                                                                CrawlerStepExecutor executor,
                                                                const std::string& description,
                                                                const std::vector<std::string>& mandatoryProperties) {
    crawlerExecutor.registerCrawlerStepExecutor(type, std::move(executor));
    crawlerStepTypes.emplace_back(type, description, mandatoryProperties);
}

serio::core::TvShowCrawler serio::core::TvShowCrawlerRuntime::deserializeTvShowCrawler(const std::string &rawCrawler) const {
    return serializer.deserialize(rawCrawler);
}

bool serio::core::TvShowCrawlerRuntime::willOverrideExistingTvShow(const serio::core::TvShowCrawler &crawler) {
    return tvShowStorage.getTvShowByName(crawler.getTvShowName()).has_value();
}

serio::core::CrawlResult serio::core::TvShowCrawlerRuntime::executeCrawler(const serio::core::Crawler &crawler) const {
    return crawlerExecutor.executeCrawler(crawler, "specified");
}

std::optional<serio::core::TvShowCrawler> serio::core::TvShowCrawlerRuntime::getTvShowCrawlerByTvShowName(const std::string &tvShowName) {
    auto rawTvShowCrawler = crawlerStorage.getTvShowCrawlerByTvShowName(tvShowName);
    if (rawTvShowCrawler) {
        return std::optional(serializer.deserialize(*rawTvShowCrawler));
    } else {
        return std::optional<serio::core::TvShowCrawler>();
    }
}

void serio::core::TvShowCrawlerRuntime::crawlTvShow(const std::string &tvShowName) {
    try {
        executeCrawler(getTvShowCrawlerByTvShowNameOrFail(tvShowName));
    } catch (std::runtime_error& e) {
        throw TvShowCrawlerExecutionError(tvShowName, e);
    }
}

void serio::core::TvShowCrawlerRuntime::executeCrawler(const serio::core::TvShowCrawler &crawler) {
    std::vector<serio::core::CrawlLogEntry> crawlLog;
    TvShow tvShow(crawler.getTvShowName(), crawlerExecutor.executeThumbnailCrawler(crawler.getCrawler(CrawlerType::thumbnailCrawler), crawlLog));
    auto episodes = crawlerExecutor.executeEpisodeCrawler(crawler.getCrawler(CrawlerType::episodeVideoCrawler), crawler.getCrawler(CrawlerType::episodeNameCrawler), crawlLog);
    tvShowStorage.saveTvShow(tvShow, episodes);
    logStorage.saveCrawlLog(tvShow.getName(), crawlLog);
}

serio::core::TvShowCrawler serio::core::TvShowCrawlerRuntime::getTvShowCrawlerByTvShowNameOrFail(const std::string &tvShowName) {
    auto crawler = getTvShowCrawlerByTvShowName(tvShowName);
    if (crawler) {
        return *crawler;
    } else {
        throw TvShowCrawlerDoesNotExistError(tvShowName);
    }
}

serio::core::TvShowCrawlerDoesNotExistError::TvShowCrawlerDoesNotExistError(const std::string &tvShowName)
    : logic_error("Crawler of tv show " + tvShowName + " does not exist") {}