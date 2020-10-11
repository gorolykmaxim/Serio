#ifndef SERIO_DATABASETVSHOWCRAWLERLOGSTORAGE_H
#define SERIO_DATABASETVSHOWCRAWLERLOGSTORAGE_H

#include <string>
#include <vector>
#include <tv-show-crawler-log-storage/CrawlLogEntry.h>
#include "BaseDatabaseStorage.h"
#include "Schema.h"

namespace serio::qt {

class DatabaseTvShowCrawlerLogStorage : public BaseDatabaseStorage, public Schema {
public:
    void saveCrawlLog(const std::string &tvShowName, const std::vector<core::CrawlLogEntry> &log) const;
    [[nodiscard]] std::vector<core::CrawlLogEntry> getLastCrawlLogOfTvShow(const std::string &tvShowName) const;
protected:
    void backupOldVersion() const override;
    void createNewVersion() const override;
    void migrateRecordsFromOldVersion() const override;
    void dropOldVersion() const override;
private:
    void insertCrawlLogEntries(const std::string& tvShowName, const std::vector<core::CrawlLogEntry>& entries) const;
    [[nodiscard]] core::CrawlLogEntry readCrawlLogEntryFrom(const QSqlQuery& query) const;
};

}

#endif //SERIO_DATABASETVSHOWCRAWLERLOGSTORAGE_H
