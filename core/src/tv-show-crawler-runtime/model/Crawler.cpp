#include <utility>
#include "tv-show-crawler-runtime/model/Crawler.h"

namespace serio::core {

Crawler::Crawler(std::vector<CrawlerStep> steps) : steps(std::move(steps)) {}

std::vector<CrawlerStep> Crawler::getSteps() const {
    return steps;
}

bool Crawler::operator==(const Crawler &rhs) const {
    return steps == rhs.steps;
}

bool Crawler::operator!=(const Crawler &rhs) const {
    return !(rhs == *this);
}

bool Crawler::hasSteps() const {
    return !steps.empty();
}

CrawlerExecutionError::CrawlerExecutionError(const std::string &crawlerType, const std::exception &cause)
    : std::runtime_error("Failed to execute " + crawlerType + " crawler: " + cause.what()) {}

}