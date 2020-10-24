#include "DatabaseStorage.h"
#include <QString>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>
#include <QStandardPaths>
#include <QDir>

namespace serio::qt {

void DatabaseStorage::initialize(bool inMemory) const {
    openDatabase(inMemory);
    initializeSchema();
}

void DatabaseStorage::openDatabase(bool inMemory) const {
    openDatabaseConnection(inMemory ? ":memory:" : getDatabaseFilePath());
}

void DatabaseStorage::initializeSchema() const {
    enableForeignKeys();
    bool migrate = isSchemaMigrationRequired();
    tvShowCrawlerLogStorage.initialize(migrate);
    tvShowCrawlerStorage.initialize(migrate);
    tvShowStorage.initialize(migrate);
    saveCurrentStorageVersion();
}

std::optional<core::TvShow> DatabaseStorage::getTvShowByName(const std::string &tvShowName) {
    return tvShowStorage.getTvShowByName(tvShowName);
}

core::ListPage<core::TvShow> DatabaseStorage::getAllTvShows(unsigned int offset, unsigned int limit) {
    return core::ListPage<core::TvShow>(
            offset,
            tvShowStorage.countAllTvShows(),
            tvShowStorage.getAllTvShowsInAlphabeticOrder(offset, limit));
}

core::ListPage<core::TvShow> DatabaseStorage::getWatchedTvShows(unsigned int offset, unsigned int limit) {
    return core::ListPage<core::TvShow>(
            offset,
            tvShowStorage.countWatchedTvShows(),
            tvShowStorage.getWatchedTvShows(offset, limit));
}

core::ListPage<core::Episode> DatabaseStorage::getEpisodesOfTvShowWithName(const std::string &tvShowName,
                                                                           unsigned int offset, unsigned int limit) {
    return core::ListPage<core::Episode>(
            offset,
            tvShowStorage.countEpisodesOfTvShowWithName(tvShowName),
            tvShowStorage.getEpisodesOfTvShowWithName(tvShowName, offset, limit));
}

void DatabaseStorage::saveTvShow(const core::TvShow& tvShow, const std::vector<core::Episode>& episodes) {
    tvShowStorage.saveTvShow(tvShow, episodes);
}

std::optional<std::string> DatabaseStorage::getTvShowCrawlerByTvShowName(const std::string &tvShowName) {
    return tvShowCrawlerStorage.getTvShowCrawlerByTvShowName(tvShowName);
}

void DatabaseStorage::saveTvShowCrawler(const std::string &tvShowName, const std::string &serializedCrawler) {
    tvShowCrawlerStorage.deleteTvShowCrawlerOfTvShow(tvShowName);
    tvShowCrawlerStorage.insertTvShowCrawler(tvShowName, serializedCrawler);
}

std::string DatabaseStorage::getDatabaseFilePath() const {
    auto home = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    return QDir(home).filePath(".serio.db").toStdString();
}

void DatabaseStorage::openDatabaseConnection(const std::string& storageUrl) const {
    auto database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(QString::fromStdString(storageUrl));
    if (!database.open()) {
        throw StorageError(storageUrl, database.lastError().text().toStdString());
    }
}

void DatabaseStorage::enableForeignKeys() const {
    QSqlQuery enableForeignKeys(QSqlDatabase::database());
    enableForeignKeys.exec("PRAGMA foreign_keys=ON");
}

void DatabaseStorage::saveCrawlLog(const std::string &tvShowName, const std::vector<core::CrawlLogEntry> &log) {
    tvShowCrawlerLogStorage.saveCrawlLog(tvShowName, log);
}

std::vector<core::CrawlLogEntry> DatabaseStorage::getLastCrawlLogOfTvShow(const std::string &tvShowName) {
    return tvShowCrawlerLogStorage.getLastCrawlLogOfTvShow(tvShowName);
}

void DatabaseStorage::clearTvShowWatchHistory(const std::string &tvShowName) {
    tvShowStorage.clearTvShowWatchHistory(tvShowName);
}

void DatabaseStorage::deleteTvShow(const std::string &tvShowName) {
    tvShowStorage.deleteTvShowWithName(tvShowName);
}

void DatabaseStorage::deleteCrawlerOfTvShow(const std::string &tvShowName) {
    tvShowCrawlerStorage.deleteTvShowCrawlerOfTvShow(tvShowName);
}

std::optional<core::Episode> DatabaseStorage::getEpisodeOfTvShowWithName(const std::string &tvShowName,
                                                                         unsigned int episodeId) {
    return tvShowStorage.getEpisodeOfTvShowWithName(tvShowName, episodeId);
}

void DatabaseStorage::watchTvShowEpisode(const std::string &tvShowName, unsigned int episodeId,
                                         core::LastWatchDate watchDate, core::WatchProgress watchProgress) {
    tvShowStorage.watchTvShowEpisode(tvShowName, episodeId, watchDate, watchProgress);
}

std::optional<core::Episode> DatabaseStorage::getLastWatchedEpisodeOfTvShow(const std::string &tvShowName) {
    return tvShowStorage.getLastWatchedEpisodeOfTvShow(tvShowName);
}

bool DatabaseStorage::isSchemaMigrationRequired() const {
    auto getSchemaVersion = createAndExec("SELECT * FROM SERIO_VERSION");
    return !getSchemaVersion.next() || getSchemaVersion.value(0).toUInt() != version;
}

void DatabaseStorage::saveCurrentStorageVersion() const {
    createAndExec("CREATE TABLE IF NOT EXISTS SERIO_VERSION(VERSION INTEGER NOT NULL)");
    createAndExec("DELETE FROM SERIO_VERSION");
    createAndExec("INSERT INTO SERIO_VERSION VALUES (?)", version);
}

StorageError::StorageError(const std::string &databaseName, const std::string &reason)
    : std::runtime_error("Failed to open database '" + databaseName + "'. Reason: " + reason) {}

}