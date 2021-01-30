#include <config/CategoryCrawlerConfig.h>

namespace serio {
bool CategoryCrawlerConfig::operator==(const CategoryCrawlerConfig &rhs) const {
    return platformName == rhs.platformName &&
           categoryName == rhs.categoryName &&
           cacheTtl == rhs.cacheTtl &&
           pagesPerLoad == rhs.pagesPerLoad &&
           crawler == rhs.crawler;
}

bool CategoryCrawlerConfig::operator!=(const CategoryCrawlerConfig &rhs) const {
    return !(rhs == *this);
}
}