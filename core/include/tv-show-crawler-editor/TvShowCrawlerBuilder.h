#ifndef SERIO_TVSHOWCRAWLERBUILDER_H
#define SERIO_TVSHOWCRAWLERBUILDER_H

#include <optional>
#include <string>
#include <tv-show-crawler-runtime/model/TvShowCrawler.h>
#include <tv-show-crawler-runtime/model/CrawlResult.h>
#include "CrawlerBuilder.h"

namespace serio::core {

class TvShowCrawlerBuilder {
public:
    TvShowCrawlerBuilder();
    void setTvShowName(std::string name);
    void editCrawler(CrawlerType type);
    void addCrawlerStep(CrawlerStep step);
    void replaceCrawlerStep(unsigned int stepIndex, CrawlerStep newStep);
    void removeCrawlerStep(unsigned int stepIndex);
    void saveCrawler();
    void setPreviewResult(const CrawlResult& result);
    [[nodiscard]] TvShowCrawler buildTvShowCrawler() const;
    [[nodiscard]] std::string getTvShowName() const;
    [[nodiscard]] std::vector<CrawlerStep> getCrawlerSteps() const;
    [[nodiscard]] std::vector<std::string> getPreviewedCrawlerResults() const;
    [[nodiscard]] std::vector<CrawlLogEntry> getPreviewedCrawlerLog() const;
private:
    std::optional<CrawlerBuilder> builder;
    std::string tvShowName;
    std::map<CrawlerType, std::vector<CrawlerStep>> crawlerTypeToSteps;
    void assertCrawlerIsEdited() const;
    [[nodiscard]] CrawlerBuilder& getBuilderOrFail();
    [[nodiscard]] const CrawlerBuilder& getBuilderOrFail() const;
    void assertTvShowIsSpecified() const;
};

class NoCrawlerEditedError : public std::logic_error {
public:
    NoCrawlerEditedError();
};

class TvShowNameNotSpecifiedError : public std::logic_error {
public:
    TvShowNameNotSpecifiedError();
};

}

#endif //SERIO_TVSHOWCRAWLERBUILDER_H
