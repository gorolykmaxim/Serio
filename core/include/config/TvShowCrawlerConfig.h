#ifndef SERIO_TVSHOWCRAWLERCONFIG_H
#define SERIO_TVSHOWCRAWLERCONFIG_H

#include <string>
#include <optional>
#include <chrono>

namespace serio {
struct TvShowCrawlerConfig {
    std::string platformName;
    std::chrono::milliseconds cacheTtl;
    std::string crawler;
    std::optional<std::string> suggestionsCrawler;

    bool operator==(const TvShowCrawlerConfig &rhs) const;
    bool operator!=(const TvShowCrawlerConfig &rhs) const;
};
}

#endif //SERIO_TVSHOWCRAWLERCONFIG_H
