#include "RegExpSystem.h"

namespace serio {
std::string RegExpSystem::getCode() const {
    return "let _regExpBuffer=null; "
           "let _regExpWaiting=false; "
           "function regExp(exp, values) {"
           "  _regExpBuffer=[exp,values];"
           "  _regExpWaiting=true;"
           "  return _regExpBuffer;"
           "}";
}

void RegExpSystem::update(CrawlerExecution &execution) const {
    const auto global = execution.getGlobal();
    if (global.get("_regExpWaiting")) {
        try {
            const auto buffer = readRegExpBuffer(global);
            const auto searchResults = search(buffer);
            writeRegExpBuffer(global, searchResults);
        } catch (std::runtime_error& e) {
            execution.fail();
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

nlohmann::json RegExpSystem::readRegExpBuffer(JsObject global) const {
    nlohmann::json request;
    const auto buffer = global.get("_regExpBuffer");
    auto regex = buffer.get(0);
    const auto strings = buffer.get(1);
    if (regex.isNullOrUndefined()) {
        throw InvalidRegExpError();
    }
    if (!strings.isArray()) {
        throw InvalidRegExpInputDataError();
    }
    request.push_back(static_cast<std::string>(regex));
    const auto count = strings.size();
    nlohmann::json requestStrings;
    for (auto i = 0; i < count; i++) {
        requestStrings.push_back(static_cast<std::string>(strings.get(i)));
    }
    request.push_back(requestStrings);
    return request;
}

void RegExpSystem::writeRegExpBuffer(JsObject global, const nlohmann::json& searchResults) const {
    global.set("_regExpWaiting", JsObject(global.mjs, false));
    std::vector<JsObject> response;
    response.reserve(searchResults.size());
    for (const auto& result: searchResults) {
        response.emplace_back(global.mjs, result.get<std::string>());
    }
    global.set("_regExpBuffer", JsObject(global.mjs, response));
}

InvalidRegExpError::InvalidRegExpError() : std::runtime_error("First argument of regExp() must be a string") {}

InvalidRegExpInputDataError::InvalidRegExpInputDataError()
    : std::runtime_error("Second argument of regExp() must be an array of strings") {}
}