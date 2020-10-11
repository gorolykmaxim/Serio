#ifndef SERIO_DATABASETVSHOWCRAWLERSTORAGE_H
#define SERIO_DATABASETVSHOWCRAWLERSTORAGE_H

#include <string>
#include <optional>
#include "BaseDatabaseStorage.h"
#include "Schema.h"

namespace serio::qt {

class DatabaseTvShowCrawlerStorage : public BaseDatabaseStorage, public Schema {
public:
    [[nodiscard]] std::optional<std::string> getTvShowCrawlerByTvShowName(const std::string &tvShowName) const;
    void deleteTvShowCrawlerOfTvShow(const std::string& tvShowName) const;
    void insertTvShowCrawler(const std::string& tvShowName, const std::string& serializedCrawler) const;
protected:
    void createNewVersion() const override;
    void migrateRecordsFromOldVersion() const override;
    void dropOldVersion() const override;
};

}

#endif //SERIO_DATABASETVSHOWCRAWLERSTORAGE_H
