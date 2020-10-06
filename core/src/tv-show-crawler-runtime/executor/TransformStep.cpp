#include "tv-show-crawler-runtime/executor/TransformStep.h"
#include <regex>

std::vector<std::string> serio::core::executeTransformStep(const serio::core::CrawlerStep &step,
                                                           const std::vector<std::string> &previousStepResults) {
    std::vector<std::string> results;
    results.reserve(previousStepResults.size());
    std::regex regex("%s");
    std::string template_ = step.getPropertyOrFail("template");
    for (const std::string& previousResult: previousStepResults) {
        results.push_back(std::regex_replace(template_, regex, previousResult));
    }
    return results;
}
