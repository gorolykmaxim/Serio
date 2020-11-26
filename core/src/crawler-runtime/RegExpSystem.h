#ifndef SERIO_REGEXPSYSTEM_H
#define SERIO_REGEXPSYSTEM_H

#include <string>
#include <regex>
#include "CrawlerExecution.h"

namespace serio {
class RegExpSystem {
public:
    [[nodiscard]] std::string getCode() const;
    void update(CrawlerExecution& execution) const;
private:
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
