#ifndef SERIO_REGEXPSYSTEM_H
#define SERIO_REGEXPSYSTEM_H

#include <string>
#include <regex>
#include <crawler-runtime/CrawlerRuntime.h>
#include "CrawlerExecution.h"

namespace serio {
class RegExpSystem {
public:
    RegExpSystem(std::vector<Crawler>& crawlers, std::vector<CrawlerExecution>& executions);
    void update();
private:
    std::vector<CrawlerExecution>& executions;

    [[nodiscard]] nlohmann::json search(const nlohmann::json& searchCriteria) const;
    void appendMatchesToResults(std::string string, const std::regex& regex, nlohmann::json& results) const;
    void appendCompleteMatchToResults(const std::smatch& match, nlohmann::json& results) const;
    void appendGroupMatchToResults(const std::smatch& match, nlohmann::json& results) const;
    [[nodiscard]] nlohmann::json readRegExpBuffer(CrawlerExecution& execution) const;
    void writeRegExpBuffer(CrawlerExecution& execution, const nlohmann::json& searchResults) const;
};

class InvalidRegExpError : public std::runtime_error {
public:
    InvalidRegExpError();
};

class InvalidRegExpInputDataError : public std::runtime_error {
public:
    InvalidRegExpInputDataError();
};
}

#endif //SERIO_REGEXPSYSTEM_H
