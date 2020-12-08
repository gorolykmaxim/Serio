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
            const auto searchTask = getCurrentSearchTask(execution);
            const auto searchResults = search(searchTask);
            setCurrentSearchResults(execution, searchResults);
        }
    }
}

std::vector<std::string> RegExpSystem::search(const RegExpSearchTask& task) const {
    std::vector<std::string> results;
    for (const auto& string: task.stringsToSearch) {
        appendMatchesToResults(string, task.regex, results);
    }
    return results;
}

void RegExpSystem::appendGroupMatchToResults(const std::smatch& match, std::vector<std::string>& results) const {
    for (auto i = 1; i < match.size(); i++) {
        results.push_back(match[i].str());
    }
}

void RegExpSystem::appendCompleteMatchToResults(const std::smatch& match, std::vector<std::string>& results) const {
    results.push_back(match[0].str());
}

void RegExpSystem::appendMatchesToResults(const std::string& string, const std::regex& regex, std::vector<std::string>& results) const {
    const auto begin = std::sregex_iterator(string.cbegin(), string.cend(), regex);
    const auto end = std::sregex_iterator();
    for (auto it = begin; it != end; it++) {
        if (it->size() > 1) {
            appendGroupMatchToResults(*it, results);
        } else {
            appendCompleteMatchToResults(*it, results);
        }
    }
}

RegExpSearchTask RegExpSystem::getCurrentSearchTask(CrawlerExecution& execution) const {
    const auto buffer = execution.readSharedBuffer();
    const auto regex = static_cast<std::string>(buffer.get(0));
    const auto strings = buffer.get(1);
    const auto count = strings.size();
    std::vector<std::string> stringsToSearch;
    stringsToSearch.reserve(strings.size());
    for (auto i = 0; i < count; i++) {
        stringsToSearch.emplace_back(static_cast<std::string>(strings.get(i)));
    }
    return {std::regex(regex), stringsToSearch};
}

void RegExpSystem::setCurrentSearchResults(CrawlerExecution& execution, const std::vector<std::string>& results) const {
    std::vector<JsObject> response;
    response.reserve(results.size());
    for (const auto& result: results) {
        response.emplace_back(execution.getContext(), result);
    }
    execution.writeSharedBuffer(JsObject(execution.getContext(), response));
}
}