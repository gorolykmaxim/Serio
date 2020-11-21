#ifndef SERIO_CATEGORYCRAWLERCONFIG_H
#define SERIO_CATEGORYCRAWLERCONFIG_H

#include <string>
#include <chrono>

namespace serio {
struct CategoryCrawlerConfig {
    std::string platformName;
    std::string categoryName;
    std::chrono::milliseconds cacheTtl;
    std::string crawler;

    bool operator==(const CategoryCrawlerConfig &rhs) const;
    bool operator!=(const CategoryCrawlerConfig &rhs) const;
};
}

#endif //SERIO_CATEGORYCRAWLERCONFIG_H
