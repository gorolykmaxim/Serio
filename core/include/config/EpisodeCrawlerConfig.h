#ifndef SERIO_EPISODECRAWLERCONFIG_H
#define SERIO_EPISODECRAWLERCONFIG_H

#include <string>
#include <chrono>

namespace serio {
struct EpisodeCrawlerConfig {
    std::string platformName;
    std::chrono::milliseconds cacheTtl;
    std::string tvShowCrawler;
    std::string episodeCrawler;

    bool operator==(const EpisodeCrawlerConfig &rhs) const;
    bool operator!=(const EpisodeCrawlerConfig &rhs) const;
};
}

#endif //SERIO_EPISODECRAWLERCONFIG_H
