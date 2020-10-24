#include "tv-show-crawler-runtime/executor/TransformStep.h"
#include <regex>

namespace serio::core {

std::vector<std::string> executeTransformStep(const CrawlerStep &step,
                                              const std::vector<std::string> &previousStepResults) {
    std::vector<std::string> results;
    results.reserve(previousStepResults.size());
    std::regex regex("%s");
    auto template_ = step.getPropertyOrFail("template");
    for (const auto& previousResult: previousStepResults) {
        results.push_back(std::regex_replace(template_, regex, previousResult));
    }
    return results;
}

}