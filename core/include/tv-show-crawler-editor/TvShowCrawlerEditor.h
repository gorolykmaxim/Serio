#ifndef SERIO_TVSHOWCRAWLEREDITOR_H
#define SERIO_TVSHOWCRAWLEREDITOR_H

#include <tv-show-crawler-runtime/TvShowCrawlerRuntime.h>
#include "TvShowCrawlerBuilder.h"

namespace serio::core {

class TvShowCrawlerEditor {
public:
    explicit TvShowCrawlerEditor(TvShowCrawlerRuntime& runtime);
    void createTvShowCrawler();
    void setTvShowName(std::string name);
    void editCrawler(CrawlerType type);
    void addCrawlerStep(CrawlerStep step);
    void replaceCrawlerStep(unsigned int stepIndex, CrawlerStep newStep);
    void removeCrawlerStep(unsigned int stepIndex);
    void saveCrawler();
    void saveAndRunTvShowCrawler();
    bool willOverrideExistingTvShow();
    [[nodiscard]] std::string getTvShowName() const;
    [[nodiscard]] std::vector<CrawlerStep> getCrawlerSteps() const;
    [[nodiscard]] std::vector<CrawlerStepType> getCrawlerStepTypes() const;
private:
    TvShowCrawlerRuntime& runtime;
    std::optional<TvShowCrawlerBuilder> builder;
    void assertTvShowCrawlerIsEdited() const;
    TvShowCrawlerBuilder& getBuilderOrFail();
    [[nodiscard]] const TvShowCrawlerBuilder& getBuilderOrFail() const;
};

class NoTvShowCrawlerEditedError : public std::logic_error {
public:
    NoTvShowCrawlerEditedError();
};

}

#endif //SERIO_TVSHOWCRAWLEREDITOR_H
