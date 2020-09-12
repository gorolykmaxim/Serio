#ifndef SERIO_CRAWLERBUILDER_H
#define SERIO_CRAWLERBUILDER_H

#include <tv-show-crawler-runtime/model/CrawlerStep.h>
#include <tv-show-crawler-runtime/model/TvShowCrawler.h>

namespace serio::core {

class CrawlerBuilder {
public:
    CrawlerBuilder(CrawlerType editedCrawlerType, std::vector<CrawlerStep> editedCrawlerSteps);
    void addCrawlerStep(CrawlerStep step);
    void replaceCrawlerStep(unsigned int stepIndex, CrawlerStep newStep);
    void removeCrawlerStep(unsigned int stepIndex);
    [[nodiscard]] std::vector<CrawlerStep> getCrawlerSteps() const;
    [[nodiscard]] CrawlerType getEditedCrawlerType() const;
private:
    CrawlerType editedCrawlerType;
    std::vector<CrawlerStep> editedCrawlerSteps;
    void assertCrawlerStepExist(unsigned int stepIndex) const;
};

class CrawlerStepDoesNotExist : public std::out_of_range {
public:
    explicit CrawlerStepDoesNotExist(unsigned int stepIndex);
};

}

#endif //SERIO_CRAWLERBUILDER_H
