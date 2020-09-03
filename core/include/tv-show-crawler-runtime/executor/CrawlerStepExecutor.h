#ifndef SERIO_CRAWLERSTEPEXECUTOR_H
#define SERIO_CRAWLERSTEPEXECUTOR_H

#include <vector>
#include "tv-show-crawler-runtime/model/CrawlerStep.h"

namespace serio::core {

class CrawlerStepExecutor {
public:
    virtual std::vector<std::string> execute(const CrawlerStep& step, const std::vector<std::string>& previousStepResults) const = 0;
    virtual ~CrawlerStepExecutor() {}
};

}

#endif //SERIO_CRAWLERSTEPEXECUTOR_H
