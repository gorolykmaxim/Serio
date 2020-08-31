#ifndef SERIO_DATABASESTORAGE_H
#define SERIO_DATABASESTORAGE_H

#include <QSqlDatabase>
#include <string>
#include <tv-show-storage/TvShowStorage.h>
#include <tv-show-crawler-storage/TvShowCrawlerStorage.h>
#include <stdexcept>
#include "DatabaseTvShowStorage.h"
#include "DatabaseTvShowCrawlerStorage.h"

namespace serio::qt {

class DatabaseStorage : public core::TvShowStorage, public core::TvShowCrawlerStorage {
public:
    void initialize(const std::string& storageUrl);
    core::ListPage<core::TvShow> getAllTvShows(unsigned int offset, unsigned int limit) override;
    core::ListPage<core::TvShow> getWatchedTvShows(unsigned int offset, unsigned int limit) override;
    void saveTvShow(const core::TvShow& tvShow) override;
    std::optional<std::string> getTvShowCrawlerByTvShowName(const std::string &tvShowName) override;
    void saveTvShowCrawler(const std::string &tvShowName, const std::string &serializedCrawler) override;
private:
    DatabaseTvShowStorage tvShowStorage;
    DatabaseTvShowCrawlerStorage tvShowCrawlerStorage;
    void openDatabaseConnection(const std::string& storageUrl);
};

class StorageError : public std::runtime_error {
public:
    StorageError(const std::string& databaseName, const std::string& reason);
};

}

#endif //SERIO_DATABASESTORAGE_H
