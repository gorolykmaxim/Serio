#ifndef SERIO_CRAWLEREXECUTION_H
#define SERIO_CRAWLEREXECUTION_H

#include <mjs.h>
#include <nlohmann/json.hpp>
#include "JsObject.h"

namespace serio {
class CrawlerExecution {
public:
    explicit CrawlerExecution(const std::string &code, const nlohmann::json &arguments);
    void executeStep();
    bool isDone();
    bool hasFailed();
    bool isWaiting();
    JsObject readSharedBuffer();
    void writeSharedBuffer(JsObject data);
    mjs* getContext();
    void fail();
    nlohmann::json getResult();
    virtual ~CrawlerExecution();
private:
    struct mjs* mjsContext;
    struct mjs_execution executionContext;
    mjs_err_t error;
    mjs_val_t result;

    [[nodiscard]] JsObject getGlobal() const;
    void initializeArguments(const nlohmann::json &arguments);
    mjs_val_t toMjsValue(const nlohmann::json& value);
};

class NonArrayCrawlerArgumentsError : public std::logic_error {
public:
    NonArrayCrawlerArgumentsError();
};

class InvalidCrawlerArgumentTypeError : public std::logic_error {
public:
    explicit InvalidCrawlerArgumentTypeError(const nlohmann::json& argument);
};
}

#endif //SERIO_CRAWLEREXECUTION_H
