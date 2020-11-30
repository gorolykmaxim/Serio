#include <config/TvShowCrawlerConfig.h>

namespace serio {
bool TvShowCrawlerConfig::operator==(const TvShowCrawlerConfig &rhs) const {
    return platformName == rhs.platformName &&
           cacheTtl == rhs.cacheTtl &&
           crawler == rhs.crawler &&
           suggestionsCrawler == rhs.suggestionsCrawler;
}

bool TvShowCrawlerConfig::operator!=(const TvShowCrawlerConfig &rhs) const {
    return !(rhs == *this);
}
}