#ifndef SERIO_TVSHOWCRAWLEREDITOR_H
#define SERIO_TVSHOWCRAWLEREDITOR_H

#include <tv-show-crawler-runtime/TvShowCrawlerRuntime.h>
#include "TvShowCrawlerBuilder.h"

namespace serio::core {

class TvShowCrawlerEditor {
public:
    explicit TvShowCrawlerEditor(TvShowCrawlerRuntime& runtime);
    void createTvShowCrawler();
    void setTvShowName(const std::string& name);
    void saveAndRunCrawler();
    void addStepToCrawler(CrawlerType type, const CrawlerStep& step);
    void replaceStepInCrawler(CrawlerType type, unsigned int stepIndex, const CrawlerStep& step);
    void removeStepFromCrawler(CrawlerType type, unsigned int stepIndex);
    [[nodiscard]] std::string getTvShowName() const;
    [[nodiscard]] std::vector<CrawlerStep> getStepsOfCrawler(CrawlerType type) const;
    [[nodiscard]] std::vector<CrawlerStepType> getCrawlerStepTypes() const;
private:
    std::optional<TvShowCrawlerBuilder> builder;
    TvShowCrawlerRuntime& runtime;
    void assertBuilderWasCreated() const;
};

class CrawlerEditorError : public std::runtime_error {
public:
    CrawlerEditorError(const std::exception& cause);
};

class NoCrawlerEditedError : public std::logic_error {
public:
    NoCrawlerEditedError();
};

}

#endif //SERIO_TVSHOWCRAWLEREDITOR_H
