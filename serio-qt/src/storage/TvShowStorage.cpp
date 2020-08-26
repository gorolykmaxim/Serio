#include "TvShowStorage.h"
#include "StorageError.h"
#include <QString>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>

void serio::qt::TvShowStorage::initialize(const std::string &storageUrl) {
    openDatabaseConnection(storageUrl);
    initializeStorage();
}

void serio::qt::TvShowStorage::openDatabaseConnection(const std::string& storageUrl) {
    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(QString::fromStdString(storageUrl));
    if (!database.open()) {
        throw StorageError::openDatabase(storageUrl, database.lastError().text().toStdString());
    }
}

void serio::qt::TvShowStorage::initializeStorage() {
    QSqlQuery createTvShows(QSqlDatabase::database());
    createTvShows.exec("CREATE TABLE IF NOT EXISTS TV_SHOW(NAME TEXT PRIMARY KEY, THUMBNAIL_URL TEXT NOT NULL, LAST_WATCH_DATE BIGINT)");
}

serio::core::ListPage<serio::core::TvShow> serio::qt::TvShowStorage::getAllTvShows(unsigned int offset, unsigned int limit) {
    return core::ListPage<serio::core::TvShow>(offset, countAllTvShows(), findAllTvShows(offset, limit));
}

void serio::qt::TvShowStorage::saveTvShow(const core::TvShow& tvShow) {
    deleteTvShowWithName(tvShow.getName());
    insertTvShow(tvShow);
}

unsigned int serio::qt::TvShowStorage::countAllTvShows() {
    QSqlQuery countTvShows(QSqlDatabase::database());
    countTvShows.exec("SELECT COUNT() FROM TV_SHOW");
    countTvShows.next();
    return countTvShows.value(0).toUInt();
}

serio::core::TvShow serio::qt::TvShowStorage::readTvShowFrom(const QSqlQuery &query) {
    std::string name = query.value(0).toString().toStdString();
    std::string thumbnailUrl = query.value(1).toString().toStdString();
    QVariant lastWatchDate = query.value(2);
    if (lastWatchDate.isNull()) {
        return core::TvShow(name, thumbnailUrl);
    } else {
        return core::TvShow(name, thumbnailUrl, lastWatchDate.toLongLong());
    }
}

std::vector<serio::core::TvShow> serio::qt::TvShowStorage::findAllTvShows(unsigned int offset, unsigned int limit) {
    std::vector<core::TvShow> result;
    QSqlQuery findAllTvShows(QSqlDatabase::database());
    findAllTvShows.prepare("SELECT NAME, THUMBNAIL_URL, LAST_WATCH_DATE FROM TV_SHOW ORDER BY NAME LIMIT ? OFFSET ?");
    findAllTvShows.addBindValue(limit);
    findAllTvShows.addBindValue(offset);
    findAllTvShows.exec();
    while (findAllTvShows.next()) {
        result.push_back(readTvShowFrom(findAllTvShows));
    }
    return result;
}

void serio::qt::TvShowStorage::deleteTvShowWithName(const std::string &name) {
    QSqlQuery removeShow(QSqlDatabase::database());
    removeShow.prepare("DELETE FROM TV_SHOW WHERE NAME = ?");
    removeShow.addBindValue(QString::fromStdString(name));
    removeShow.exec();
}

void serio::qt::TvShowStorage::insertTvShow(const serio::core::TvShow &tvShow) {
    QSqlQuery addShow(QSqlDatabase::database());
    addShow.prepare("INSERT INTO TV_SHOW VALUES(?, ?, ?)");
    addShow.addBindValue(QString::fromStdString(tvShow.getName()));
    addShow.addBindValue(QString::fromStdString(tvShow.getThumbnailUrl()));
    if (tvShow.getLastWatchDate()) {
        addShow.addBindValue(QVariant(tvShow.getLastWatchDate()->getSinceEpoch()));
    } else {
        addShow.addBindValue(QVariant());
    }
    addShow.exec();
}
