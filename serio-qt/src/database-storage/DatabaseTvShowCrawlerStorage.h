#ifndef SERIO_DATABASETVSHOWCRAWLERSTORAGE_H
#define SERIO_DATABASETVSHOWCRAWLERSTORAGE_H

#include <string>
#include <optional>
#include "BaseDatabaseStorage.h"

namespace serio::qt {

class DatabaseTvShowCrawlerStorage : public BaseDatabaseStorage {
public:
    void initialize() const;
    [[nodiscard]] std::optional<std::string> getTvShowCrawlerByTvShowName(const std::string &tvShowName) const;
    void deleteTvShowCrawlerOfTvShow(const std::string& tvShowName) const;
    void insertTvShowCrawler(const std::string& tvShowName, const std::string& serializedCrawler) const;
};

}

#endif //SERIO_DATABASETVSHOWCRAWLERSTORAGE_H
