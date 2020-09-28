#ifndef SERIO_TVSHOWCRAWLERSTORAGE_H
#define SERIO_TVSHOWCRAWLERSTORAGE_H

#include <string>
#include <optional>

namespace serio::core {

class TvShowCrawlerStorage {
public:
    virtual std::optional<std::string> getTvShowCrawlerByTvShowName(const std::string& tvShowName) = 0;
    virtual void saveTvShowCrawler(const std::string& tvShowName, const std::string& serializedCrawler) = 0;
    virtual void deleteCrawlerOfTvShow(const std::string& tvShowName) = 0;
};

}

#endif //SERIO_TVSHOWCRAWLERSTORAGE_H