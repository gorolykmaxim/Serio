#ifndef SERIO_ARGUMENTSYSTEM_H
#define SERIO_ARGUMENTSYSTEM_H

#include <vector>
#include <set>
#include <crawler-runtime/CrawlerRuntime.h>
#include "../component/ExecutionComponent.h"

namespace serio {
class ArgumentSystem {
public:
    ArgumentSystem(const std::vector<Crawler>& crawlers, std::vector<ExecutionComponent>& executions);
private:
    mjs_val_t toMjsValue(mjs* mjs, const nlohmann::json& value);
};

class NonArrayCrawlerArgumentsError : public std::logic_error {
public:
    NonArrayCrawlerArgumentsError();
};

class InvalidCrawlerArgumentTypeError : public std::logic_error {
public:
    explicit InvalidCrawlerArgumentTypeError(const nlohmann::json& argument);
};

class InvalidCrawlerError : public std::logic_error {
public:
    InvalidCrawlerError(const Crawler& crawler, const std::string &reason);
};
}

#endif //SERIO_ARGUMENTSYSTEM_H
