#ifndef SERIO_TVSHOWCRAWLERLOGSTORAGE_H
#define SERIO_TVSHOWCRAWLERLOGSTORAGE_H

#include <string>
#include "CrawlLogEntry.h"

namespace serio::core {

class TvShowCrawlerLogStorage {
public:
    virtual void saveCrawlLog(const std::string& tvShowName, const std::vector<CrawlLogEntry>& log) = 0;
    [[nodiscard]] virtual std::vector<CrawlLogEntry> getLastCrawlLogOfTvShow(const std::string& tvShowName) = 0;
};

}

#endif //SERIO_TVSHOWCRAWLERLOGSTORAGE_H
