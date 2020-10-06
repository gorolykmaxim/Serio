#include "tv-show-crawler-runtime/executor/ValueStep.h"

std::vector<std::string> serio::core::executeValueStep(const serio::core::CrawlerStep &step,
                                                       const std::vector<std::string> &previousStepResults) {
    return {step.getPropertyOrFail("value")};
}
