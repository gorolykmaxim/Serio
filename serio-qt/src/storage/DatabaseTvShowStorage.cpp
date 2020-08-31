#include <QSqlDatabase>
#include <QVariant>
#include "DatabaseTvShowStorage.h"

void serio::qt::DatabaseTvShowStorage::initialize() {
    QSqlQuery createTvShows(QSqlDatabase::database());
    createTvShows.exec("CREATE TABLE IF NOT EXISTS TV_SHOW(NAME TEXT PRIMARY KEY, THUMBNAIL_URL TEXT NOT NULL, LAST_WATCH_DATE BIGINT)");
}

serio::core::TvShow serio::qt::DatabaseTvShowStorage::readTvShowFrom(const QSqlQuery &query) const {
    std::string name = query.value(0).toString().toStdString();
    std::string thumbnailUrl = query.value(1).toString().toStdString();
    QVariant lastWatchDate = query.value(2);
    if (lastWatchDate.isNull()) {
        return core::TvShow(name, thumbnailUrl);
    } else {
        return core::TvShow(name, thumbnailUrl, lastWatchDate.toLongLong());
    }
}

unsigned int serio::qt::DatabaseTvShowStorage::countTvShowsMatchingQuery(const QString &query) {
    QSqlQuery countTvShows(QSqlDatabase::database());
    countTvShows.exec("SELECT COUNT() FROM TV_SHOW " + query);
    countTvShows.next();
    return countTvShows.value(0).toUInt();
}

std::vector<serio::core::TvShow> serio::qt::DatabaseTvShowStorage::findTvShowsMatchingQuery(const QString &query, unsigned int offset, unsigned int limit) {
    std::vector<core::TvShow> result;
    QSqlQuery findAllTvShows(QSqlDatabase::database());
    findAllTvShows.prepare("SELECT NAME, THUMBNAIL_URL, LAST_WATCH_DATE FROM TV_SHOW " + query + " LIMIT ? OFFSET ?");
    findAllTvShows.addBindValue(limit);
    findAllTvShows.addBindValue(offset);
    findAllTvShows.exec();
    while (findAllTvShows.next()) {
        result.push_back(readTvShowFrom(findAllTvShows));
    }
    return result;
}

void serio::qt::DatabaseTvShowStorage::deleteTvShowWithName(const std::string &name) {
    QSqlQuery removeShow(QSqlDatabase::database());
    removeShow.prepare("DELETE FROM TV_SHOW WHERE NAME = ?");
    removeShow.addBindValue(QString::fromStdString(name));
    removeShow.exec();
}

void serio::qt::DatabaseTvShowStorage::insertTvShow(const serio::core::TvShow &tvShow) {
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

unsigned int serio::qt::DatabaseTvShowStorage::countAllTvShows() {
    return countTvShowsMatchingQuery();
}

std::vector<serio::core::TvShow> serio::qt::DatabaseTvShowStorage::getAllTvShowsInAlphabeticOrder(unsigned int offset, unsigned int limit) {
    return findTvShowsMatchingQuery("ORDER BY NAME", offset, limit);
}

unsigned int serio::qt::DatabaseTvShowStorage::countWatchedTvShows() {
    return countTvShowsMatchingQuery("WHERE LAST_WATCH_DATE IS NOT NULL");
}

std::vector<serio::core::TvShow> serio::qt::DatabaseTvShowStorage::getWatchedTvShows(unsigned int offset, unsigned int limit) {
    return findTvShowsMatchingQuery("WHERE LAST_WATCH_DATE IS NOT NULL ORDER BY LAST_WATCH_DATE DESC", offset, limit);
}
