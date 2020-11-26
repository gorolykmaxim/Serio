#ifndef SERIO_CRAWLERARGUMENTSYSTEM_H
#define SERIO_CRAWLERARGUMENTSYSTEM_H

#include <vector>
#include <crawler-runtime/CrawlerRuntime.h>
#include "CrawlerExecution.h"

namespace serio {
class CrawlerArgumentSystem {
public:
    CrawlerArgumentSystem(std::vector<Crawler>& crawlers, std::vector<CrawlerExecution>& executions);
private:
    void initializeArguments(const Crawler& crawler, CrawlerExecution& execution);
    mjs_val_t toMjsValue(mjs* context, const nlohmann::json& value);
};

class InvalidCrawlerError : public std::logic_error {
public:
    InvalidCrawlerError(const Crawler& crawler, const std::string &reason);
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

#endif //SERIO_CRAWLERARGUMENTSYSTEM_H
