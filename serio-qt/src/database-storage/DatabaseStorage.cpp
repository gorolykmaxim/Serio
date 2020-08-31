#include "DatabaseStorage.h"
#include <QString>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>

void serio::qt::DatabaseStorage::initialize(const std::string &storageUrl) {
    openDatabaseConnection(storageUrl);
    tvShowStorage.initialize();
    tvShowCrawlerStorage.initialize();
}

void serio::qt::DatabaseStorage::openDatabaseConnection(const std::string& storageUrl) {
    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(QString::fromStdString(storageUrl));
    if (!database.open()) {
        throw StorageError(storageUrl, database.lastError().text().toStdString());
    }
}

serio::core::ListPage<serio::core::TvShow> serio::qt::DatabaseStorage::getAllTvShows(unsigned int offset, unsigned int limit) {
    return core::ListPage<serio::core::TvShow>(
            offset,
            tvShowStorage.countAllTvShows(),
            tvShowStorage.getAllTvShowsInAlphabeticOrder(offset, limit));
}

void serio::qt::DatabaseStorage::saveTvShow(const core::TvShow& tvShow) {
    tvShowStorage.deleteTvShowWithName(tvShow.getName());
    tvShowStorage.insertTvShow(tvShow);
}

serio::core::ListPage<serio::core::TvShow> serio::qt::DatabaseStorage::getWatchedTvShows(unsigned int offset, unsigned int limit) {
    return core::ListPage<core::TvShow>(
            offset,
            tvShowStorage.countWatchedTvShows(),
            tvShowStorage.getWatchedTvShows(offset, limit));
}

std::optional<std::string> serio::qt::DatabaseStorage::getTvShowCrawlerByTvShowName(const std::string &tvShowName) {
    return tvShowCrawlerStorage.getTvShowCrawlerByTvShowName(tvShowName);
}

void serio::qt::DatabaseStorage::saveTvShowCrawler(const std::string &tvShowName, const std::string &serializedCrawler) {
    tvShowCrawlerStorage.deleteTvShowCrawlerOfTvShow(tvShowName);
    tvShowCrawlerStorage.insertTvShowCrawler(tvShowName, serializedCrawler);
}

serio::qt::StorageError::StorageError(const std::string &databaseName, const std::string &reason)
    : std::runtime_error("Failed to open database '" + databaseName + "'. Reason: " + reason) {}