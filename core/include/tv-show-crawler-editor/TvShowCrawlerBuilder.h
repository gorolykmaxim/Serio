#ifndef SERIO_TVSHOWCRAWLERBUILDER_H
#define SERIO_TVSHOWCRAWLERBUILDER_H

#include <string>
#include <map>
#include <tv-show-crawler-runtime/model/TvShowCrawler.h>

namespace serio::core {

class TvShowCrawlerBuilder {
public:
    TvShowCrawlerBuilder();
    void setTvShowName(std::string name);
    void addStepToCrawler(CrawlerType type, CrawlerStep step);
    void replaceStepInCrawler(CrawlerType type, unsigned int stepIndex, CrawlerStep step);
    void removeStepFromCrawler(CrawlerType type, unsigned int stepIndex);
    [[nodiscard]] TvShowCrawler build() const;
    [[nodiscard]] std::string getTvShowName() const;
    [[nodiscard]] std::vector<CrawlerStep> getStepsOfCrawler(CrawlerType type) const;
private:
    std::string tvShowName;
    std::map<CrawlerType, std::vector<CrawlerStep>> crawlerTypeToSteps;
    void assertStepWithIndexExists(CrawlerType type, unsigned int stepIndex) const;
};

class TvShowNameNotSpecifiedError : public std::logic_error {
public:
    TvShowNameNotSpecifiedError();
};

class CrawlerStepDoesNotExistError : public std::out_of_range {
public:
    explicit CrawlerStepDoesNotExistError(unsigned int stepIndex);
};

}

#endif //SERIO_TVSHOWCRAWLERBUILDER_H
