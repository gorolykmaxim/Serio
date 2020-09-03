#include "tv-show-crawler-runtime/executor/ValueStepExecutor.h"

std::vector<std::string> serio::core::ValueStepExecutor::execute(const serio::core::CrawlerStep &step, const std::vector<std::string>& previousStepResults) const {
    return {step.getPropertyOrFail("value")};
}
