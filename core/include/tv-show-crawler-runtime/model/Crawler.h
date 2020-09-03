#ifndef SERIO_CRAWLER_H
#define SERIO_CRAWLER_H

#include <vector>
#include "CrawlerStep.h"

namespace serio::core {

class Crawler {
public:
    Crawler(std::vector<CrawlerStep> steps = {});
    std::vector<CrawlerStep> getSteps() const;
    bool operator==(const Crawler &rhs) const;
    bool operator!=(const Crawler &rhs) const;
    bool hasSteps() const;
private:
    std::vector<CrawlerStep> steps;
};

class CrawlerExecutionError : public std::runtime_error {
public:
    CrawlerExecutionError(const std::string& crawlerType, const std::exception& cause);
};

}

#endif //SERIO_CRAWLER_H
