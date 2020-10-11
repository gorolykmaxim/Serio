#include "DatabaseStorage.h"
#include <QString>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>
#include <QStandardPaths>
#include <QDir>

void serio::qt::DatabaseStorage::initialize(bool inMemory) const {
    openDatabase(inMemory);
    initializeSchema();
}

void serio::qt::DatabaseStorage::openDatabase(bool inMemory) const {
    openDatabaseConnection(inMemory ? ":memory:" : getDatabaseFilePath());
}

void serio::qt::DatabaseStorage::initializeSchema() const {
    enableForeignKeys();
    bool migrate = isSchemaMigrationRequired();
    tvShowStorage.initialize(migrate);
    tvShowCrawlerStorage.initialize(migrate);
    tvShowCrawlerLogStorage.initialize(migrate);
    saveCurrentStorageVersion();
}

std::optional<serio::core::TvShow> serio::qt::DatabaseStorage::getTvShowByName(const std::string &tvShowName) {
    return tvShowStorage.getTvShowByName(tvShowName);
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
    auto home = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    return QDir(home).filePath(".serio.dev.db").toStdString();
}

void serio::qt::DatabaseStorage::openDatabaseConnection(const std::string& storageUrl) const {
    auto database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(QString::fromStdString(storageUrl));
    if (!database.open()) {
        throw StorageError(storageUrl, database.lastError().text().toStdString());
    }
}

void serio::qt::DatabaseStorage::enableForeignKeys() const {
    QSqlQuery enableForeignKeys(QSqlDatabase::database());
    enableForeignKeys.exec("PRAGMA foreign_keys=ON");
}

void serio::qt::DatabaseStorage::saveCrawlLog(const std::string &tvShowName, const std::vector<serio::core::CrawlLogEntry> &log) {
    tvShowCrawlerLogStorage.saveCrawlLog(tvShowName, log);
}

std::vector<serio::core::CrawlLogEntry> serio::qt::DatabaseStorage::getLastCrawlLogOfTvShow(const std::string &tvShowName) {
    return tvShowCrawlerLogStorage.getLastCrawlLogOfTvShow(tvShowName);
}

void serio::qt::DatabaseStorage::clearTvShowWatchHistory(const std::string &tvShowName) {
    tvShowStorage.clearTvShowWatchHistory(tvShowName);
}

void serio::qt::DatabaseStorage::deleteTvShow(const std::string &tvShowName) {
    tvShowStorage.deleteTvShowWithName(tvShowName);
}

void serio::qt::DatabaseStorage::deleteCrawlerOfTvShow(const std::string &tvShowName) {
    tvShowCrawlerStorage.deleteTvShowCrawlerOfTvShow(tvShowName);
}

std::optional<serio::core::Episode> serio::qt::DatabaseStorage::getEpisodeOfTvShowWithName(const std::string &tvShowName, unsigned int episodeId) {
    return tvShowStorage.getEpisodeOfTvShowWithName(tvShowName, episodeId);
}

void serio::qt::DatabaseStorage::watchTvShowEpisode(const std::string &tvShowName, unsigned int episodeId,
                                                    serio::core::LastWatchDate watchDate,
                                                    serio::core::WatchProgress watchProgress) {
    tvShowStorage.watchTvShowEpisode(tvShowName, episodeId, watchDate, watchProgress);
}

std::optional<serio::core::Episode> serio::qt::DatabaseStorage::getLastWatchedEpisodeOfTvShow(const std::string &tvShowName) {
    return tvShowStorage.getLastWatchedEpisodeOfTvShow(tvShowName);
}

bool serio::qt::DatabaseStorage::isSchemaMigrationRequired() const {
    auto getSchemaVersion = createAndExec("SELECT * FROM SERIO_VERSION");
    return !getSchemaVersion.next() || getSchemaVersion.value(0).toUInt() != version;
}

void serio::qt::DatabaseStorage::saveCurrentStorageVersion() const {
    createAndExec("CREATE TABLE IF NOT EXISTS SERIO_VERSION(VERSION INTEGER NOT NULL)");
    createAndExec("DELETE FROM SERIO_VERSION");
    createAndExec("INSERT INTO SERIO_VERSION VALUES (?)", version);
}

serio::qt::StorageError::StorageError(const std::string &databaseName, const std::string &reason)
    : std::runtime_error("Failed to open database '" + databaseName + "'. Reason: " + reason) {}
