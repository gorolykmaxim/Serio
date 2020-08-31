#include <QSqlDatabase>
#include <QVariant>
#include "DatabaseTvShowStorage.h"

void serio::qt::DatabaseTvShowStorage::initialize() {
    createTvShowTable();
    createEpisodeTable();
}

void serio::qt::DatabaseTvShowStorage::saveTvShow(const serio::core::TvShow &tvShow, const std::vector<core::Episode> &episodes) {
    deleteTvShowWithName(tvShow.getName());
    insertTvShow(tvShow);
    insertEpisodes(tvShow.getName(), episodes);
}

std::vector<serio::core::TvShow> serio::qt::DatabaseTvShowStorage::getAllTvShowsInAlphabeticOrder(unsigned int offset, unsigned int limit) {
    return findTvShowsMatchingQuery("ORDER BY NAME", offset, limit);
}

std::vector<serio::core::TvShow> serio::qt::DatabaseTvShowStorage::getWatchedTvShows(unsigned int offset, unsigned int limit) {
    return findTvShowsMatchingQuery("WHERE LAST_WATCH_DATE IS NOT NULL ORDER BY LAST_WATCH_DATE DESC", offset, limit);
}

std::vector<serio::core::Episode> serio::qt::DatabaseTvShowStorage::getEpisodesOfTvShowWithName(const std::string &tvShowName,
                                                                                                unsigned int offset,
                                                                                                unsigned int limit) {
    std::vector<core::Episode> result;
    QSqlQuery findEpisodesOfTvShow(QSqlDatabase::database());
    findEpisodesOfTvShow.prepare("SELECT ID, NAME, VIDEO_URL FROM EPISODE WHERE TV_SHOW_NAME = ? LIMIT ? OFFSET ?");
    findEpisodesOfTvShow.addBindValue(QString::fromStdString(tvShowName));
    findEpisodesOfTvShow.addBindValue(limit);
    findEpisodesOfTvShow.addBindValue(offset);
    findEpisodesOfTvShow.exec();
    while (findEpisodesOfTvShow.next()) {
        result.push_back(readEpisodeFrom(findEpisodesOfTvShow));
    }
    return result;
}

unsigned int serio::qt::DatabaseTvShowStorage::countAllTvShows() {
    return countTvShowsMatchingQuery();
}

unsigned int serio::qt::DatabaseTvShowStorage::countWatchedTvShows() {
    return countTvShowsMatchingQuery("WHERE LAST_WATCH_DATE IS NOT NULL");
}

unsigned int serio::qt::DatabaseTvShowStorage::countEpisodesOfTvShowWithName(const std::string &tvShowName) {
    QSqlQuery countEpisodes(QSqlDatabase::database());
    countEpisodes.prepare("SELECT COUNT() FROM EPISODE WHERE TV_SHOW_NAME = ?");
    countEpisodes.addBindValue(QString::fromStdString(tvShowName));
    countEpisodes.exec();
    countEpisodes.next();
    return countEpisodes.value(0).toUInt();
}

void serio::qt::DatabaseTvShowStorage::createTvShowTable() {
    QSqlQuery createTvShows(QSqlDatabase::database());
    createTvShows.exec("CREATE TABLE IF NOT EXISTS TV_SHOW("
                                "NAME TEXT PRIMARY KEY, "
                                "THUMBNAIL_URL TEXT NOT NULL, "
                                "LAST_WATCH_DATE BIGINT)");
}

void serio::qt::DatabaseTvShowStorage::createEpisodeTable() {
    QSqlQuery createEpisodes(QSqlDatabase::database());
    createEpisodes.exec("CREATE TABLE IF NOT EXISTS EPISODE("
                                "ID UNSIGNED BIG INT NOT NULL, "
                                "TV_SHOW_NAME TEXT NOT NULL, "
                                "NAME TEXT NOT NULL, "
                                "VIDEO_URL TEXT NOT NULL, "
                                "PRIMARY KEY(ID, TV_SHOW_NAME),"
                                "CONSTRAINT FK_TV_SHOW FOREIGN KEY (TV_SHOW_NAME) REFERENCES TV_SHOW(NAME) ON DELETE CASCADE)");
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

void serio::qt::DatabaseTvShowStorage::insertEpisodes(const std::string& tvShowName, const std::vector<core::Episode>& episodes) {
    QSqlQuery insertEpisodes(QSqlDatabase::database());
    insertEpisodes.prepare("INSERT INTO EPISODE VALUES(?, ?, ?, ?)");
    QString qTvShowName = QString::fromStdString(tvShowName);
    QVariantList ids, tvShowNames, names, videoUrls;
    for (const core::Episode& episode: episodes) {
        ids << QVariant(episode.getId());
        tvShowNames << qTvShowName;
        names << QString::fromStdString(episode.getName());
        videoUrls << QString::fromStdString(episode.getVideoUrl());
    }
    insertEpisodes.addBindValue(ids);
    insertEpisodes.addBindValue(tvShowNames);
    insertEpisodes.addBindValue(names);
    insertEpisodes.addBindValue(videoUrls);
    insertEpisodes.execBatch();
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

serio::core::Episode serio::qt::DatabaseTvShowStorage::readEpisodeFrom(const QSqlQuery &query) const {
    unsigned int id = query.value(0).toUInt();
    std::string name = query.value(1).toString().toStdString();
    std::string videoUrl = query.value(2).toString().toStdString();
    return core::Episode(id, videoUrl, name);
}
