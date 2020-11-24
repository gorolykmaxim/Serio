#include <crawler-runtime/CrawlerExecution.h>

namespace serio {
CrawlerExecution::CrawlerExecution(const std::string &code, const nlohmann::json &arguments) {
    mjsContext = mjs_create();
    initializeArguments(arguments);
    const auto fullCode = code + " JSON.stringify(crawl.apply(null, args))";
    executionContext.done = 0;
    error = mjs_start_execution(mjsContext, &executionContext, fullCode.c_str(), &result);
}

void CrawlerExecution::executeStep() {
    error = mjs_execute_step(&executionContext);
}

bool CrawlerExecution::isDone() {
    return hasFailed() || mjs_is_execution_done(&executionContext);
}

bool CrawlerExecution::hasFailed() {
    return error != MJS_OK;
}

nlohmann::json CrawlerExecution::getResult() {
    const auto stringResult = mjs_get_cstring(mjsContext, &result);
    return nlohmann::json::parse(stringResult);
}

CrawlerExecution::~CrawlerExecution() {
    mjs_destroy(mjsContext);
}

void CrawlerExecution::initializeArguments(const nlohmann::json &arguments) {
    if (!arguments.is_array() && !arguments.is_null()) {
        throw NonArrayCrawlerArgumentsError();
    }
    const std::string args = "args";
    const auto global = mjs_get_global(mjsContext);
    const auto mjsArgsName = mjs_mk_string(mjsContext, args.c_str(), args.length(), true);
    const auto mjsArgsVal = mjs_mk_array(mjsContext);
    for (const auto& argument: arguments) {
        mjs_array_push(mjsContext, mjsArgsVal, toMjsValue(argument));
    }
    mjs_set_v(mjsContext, global, mjsArgsName, mjsArgsVal);
}

mjs_val_t CrawlerExecution::toMjsValue(const nlohmann::json &value) {
    if (value.is_string()) {
        return mjs_mk_string(mjsContext, value.get<std::string>().c_str(), value.get<std::string>().length(), true);
    } else if (value.is_boolean()) {
        return mjs_mk_boolean(mjsContext, value.get<bool>());
    } else if (value.is_number()) {
        return mjs_mk_number(mjsContext, value.get<double>());
    } else {
        throw InvalidCrawlerArgumentTypeError(value);
    }
}

NonArrayCrawlerArgumentsError::NonArrayCrawlerArgumentsError() : std::logic_error("Crawler arguments should be an array") {}

InvalidCrawlerArgumentTypeError::InvalidCrawlerArgumentTypeError(const nlohmann::json &argument)
    : std::logic_error("Crawler argument with invalid type: " + argument.dump()) {}
}