#ifndef SERIO_CRAWLEREXECUTION_H
#define SERIO_CRAWLEREXECUTION_H

#include <mjs.h>
#include "JsObject.h"

namespace serio {
class CrawlerExecution {
public:
    explicit CrawlerExecution(const std::string &code);
    void executeStep();
    bool isDone();
    bool hasFailed();
    bool isWaiting();
    bool isWaiting(const std::string& target);
    JsObject readSharedBuffer();
    void writeSharedBuffer(JsObject data);
    mjs* getContext();
    void fail(const std::exception& exception);
    [[nodiscard]] JsObject getGlobal() const;
    JsObject getResult();
    std::string getWaitingTarget();
    std::string getErrorMessage();
    virtual ~CrawlerExecution();
private:
    struct mjs* mjsContext;
    struct mjs_execution executionContext;
    mjs_err_t error;
    mjs_val_t result;
    std::string waiting;
};
}

#endif //SERIO_CRAWLEREXECUTION_H
