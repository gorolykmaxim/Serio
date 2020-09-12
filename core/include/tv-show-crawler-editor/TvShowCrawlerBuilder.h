#ifndef SERIO_TVSHOWCRAWLERBUILDER_H
#define SERIO_TVSHOWCRAWLERBUILDER_H

#include <optional>
#include <string>
#include <tv-show-crawler-runtime/model/TvShowCrawler.h>

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
    [[nodiscard]] TvShowCrawler buildTvShowCrawler() const;
    [[nodiscard]] std::string getTvShowName() const;
    [[nodiscard]] std::vector<CrawlerStep> getCrawlerSteps() const;
private:
    std::string tvShowName;
    std::optional<CrawlerType> editedCrawlerType;
    std::map<CrawlerType, std::vector<CrawlerStep>> crawlerTypeToSteps;
    std::vector<CrawlerStep> editedCrawlerSteps;
    void assertCrawlerIsEdited() const;
    void assertCrawlerStepExist(unsigned int stepIndex) const;
    void assertTvShowIsSpecified() const;
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

#endif //SERIO_TVSHOWCRAWLERBUILDER_H
