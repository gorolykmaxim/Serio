#include <config/EpisodeCrawlerConfig.h>

namespace serio {
bool EpisodeCrawlerConfig::operator==(const EpisodeCrawlerConfig &rhs) const {
    return platformName == rhs.platformName &&
           cacheTtl == rhs.cacheTtl &&
           tvShowCrawler == rhs.tvShowCrawler &&
           episodeCrawler == rhs.episodeCrawler;
}

bool EpisodeCrawlerConfig::operator!=(const EpisodeCrawlerConfig &rhs) const {
    return !(rhs == *this);
}
}