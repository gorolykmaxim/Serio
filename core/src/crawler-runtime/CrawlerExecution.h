#ifndef SERIO_CRAWLEREXECUTION_H
#define SERIO_CRAWLEREXECUTION_H

#include <mjs.h>
#include <nlohmann/json.hpp>
#include "JsObject.h"

namespace serio {
class CrawlerExecution {
public:
    explicit CrawlerExecution(const std::string &code);
    void executeStep();
    bool isDone();
    bool hasFailed();
    bool isWaiting();
    JsObject readSharedBuffer();
    void writeSharedBuffer(JsObject data);
    mjs* getContext();
    void fail();
    [[nodiscard]] JsObject getGlobal() const;
    nlohmann::json getResult();
    virtual ~CrawlerExecution();
private:
    struct mjs* mjsContext;
    struct mjs_execution executionContext;
    mjs_err_t error;
    mjs_val_t result;
};
}

#endif //SERIO_CRAWLEREXECUTION_H
