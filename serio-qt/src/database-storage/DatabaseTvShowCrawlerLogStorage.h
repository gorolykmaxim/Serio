#ifndef SERIO_DATABASETVSHOWCRAWLERLOGSTORAGE_H
#define SERIO_DATABASETVSHOWCRAWLERLOGSTORAGE_H

#include <string>
#include <vector>
#include <tv-show-crawler-log-storage/CrawlLogEntry.h>
#include "BaseDatabaseStorage.h"

namespace serio::qt {

class DatabaseTvShowCrawlerLogStorage : public BaseDatabaseStorage {
public:
    void initialize();
    void saveCrawlLog(const std::string &tvShowName, const std::vector<core::CrawlLogEntry> &log);
    std::vector<core::CrawlLogEntry> getLastCrawlLogOfTvShow(const std::string &tvShowName);
private:
    void insertCrawlLogEntries(const std::string& tvShowName, const std::vector<core::CrawlLogEntry>& entries);
    [[nodiscard]] core::CrawlLogEntry readCrawlLogEntryFrom(const QSqlQuery& query) const;
};

}

#endif //SERIO_DATABASETVSHOWCRAWLERLOGSTORAGE_H
