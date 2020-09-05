#ifndef SERIO_DATABASETVSHOWCRAWLERSTORAGE_H
#define SERIO_DATABASETVSHOWCRAWLERSTORAGE_H

#include <string>
#include <optional>

namespace serio::qt {

class DatabaseTvShowCrawlerStorage {
public:
    void initialize();
    std::optional<std::string> getTvShowCrawlerByTvShowName(const std::string &tvShowName);
    void deleteTvShowCrawlerOfTvShow(const std::string& tvShowName);
    void insertTvShowCrawler(const std::string& tvShowName, const std::string& serializedCrawler);
};

}

#endif //SERIO_DATABASETVSHOWCRAWLERSTORAGE_H
