#ifndef SERIO_CRAWLERBUILDER_H
#define SERIO_CRAWLERBUILDER_H

#include <tv-show-crawler-runtime/model/CrawlerStep.h>
#include <tv-show-crawler-runtime/model/TvShowCrawler.h>
#include <tv-show-crawler-runtime/model/CrawlResult.h>

namespace serio::core {

class CrawlerBuilder {
public:
    CrawlerBuilder(CrawlerType editedCrawlerType, std::vector<CrawlerStep> editedCrawlerSteps);
    void addCrawlerStep(CrawlerStep step);
    void replaceCrawlerStep(unsigned int stepIndex, CrawlerStep newStep);
    void removeCrawlerStep(unsigned int stepIndex);
    void setPreviewResult(CrawlResult result);
    [[nodiscard]] std::vector<CrawlerStep> getCrawlerSteps() const;
    [[nodiscard]] CrawlerType getEditedCrawlerType() const;
    [[nodiscard]] std::vector<std::string> getPreviewedCrawlerResults() const;
    [[nodiscard]] std::vector<CrawlLogEntry> getPreviewedCrawlerLog() const;
private:
    CrawlerType editedCrawlerType;
    std::vector<CrawlerStep> editedCrawlerSteps;
    std::optional<CrawlResult> previewResult;
    void assertCrawlerStepExist(unsigned int stepIndex) const;
    void assertCrawlerHasBeenPreviewed() const;
};

class CrawlerStepDoesNotExist : public std::out_of_range {
public:
    explicit CrawlerStepDoesNotExist(unsigned int stepIndex);
};

class CrawlerNotPreviewedError : public std::logic_error {
public:
    CrawlerNotPreviewedError();
};

}

#endif //SERIO_CRAWLERBUILDER_H
