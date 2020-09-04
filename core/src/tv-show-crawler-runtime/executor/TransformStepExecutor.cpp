#include "tv-show-crawler-runtime/executor/TransformStepExecutor.h"
#include <regex>

std::vector<std::string> serio::core::TransformStepExecutor::execute(const serio::core::CrawlerStep &step,
                                                                     const std::vector<std::string>& previousStepResults) const {
    std::vector<std::string> results;
    results.reserve(previousStepResults.size());
    std::regex regex("%s");
    for (const std::string& previousResult: previousStepResults) {
        results.push_back(std::regex_replace(step.getPropertyOrFail("template"), regex, previousResult));
    }
    return results;
}
