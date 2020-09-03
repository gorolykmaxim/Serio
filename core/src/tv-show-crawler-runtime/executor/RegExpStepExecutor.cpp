#include "tv-show-crawler-runtime/executor/RegExpStepExecutor.h"

std::vector<std::string> serio::core::RegExpStepExecutor::execute(const serio::core::CrawlerStep &step,
                                                                  const std::vector<std::string> &previousStepResults) const {
    std::vector<std::string> results;
    results.reserve(previousStepResults.size());
    std::regex regExp(step.getPropertyOrFail("regExp"));
    for (const std::string& previousResult: previousStepResults) {
        findAndAppendMatchesToResults(previousResult, regExp, results);
    }
    return results;
}


void serio::core::RegExpStepExecutor::findAndAppendMatchesToResults(std::string input, const std::regex& regExp, std::vector<std::string> &results) const {
    std::smatch match;
    while (std::regex_search(input, match, regExp)) {
        appendMatchToResults(match, results);
        input = match.suffix();
    }
}

void serio::core::RegExpStepExecutor::appendMatchToResults(const std::smatch &match, std::vector<std::string> &results) const {
    if (match.size() > 1) {
        appendGroupMatchToResults(match, results);
    } else {
        appendCompleteMatchToResults(match, results);
    }
}

void serio::core::RegExpStepExecutor::appendGroupMatchToResults(const std::smatch &match, std::vector<std::string> &results) const {
    for (int i = 1; i < match.size(); i++) {
        results.push_back(match[i].str());
    }
}

void serio::core::RegExpStepExecutor::appendCompleteMatchToResults(const std::smatch &match, std::vector<std::string> &results) const {
    results.push_back(match[0].str());
}
