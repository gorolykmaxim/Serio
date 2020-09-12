#ifndef SERIO_TVSHOWCRAWLEREDITOR_H
#define SERIO_TVSHOWCRAWLEREDITOR_H

#include <tv-show-crawler-runtime/TvShowCrawlerRuntime.h>

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
    [[nodiscard]] std::string getTvShowName() const;
    [[nodiscard]] std::vector<CrawlerStep> getCrawlerSteps() const;
    [[nodiscard]] std::vector<CrawlerStepType> getCrawlerStepTypes() const;
private:
    TvShowCrawlerRuntime& runtime;
    std::optional<std::string> tvShowName;
    std::optional<CrawlerType> editedCrawlerType;
    std::map<CrawlerType, std::vector<CrawlerStep>> crawlerTypeToSteps;
    std::vector<CrawlerStep> editedCrawlerSteps;
    void assertTvShowCrawlerIsEdited() const;
    void assertCrawlerIsEdited() const;
    void assertCrawlerStepExist(unsigned int stepIndex) const;
    void assertTvShowIsSpecified() const;
};

class NoTvShowCrawlerEditedError : public std::logic_error {
public:
    NoTvShowCrawlerEditedError();
};

class NoCrawlerEditedError : public std::logic_error {
public:
    NoCrawlerEditedError();
};

class CrawlerStepDoesNotExist : public std::out_of_range {
public:
    CrawlerStepDoesNotExist(unsigned int stepIndex);
};

class TvShowNameNotSpecifiedError : public std::logic_error {
public:
    TvShowNameNotSpecifiedError();
};

class TvShowCrawlerEditorError : public std::runtime_error {
public:
    TvShowCrawlerEditorError(const std::runtime_error& cause);
};

}

#endif //SERIO_TVSHOWCRAWLEREDITOR_H
