#include "tv-show-crawler-runtime/executor/ValueStep.h"

namespace serio::core {

std::vector<std::string> executeValueStep(const CrawlerStep &step, const std::vector<std::string> &previousStepResults) {
    return {step.getPropertyOrFail("value")};
}

}