#include "RegExpSystem.h"

namespace serio {
RegExpSystem::RegExpSystem(std::vector<Crawler> &crawlers, std::vector<CrawlerExecution> &executions) : executions(executions) {
    for (auto& crawler: crawlers) {
        crawler.code = "function regExp(exp, values) {"
                       "  if (typeof exp !== 'string') die('Regular expression must be a string');"
                       "  for (let i = 0; i < values.length; i++) {"
                       "    if (typeof values[i] !== 'string') die('Regular expression data should be an array of strings');"
                       "  }"
                       "  _buffer=[exp,values];"
                       "  _waiting='regExp';"
                       "  return _buffer;"
                       "} "
                       + crawler.code;
    }
}

void RegExpSystem::update() {
    for (auto& execution: executions) {
        if (execution.isWaiting("regExp")) {
            try {
                const auto buffer = readRegExpBuffer(execution);
                const auto searchResults = search(buffer);
                writeRegExpBuffer(execution, searchResults);
            } catch (std::runtime_error& e) {
                execution.fail();
            }
        }
    }
}

nlohmann::json RegExpSystem::search(const nlohmann::json& searchCriteria) const {
    std::regex regex(searchCriteria.at(0).get<std::string>());
    const auto strings = searchCriteria.at(1).get<std::vector<std::string>>();
    nlohmann::json searchResults;
    for (const auto& string: strings) {
        appendMatchesToResults(string, regex, searchResults);
    }
    return searchResults;
}

void RegExpSystem::appendGroupMatchToResults(const std::smatch& match, nlohmann::json& results) const {
    for (auto i = 1; i < match.size(); i++) {
        results.push_back(match[i].str());
    }
}

void RegExpSystem::appendCompleteMatchToResults(const std::smatch& match, nlohmann::json& results) const {
    results.push_back(match[0].str());
}

void RegExpSystem::appendMatchesToResults(std::string string, const std::regex& regex, nlohmann::json& results) const {
    std::smatch match;
    while (std::regex_search(string, match, regex)) {
        if (match.size() > 1) {
            appendGroupMatchToResults(match, results);
        } else {
            appendCompleteMatchToResults(match, results);
        }
        string = match.suffix();
    }
}

nlohmann::json RegExpSystem::readRegExpBuffer(CrawlerExecution& execution) const {
    const auto buffer = execution.readSharedBuffer();
    nlohmann::json request;
    auto regex = buffer.get(0);
    const auto strings = buffer.get(1);
    request.push_back(static_cast<std::string>(regex));
    const auto count = strings.size();
    nlohmann::json requestStrings;
    for (auto i = 0; i < count; i++) {
        requestStrings.push_back(static_cast<std::string>(strings.get(i)));
    }
    request.push_back(requestStrings);
    return request;
}

void RegExpSystem::writeRegExpBuffer(CrawlerExecution& execution, const nlohmann::json& searchResults) const {
    std::vector<JsObject> response;
    response.reserve(searchResults.size());
    for (const auto& result: searchResults) {
        response.emplace_back(execution.getContext(), result.get<std::string>());
    }
    execution.writeSharedBuffer(JsObject(execution.getContext(), response));
}
}