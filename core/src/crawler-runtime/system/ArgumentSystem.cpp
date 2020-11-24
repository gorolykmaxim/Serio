#include "ArgumentSystem.h"

namespace serio {
ArgumentSystem::ArgumentSystem(const std::vector<Crawler> &crawlers,
                               std::vector<ExecutionComponent> &executions) {
    for (auto i = 0; i < crawlers.size(); i++) {
        const auto crawler = crawlers.at(i);
        try {
            auto mjs = executions[i].mjs;
            auto arguments = crawler.arguments;
            if (!arguments.is_array() && !arguments.is_null()) {
                throw NonArrayCrawlerArgumentsError();
            }
            const std::string args = "args";
            const auto global = mjs_get_global(mjs);
            const auto mjsArgsName = mjs_mk_string(mjs, args.c_str(), args.length(), true);
            const auto mjsArgsVal = mjs_mk_array(mjs);
            for (const auto& argument: arguments) {
                mjs_array_push(mjs, mjsArgsVal, toMjsValue(mjs, argument));
            }
            mjs_set_v(mjs, global, mjsArgsName, mjsArgsVal);
        } catch (std::logic_error& e) {
            throw InvalidCrawlerError(crawler, e.what());
        }
    }
}

mjs_val_t ArgumentSystem::toMjsValue(mjs* mjs, const nlohmann::json &value) {
    if (value.is_string()) {
        return mjs_mk_string(mjs, value.get<std::string>().c_str(), value.get<std::string>().length(), true);
    } else if (value.is_boolean()) {
        return mjs_mk_boolean(mjs, value.get<bool>());
    } else if (value.is_number()) {
        return mjs_mk_number(mjs, value.get<double>());
    } else {
        throw InvalidCrawlerArgumentTypeError(value);
    }
}

NonArrayCrawlerArgumentsError::NonArrayCrawlerArgumentsError() : std::logic_error("Crawler arguments should be an array") {}

InvalidCrawlerArgumentTypeError::InvalidCrawlerArgumentTypeError(const nlohmann::json &argument)
    : std::logic_error("Crawler argument with invalid type: " + argument.dump()) {}

InvalidCrawlerError::InvalidCrawlerError(const Crawler& crawler, const std::string &reason)
    : std::logic_error("Crawler with code '" + crawler.code + "' and arguments '" + crawler.arguments.dump() + "' is invalid: " + reason) {}
}