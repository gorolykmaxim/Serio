#include <crawler-runtime/CrawlerExecution.h>

namespace serio {
CrawlerExecution::CrawlerExecution(const std::string& code) {
    mjsContext = mjs_create();
    const auto fullCode = code + " JSON.stringify(crawl())";
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
}