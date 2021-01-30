#include <config/SearchCrawlerConfig.h>

namespace serio {
bool SearchCrawlerConfig::operator==(const SearchCrawlerConfig &rhs) const {
    return platformName == rhs.platformName &&
           cacheTtl == rhs.cacheTtl &&
           pagesPerLoad == rhs.pagesPerLoad &&
           crawler == rhs.crawler;
}

bool SearchCrawlerConfig::operator!=(const SearchCrawlerConfig &rhs) const {
    return !(rhs == *this);
}
}