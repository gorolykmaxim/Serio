#ifndef SERIO_TVSHOWCRAWLEREDITOR_H
#define SERIO_TVSHOWCRAWLEREDITOR_H

#include <tv-show-crawler-runtime/TvShowCrawlerRuntime.h>
#include "TvShowCrawlerBuilder.h"

namespace serio::core {

class TvShowCrawlerEditor {
public:
    explicit TvShowCrawlerEditor(TvShowCrawlerRuntime& runtime);
    virtual void createTvShowCrawler();
    virtual void importTvShowCrawler(const std::string &rawCrawler);
    virtual void editTvShowCrawler(const std::string& tvShowName);
    virtual void setTvShowName(std::string name);
    virtual void editCrawler(CrawlerType type);
    virtual void addCrawlerStep(CrawlerStep step);
    virtual void replaceCrawlerStep(unsigned int stepIndex, CrawlerStep newStep);
    virtual void removeCrawlerStep(unsigned int stepIndex);
    virtual void saveCrawler();
    virtual void saveAndRunTvShowCrawler();
    virtual CrawlResult previewCrawler();
    virtual bool willOverrideExistingTvShow();
    [[nodiscard]] virtual std::string getTvShowName() const;
    [[nodiscard]] virtual std::vector<CrawlerStep> getCrawlerSteps() const;
    [[nodiscard]] virtual std::vector<CrawlerStepType> getCrawlerStepTypes() const;
    [[nodiscard]] virtual std::vector<std::string> getPreviewedCrawlerResults() const;
    [[nodiscard]] virtual std::vector<CrawlLogEntry> getPreviewedCrawlerLog() const;
private:
    TvShowCrawlerRuntime& runtime;
    std::optional<TvShowCrawlerBuilder> builder;
    void assertTvShowCrawlerIsEdited() const;
    TvShowCrawlerBuilder& getBuilderOrFail();
    [[nodiscard]] const TvShowCrawlerBuilder& getBuilderOrFail() const;
    void addCrawlerStepsFrom(CrawlerType type, const TvShowCrawler &crawler);
    void setTvShowCrawler(const TvShowCrawler& crawler);
};

class NoTvShowCrawlerEditedError : public std::logic_error {
public:
    NoTvShowCrawlerEditedError();
};

class TvShowCrawlerEditorError : public std::runtime_error {
public:
    explicit TvShowCrawlerEditorError(const std::runtime_error& cause);
};

}

#endif //SERIO_TVSHOWCRAWLEREDITOR_H
