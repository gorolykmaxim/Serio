#include <config/CategoryCrawlerConfig.h>

namespace serio {
bool CategoryCrawlerConfig::operator==(const CategoryCrawlerConfig &rhs) const {
    return platformName == rhs.platformName &&
           categoryName == rhs.categoryName &&
           cacheTtl == rhs.cacheTtl &&
           crawler == rhs.crawler;
}

bool CategoryCrawlerConfig::operator!=(const CategoryCrawlerConfig &rhs) const {
    return !(rhs == *this);
}
}