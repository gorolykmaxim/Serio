#include <utility>
#include "tv-show-crawler-editor/CrawlerBuilder.h"

namespace serio::core {

CrawlerBuilder::CrawlerBuilder(CrawlerType editedCrawlerType, std::vector<CrawlerStep> editedCrawlerSteps)
        : editedCrawlerType(editedCrawlerType), editedCrawlerSteps(std::move(editedCrawlerSteps)) {}

void CrawlerBuilder::addCrawlerStep(CrawlerStep step) {
    editedCrawlerSteps.push_back(std::move(step));
}

void CrawlerBuilder::replaceCrawlerStep(unsigned int stepIndex, CrawlerStep newStep) {
    assertCrawlerStepExist(stepIndex);
    editedCrawlerSteps[stepIndex] = std::move(newStep);
}

void CrawlerBuilder::removeCrawlerStep(unsigned int stepIndex) {
    assertCrawlerStepExist(stepIndex);
    editedCrawlerSteps.erase(editedCrawlerSteps.begin() + stepIndex);
}

void CrawlerBuilder::setPreviewResult(CrawlResult result) {
    previewResult = std::move(result);
}

CrawlerType CrawlerBuilder::getEditedCrawlerType() const {
    return editedCrawlerType;
}

std::vector<CrawlerStep> CrawlerBuilder::getCrawlerSteps() const {
    return editedCrawlerSteps;
}

std::vector<std::string> CrawlerBuilder::getPreviewedCrawlerResults() const {
    assertCrawlerHasBeenPreviewed();
    return previewResult->result;
}

std::vector<CrawlLogEntry> CrawlerBuilder::getPreviewedCrawlerLog() const {
    assertCrawlerHasBeenPreviewed();
    return previewResult->log;
}

void CrawlerBuilder::assertCrawlerHasBeenPreviewed() const {
    if (!previewResult) {
        throw CrawlerNotPreviewedError();
    }
}

void CrawlerBuilder::assertCrawlerStepExist(unsigned int stepIndex) const {
    if (stepIndex >= editedCrawlerSteps.size()) {
        throw CrawlerStepDoesNotExist(stepIndex);
    }
}

CrawlerStepDoesNotExist::CrawlerStepDoesNotExist(unsigned int stepIndex)
    : std::out_of_range("Edited crawler does not have a step with index " + std::to_string(stepIndex)) {}

CrawlerNotPreviewedError::CrawlerNotPreviewedError()
    : std::logic_error("Edited crawler has not been previewed yet") {}

}