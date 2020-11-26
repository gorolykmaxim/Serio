#include "CrawlerArgumentSystem.h"

namespace serio {
CrawlerArgumentSystem::CrawlerArgumentSystem(std::vector<Crawler> &crawlers,
                                             std::vector<CrawlerExecution> &executions) {
    for (auto i = 0; i < crawlers.size(); i++) {
        try {
            initializeArguments(crawlers[i], executions[i]);
        } catch (std::logic_error& e) {
            throw InvalidCrawlerError(crawlers[i], e.what());
        }
    }
}

void CrawlerArgumentSystem::initializeArguments(const Crawler& crawler, CrawlerExecution& execution) {
    const auto arguments = crawler.arguments;
    const auto context = execution.getContext();
    if (!arguments.is_array() && !arguments.is_null()) {
        throw NonArrayCrawlerArgumentsError();
    }
    std::vector<JsObject> args;
    args.reserve(arguments.size());
    for (const auto& argument: arguments) {
        args.emplace_back(context, toMjsValue(context, argument));
    }
    execution.getGlobal().set("args", JsObject(context, args));
}

mjs_val_t CrawlerArgumentSystem::toMjsValue(mjs* context, const nlohmann::json &value) {
    if (value.is_string()) {
        return mjs_mk_string(context, value.get<std::string>().c_str(), value.get<std::string>().length(), true);
    } else if (value.is_boolean()) {
        return mjs_mk_boolean(context, value.get<bool>());
    } else if (value.is_number()) {
        return mjs_mk_number(context, value.get<double>());
    } else {
        throw InvalidCrawlerArgumentTypeError(value);
    }
}

InvalidCrawlerError::InvalidCrawlerError(const Crawler& crawler, const std::string &reason)
    : std::logic_error("Crawler with code '" + crawler.code + "' and arguments '" + crawler.arguments.dump() + "' is invalid: " + reason) {}

NonArrayCrawlerArgumentsError::NonArrayCrawlerArgumentsError() : std::logic_error("Crawler arguments should be an array") {}

InvalidCrawlerArgumentTypeError::InvalidCrawlerArgumentTypeError(const nlohmann::json &argument)
    : std::logic_error("Crawler argument with invalid type: " + argument.dump()) {}
}