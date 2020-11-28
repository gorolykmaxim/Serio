#include "CrawlerExecution.h"

namespace serio {
CrawlerExecution::CrawlerExecution(const std::string &code) {
    mjsContext = mjs_create();
    const auto fullCode = "let _waiting=null; let _buffer=null; " + code;
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

bool CrawlerExecution::isWaiting(const std::optional<std::string>& target) {
    if (isDone()) {
        return false;
    }
    auto waiting = getGlobal().get("_waiting");
    if (!target) {
        return !waiting.isNullOrUndefined();
    }
    return !waiting.isNullOrUndefined() && target == static_cast<std::string>(waiting);
}

JsObject CrawlerExecution::readSharedBuffer() {
    return getGlobal().get("_buffer");
}

void CrawlerExecution::writeSharedBuffer(JsObject data) {
    auto global = getGlobal();
    global.set("_waiting", JsObject(mjsContext));
    global.set("_buffer", data);
}

mjs *CrawlerExecution::getContext() {
    return mjsContext;
}

void CrawlerExecution::fail() {
    error = MJS_INTERNAL_ERROR;
}

JsObject CrawlerExecution::getResult() {
    return JsObject(mjsContext, result);
}

JsObject CrawlerExecution::getGlobal() const {
    return JsObject(mjsContext, mjs_get_global(mjsContext));
}

CrawlerExecution::~CrawlerExecution() {
    mjs_destroy(mjsContext);
}
}