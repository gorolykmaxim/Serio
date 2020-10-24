#include "tv-show-crawler-runtime/executor/RegExpStep.h"
#include <regex>

namespace serio::core {

static void appendCompleteMatchToResults(const std::smatch &match, std::vector<std::string> &results) {
    results.push_back(match[0].str());
}

static void appendGroupMatchToResults(const std::smatch &match, std::vector<std::string> &results) {
    for (int i = 1; i < match.size(); i++) {
        results.push_back(match[i].str());
    }
}

static void appendMatchToResults(const std::smatch &match, std::vector<std::string> &results) {
    if (match.size() > 1) {
        appendGroupMatchToResults(match, results);
    } else {
        appendCompleteMatchToResults(match, results);
    }
}

static void findAndAppendMatchesToResults(std::string input, const std::regex& regExp, std::vector<std::string> &results) {
    std::smatch match;
    while (std::regex_search(input, match, regExp)) {
        appendMatchToResults(match, results);
        input = match.suffix();
    }
}

std::vector<std::string> executeRegExpStep(const CrawlerStep &step, const std::vector<std::string> &previousStepResults) {
    std::vector<std::string> results;
    results.reserve(previousStepResults.size());
    std::regex regExp(step.getPropertyOrFail("regExp"));
    for (const auto & previousResult: previousStepResults) {
        findAndAppendMatchesToResults(previousResult, regExp, results);
    }
    return results;
}

}