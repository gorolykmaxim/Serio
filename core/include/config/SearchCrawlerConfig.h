#ifndef SERIO_SEARCHCRAWLERCONFIG_H
#define SERIO_SEARCHCRAWLERCONFIG_H

#include <string>
#include <chrono>

namespace serio {
struct SearchCrawlerConfig {
    std::string platformName;
    std::chrono::milliseconds cacheTtl;
    std::string crawler;
    bool isAvailableToUser;

    bool operator==(const SearchCrawlerConfig &rhs) const;
    bool operator!=(const SearchCrawlerConfig &rhs) const;
};
}

#endif //SERIO_SEARCHCRAWLERCONFIG_H
