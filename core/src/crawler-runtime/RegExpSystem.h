#ifndef SERIO_REGEXPSYSTEM_H
#define SERIO_REGEXPSYSTEM_H

#include <string>
#include <regex>
#include <crawler-runtime/CrawlerRuntime.h>
#include "CrawlerExecution.h"

namespace serio {
struct RegExpSearchTask {
    std::regex regex;
    std::vector<std::string> stringsToSearch;
};

class RegExpSystem {
public:
    RegExpSystem(std::vector<Crawler>& crawlers, std::vector<CrawlerExecution>& executions);
    void update();
private:
    std::vector<CrawlerExecution>& executions;

    [[nodiscard]] std::vector<std::string> search(const RegExpSearchTask& task) const;
    void appendMatchesToResults(const std::string& string, const std::regex& regex, std::vector<std::string>& results) const;
    void appendCompleteMatchToResults(const std::smatch& match, std::vector<std::string>& results) const;
    void appendGroupMatchToResults(const std::smatch& match, std::vector<std::string>& results) const;
    [[nodiscard]] RegExpSearchTask getCurrentSearchTask(CrawlerExecution& execution) const;
    void setCurrentSearchResults(CrawlerExecution& execution, const std::vector<std::string>& results) const;
};
}

#endif //SERIO_REGEXPSYSTEM_H
