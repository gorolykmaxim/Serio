#include <config/SuggestionsCrawlerConfig.h>

namespace serio {
bool SuggestionsCrawlerConfig::operator==(const SuggestionsCrawlerConfig &rhs) const {
    return platformName == rhs.platformName &&
           cacheTtl == rhs.cacheTtl &&
           crawler == rhs.crawler;
}

bool SuggestionsCrawlerConfig::operator!=(const SuggestionsCrawlerConfig &rhs) const {
    return !(rhs == *this);
}
}