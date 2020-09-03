#ifndef SERIO_REGEXPSTEPEXECUTOR_H
#define SERIO_REGEXPSTEPEXECUTOR_H

#include <regex>
#include "CrawlerStepExecutor.h"

namespace serio::core {

class RegExpStepExecutor : public CrawlerStepExecutor {
public:
    std::vector<std::string> execute(const CrawlerStep &step, const std::vector<std::string> &previousStepResults) const override;
private:
    void findAndAppendMatchesToResults(std::string input, const std::regex& regExp, std::vector<std::string>& results) const;
    void appendMatchToResults(const std::smatch& match, std::vector<std::string>& results) const;
    void appendGroupMatchToResults(const std::smatch& match, std::vector<std::string>& results) const;
    void appendCompleteMatchToResults(const std::smatch& match, std::vector<std::string>& results) const;
};

}

#endif //SERIO_REGEXPSTEPEXECUTOR_H
