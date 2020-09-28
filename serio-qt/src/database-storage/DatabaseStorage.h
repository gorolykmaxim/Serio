#ifndef SERIO_DATABASESTORAGE_H
#define SERIO_DATABASESTORAGE_H

#include <QSqlDatabase>
#include <string>
#include <tv-show-storage/TvShowStorage.h>
#include <tv-show-crawler-storage/TvShowCrawlerStorage.h>
#include <stdexcept>
#include <tv-show-crawler-log-storage/TvShowCrawlerLogStorage.h>
#include "DatabaseTvShowStorage.h"
#include "DatabaseTvShowCrawlerStorage.h"
#include "DatabaseTvShowCrawlerLogStorage.h"

namespace serio::qt {

class DatabaseStorage : public core::TvShowStorage, public core::TvShowCrawlerStorage, public core::TvShowCrawlerLogStorage {
public:
    void initialize(bool inMemory = false);
    std::optional<core::TvShow> getTvShowByName(const std::string &tvShowName) override;
    core::ListPage<core::TvShow> getAllTvShows(unsigned int offset, unsigned int limit) override;
    core::ListPage<core::TvShow> getWatchedTvShows(unsigned int offset, unsigned int limit) override;
    core::ListPage<core::Episode> getEpisodesOfTvShowWithName(const std::string &tvShowName, unsigned int offset, unsigned int limit) override;
    void saveTvShow(const core::TvShow& tvShow, const std::vector<core::Episode>& episodes) override;
    std::optional<std::string> getTvShowCrawlerByTvShowName(const std::string &tvShowName) override;
    void saveTvShowCrawler(const std::string &tvShowName, const std::string &serializedCrawler) override;
    void saveCrawlLog(const std::string &tvShowName, const std::vector<core::CrawlLogEntry> &log) override;
    std::vector<core::CrawlLogEntry> getLastCrawlLogOfTvShow(const std::string &tvShowName) override;
    void clearTvShowWatchHistory(const std::string &tvShowName) override;
private:
    DatabaseTvShowStorage tvShowStorage;
    DatabaseTvShowCrawlerStorage tvShowCrawlerStorage;
    DatabaseTvShowCrawlerLogStorage tvShowCrawlerLogStorage;
    [[nodiscard]] std::string getDatabaseFilePath() const;
    void openDatabaseConnection(const std::string& storageUrl);
    void enableForeignKeys();
};

class StorageError : public std::runtime_error {
public:
    StorageError(const std::string& databaseName, const std::string& reason);
};

}

#endif //SERIO_DATABASESTORAGE_H
