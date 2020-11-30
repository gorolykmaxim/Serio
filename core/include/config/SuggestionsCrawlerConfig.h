#ifndef SERIO_SUGGESTIONSCRAWLERCONFIG_H
#define SERIO_SUGGESTIONSCRAWLERCONFIG_H

#include <string>
#include <chrono>

namespace serio {
struct SuggestionsCrawlerConfig {
    std::string platformName;
    std::chrono::milliseconds cacheTtl;
    std::string crawler;

    bool operator==(const SuggestionsCrawlerConfig &rhs) const;
    bool operator!=(const SuggestionsCrawlerConfig &rhs) const;
};
}

#endif //SERIO_SUGGESTIONSCRAWLERCONFIG_H
