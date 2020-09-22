#include <utility>
#include "tv-show-crawler-editor/CrawlerBuilder.h"

serio::core::CrawlerBuilder::CrawlerBuilder(serio::core::CrawlerType editedCrawlerType,
                                            std::vector<CrawlerStep> editedCrawlerSteps)
    : editedCrawlerType(editedCrawlerType), editedCrawlerSteps(std::move(editedCrawlerSteps)) {}

void serio::core::CrawlerBuilder::addCrawlerStep(serio::core::CrawlerStep step) {
    editedCrawlerSteps.push_back(std::move(step));
}

void serio::core::CrawlerBuilder::replaceCrawlerStep(unsigned int stepIndex, serio::core::CrawlerStep newStep) {
    assertCrawlerStepExist(stepIndex);
    editedCrawlerSteps[stepIndex] = std::move(newStep);
}

void serio::core::CrawlerBuilder::removeCrawlerStep(unsigned int stepIndex) {
    assertCrawlerStepExist(stepIndex);
    editedCrawlerSteps.erase(editedCrawlerSteps.begin() + stepIndex);
}

void serio::core::CrawlerBuilder::setPreviewResult(CrawlResult result) {
    previewResult = std::move(result);
}

serio::core::CrawlerType serio::core::CrawlerBuilder::getEditedCrawlerType() const {
    return editedCrawlerType;
}

std::vector<serio::core::CrawlerStep> serio::core::CrawlerBuilder::getCrawlerSteps() const {
    return editedCrawlerSteps;
}

std::vector<std::string> serio::core::CrawlerBuilder::getPreviewedCrawlerResults() const {
    assertCrawlerHasBeenPreviewed();
    return previewResult->result;
}

std::vector<serio::core::CrawlLogEntry> serio::core::CrawlerBuilder::getPreviewedCrawlerLog() const {
    assertCrawlerHasBeenPreviewed();
    return previewResult->log;
}

void serio::core::CrawlerBuilder::assertCrawlerHasBeenPreviewed() const {
    if (!previewResult) {
        throw CrawlerNotPreviewedError();
    }
}

void serio::core::CrawlerBuilder::assertCrawlerStepExist(unsigned int stepIndex) const {
    if (stepIndex >= editedCrawlerSteps.size()) {
        throw CrawlerStepDoesNotExist(stepIndex);
    }
}

serio::core::CrawlerStepDoesNotExist::CrawlerStepDoesNotExist(unsigned int stepIndex)
    : std::out_of_range("Edited crawler does not have a step with index " + std::to_string(stepIndex)) {}

serio::core::CrawlerNotPreviewedError::CrawlerNotPreviewedError()
    : std::logic_error("Edited crawler has not been previewed yet") {}
