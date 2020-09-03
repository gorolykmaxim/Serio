#include <utility>
#include "tv-show-crawler-runtime/model/Crawler.h"

serio::core::Crawler::Crawler(std::vector<CrawlerStep> steps) : steps(std::move(steps)) {}

std::vector<serio::core::CrawlerStep> serio::core::Crawler::getSteps() const {
    return steps;
}

bool serio::core::Crawler::operator==(const serio::core::Crawler &rhs) const {
    return steps == rhs.steps;
}

bool serio::core::Crawler::operator!=(const serio::core::Crawler &rhs) const {
    return !(rhs == *this);
}

bool serio::core::Crawler::hasSteps() const {
    return !steps.empty();
}

serio::core::CrawlerExecutionError::CrawlerExecutionError(const std::string &crawlerType, const std::exception &cause)
    : std::runtime_error("Failed to execute " + crawlerType + " crawler: " + cause.what()) {}
