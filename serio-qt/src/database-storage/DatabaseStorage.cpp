#include "DatabaseStorage.h"
#include <QString>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>
#include <QStandardPaths>
#include <QDir>

void serio::qt::DatabaseStorage::initialize(bool inMemory) {
    openDatabaseConnection(inMemory ? ":memory:" : getDatabaseFilePath());
    enableForeignKeys();
    tvShowStorage.initialize();
    tvShowCrawlerStorage.initialize();
}

serio::core::ListPage<serio::core::TvShow> serio::qt::DatabaseStorage::getAllTvShows(unsigned int offset, unsigned int limit) {
    return core::ListPage<serio::core::TvShow>(
            offset,
            tvShowStorage.countAllTvShows(),
            tvShowStorage.getAllTvShowsInAlphabeticOrder(offset, limit));
}

serio::core::ListPage<serio::core::TvShow> serio::qt::DatabaseStorage::getWatchedTvShows(unsigned int offset, unsigned int limit) {
    return core::ListPage<core::TvShow>(
            offset,
            tvShowStorage.countWatchedTvShows(),
            tvShowStorage.getWatchedTvShows(offset, limit));
}

serio::core::ListPage<serio::core::Episode> serio::qt::DatabaseStorage::getEpisodesOfTvShowWithName(const std::string &tvShowName,
                                                                                                    unsigned int offset,
                                                                                                    unsigned int limit) {
    return core::ListPage<core::Episode>(
            offset,
            tvShowStorage.countEpisodesOfTvShowWithName(tvShowName),
            tvShowStorage.getEpisodesOfTvShowWithName(tvShowName, offset, limit));
}

void serio::qt::DatabaseStorage::saveTvShow(const core::TvShow& tvShow, const std::vector<core::Episode>& episodes) {
    tvShowStorage.saveTvShow(tvShow, episodes);
}

std::optional<std::string> serio::qt::DatabaseStorage::getTvShowCrawlerByTvShowName(const std::string &tvShowName) {
    return tvShowCrawlerStorage.getTvShowCrawlerByTvShowName(tvShowName);
}

void serio::qt::DatabaseStorage::saveTvShowCrawler(const std::string &tvShowName, const std::string &serializedCrawler) {
    tvShowCrawlerStorage.deleteTvShowCrawlerOfTvShow(tvShowName);
    tvShowCrawlerStorage.insertTvShowCrawler(tvShowName, serializedCrawler);
}

std::string serio::qt::DatabaseStorage::getDatabaseFilePath() const {
    QString home = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    return QDir(home).filePath(".serio.dev.db").toStdString();
}

void serio::qt::DatabaseStorage::openDatabaseConnection(const std::string& storageUrl) {
    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(QString::fromStdString(storageUrl));
    if (!database.open()) {
        throw StorageError(storageUrl, database.lastError().text().toStdString());
    }
}

void serio::qt::DatabaseStorage::enableForeignKeys() {
    QSqlQuery enableForeignKeys(QSqlDatabase::database());
    enableForeignKeys.exec("PRAGMA foreign_keys=ON");
}

serio::qt::StorageError::StorageError(const std::string &databaseName, const std::string &reason)
    : std::runtime_error("Failed to open database '" + databaseName + "'. Reason: " + reason) {}
