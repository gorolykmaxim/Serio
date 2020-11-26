#include "CrawlerExecution.h"

namespace serio {
CrawlerExecution::CrawlerExecution(const std::string &code, const nlohmann::json &arguments) {
    mjsContext = mjs_create();
    initializeArguments(arguments);
    const auto fullCode = "let _waiting=false; let _buffer=null; " + code + " JSON.stringify(crawl(args));";
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

bool CrawlerExecution::isWaiting() {
    return getGlobal().get("_waiting");
}

JsObject CrawlerExecution::readSharedBuffer() {
    return getGlobal().get("_buffer");
}

void CrawlerExecution::writeSharedBuffer(JsObject data) {
    auto global = getGlobal();
    global.set("_waiting", JsObject(mjsContext, false));
    global.set("_buffer", data);
}

mjs *CrawlerExecution::getContext() {
    return mjsContext;
}

void CrawlerExecution::fail() {
    error = MJS_INTERNAL_ERROR;
}

nlohmann::json CrawlerExecution::getResult() {
    return nlohmann::json::parse(static_cast<std::string>(JsObject(mjsContext, result)));
}

JsObject CrawlerExecution::getGlobal() const {
    return JsObject(mjsContext, mjs_get_global(mjsContext));
}

CrawlerExecution::~CrawlerExecution() {
    mjs_destroy(mjsContext);
}

void CrawlerExecution::initializeArguments(const nlohmann::json &arguments) {
    if (!arguments.is_array() && !arguments.is_null()) {
        throw NonArrayCrawlerArgumentsError();
    }
    std::vector<JsObject> args;
    args.reserve(arguments.size());
    for (const auto& argument: arguments) {
        args.emplace_back(mjsContext, toMjsValue(argument));
    }
    getGlobal().set("args", JsObject(mjsContext, args));
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