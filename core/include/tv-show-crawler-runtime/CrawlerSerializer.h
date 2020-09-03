#ifndef SERIO_CRAWLERSERIALIZER_H
#define SERIO_CRAWLERSERIALIZER_H

#include <nlohmann/json.hpp>
#include "tv-show-crawler-runtime/model/TvShowCrawler.h"

namespace serio::core {

class CrawlerSerializer {
public:
    std::string serialize(const TvShowCrawler& crawler) const;
private:
    nlohmann::json serialize(const Crawler& crawler) const;
    nlohmann::json serialize(const CrawlerStep& step) const;
};

}

#endif //SERIO_CRAWLERSERIALIZER_H
