#include <QSqlDatabase>
#include <QVariant>
#include "DatabaseTvShowStorage.h"

void serio::qt::DatabaseTvShowStorage::initialize() const {
    createTvShowTable();
    createEpisodeTable();
    createEpisodeViewTable();
}

void serio::qt::DatabaseTvShowStorage::saveTvShow(const serio::core::TvShow &tvShow, const std::vector<core::Episode> &episodes) const {
    deleteTvShowWithName(tvShow.getName());
    insertTvShow(tvShow);
    insertEpisodes(tvShow.getName(), episodes);
}

std::optional<serio::core::TvShow> serio::qt::DatabaseTvShowStorage::getTvShowByName(const std::string &tvShowName) const {
    std::vector<serio::core::TvShow> tvShows = findTvShowsMatchingQuery("WHERE NAME = ?", 0, 1, {QString::fromStdString(tvShowName)});
    return tvShows.empty() ? std::optional<serio::core::TvShow>() : tvShows[0];
}

std::vector<serio::core::TvShow> serio::qt::DatabaseTvShowStorage::getAllTvShowsInAlphabeticOrder(unsigned int offset, unsigned int limit) const {
    return findTvShowsMatchingQuery("ORDER BY NAME", offset, limit);
}

std::vector<serio::core::TvShow> serio::qt::DatabaseTvShowStorage::getWatchedTvShows(unsigned int offset, unsigned int limit) const {
    return findTvShowsMatchingQuery("WHERE LAST_WATCH_DATE IS NOT NULL ORDER BY LAST_WATCH_DATE DESC", offset, limit);
}

std::vector<serio::core::Episode> serio::qt::DatabaseTvShowStorage::getEpisodesOfTvShowWithName(const std::string &tvShowName,
                                                                                                unsigned int offset,
                                                                                                unsigned int limit) const {
    return findEpisodesMatchingQuery("WHERE EPISODE.TV_SHOW_NAME = ?", offset, limit, {QString::fromStdString(tvShowName)});
}

unsigned int serio::qt::DatabaseTvShowStorage::countAllTvShows() const {
    return countTvShowsMatchingQuery();
}

unsigned int serio::qt::DatabaseTvShowStorage::countWatchedTvShows() const {
    return countTvShowsMatchingQuery("WHERE LAST_WATCH_DATE IS NOT NULL");
}

unsigned int serio::qt::DatabaseTvShowStorage::countEpisodesOfTvShowWithName(const std::string &tvShowName) const {
    QSqlQuery countEpisodes = createAndExec("SELECT COUNT() FROM EPISODE WHERE TV_SHOW_NAME = ?",
                                            QString::fromStdString(tvShowName));
    countEpisodes.next();
    return countEpisodes.value(0).toUInt();
}

void serio::qt::DatabaseTvShowStorage::createTvShowTable() const {
    QSqlQuery createTvShows(QSqlDatabase::database());
    createTvShows.exec("CREATE TABLE IF NOT EXISTS TV_SHOW("
                                "NAME TEXT PRIMARY KEY, "
                                "THUMBNAIL_URL TEXT NOT NULL)");
}

void serio::qt::DatabaseTvShowStorage::createEpisodeTable() const {
    QSqlQuery createEpisodes(QSqlDatabase::database());
    createEpisodes.exec("CREATE TABLE IF NOT EXISTS EPISODE("
                                "ID UNSIGNED BIG INT NOT NULL, "
                                "TV_SHOW_NAME TEXT NOT NULL, "
                                "NAME TEXT NOT NULL, "
                                "VIDEO_URL TEXT NOT NULL, "
                                "PRIMARY KEY(ID, TV_SHOW_NAME),"
                                "CONSTRAINT FK_TV_SHOW FOREIGN KEY (TV_SHOW_NAME) REFERENCES TV_SHOW(NAME) ON DELETE CASCADE)");
}

void serio::qt::DatabaseTvShowStorage::createEpisodeViewTable() const {
    QSqlQuery createEpisodeViews(QSqlDatabase::database());
    createEpisodeViews.exec("CREATE TABLE IF NOT EXISTS EPISODE_VIEW("
                                    "TV_SHOW_NAME TEXT NOT NULL, "
                                    "EPISODE_ID UNSIGNED BIG INT NOT NULL, "
                                    "LAST_WATCH_DATE BIGINT NOT NULL, "
                                    "WATCH_PROGRESS REAL NOT NULL, "
                                    "PRIMARY KEY(TV_SHOW_NAME, EPISODE_ID))");
}

void serio::qt::DatabaseTvShowStorage::deleteTvShowWithName(const std::string &name) const {
    createAndExec("DELETE FROM TV_SHOW WHERE NAME = ?", QString::fromStdString(name));
}

void serio::qt::DatabaseTvShowStorage::insertTvShow(const serio::core::TvShow &tvShow) const {
    QSqlQuery addShow(QSqlDatabase::database());
    addShow.prepare("INSERT INTO TV_SHOW VALUES(?, ?)");
    addShow.addBindValue(QString::fromStdString(tvShow.getName()));
    addShow.addBindValue(QString::fromStdString(tvShow.getThumbnailUrl()));
    addShow.exec();
}

void serio::qt::DatabaseTvShowStorage::insertEpisodes(const std::string& tvShowName, const std::vector<core::Episode>& episodes) const {
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

unsigned int serio::qt::DatabaseTvShowStorage::countTvShowsMatchingQuery(const QString &query) const {
    QSqlQuery countTvShows(QSqlDatabase::database());
    countTvShows.exec("SELECT COUNT() "
                      + fromTvShow + query);
    countTvShows.next();
    return countTvShows.value(0).toUInt();
}

std::vector<serio::core::TvShow> serio::qt::DatabaseTvShowStorage::findTvShowsMatchingQuery(const QString &query,
                                                                                            unsigned int offset,
                                                                                            unsigned int limit,
                                                                                            const std::vector<QVariant>& values) const {
    std::vector<core::TvShow> result;
    QSqlQuery findAllTvShows(QSqlDatabase::database());
    findAllTvShows.prepare("SELECT NAME, THUMBNAIL_URL, LAST_WATCH_DATE "
                           + fromTvShow +  query +
                           " LIMIT ? OFFSET ?");
    for (const auto& value: values) {
        findAllTvShows.addBindValue(value);
    }
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
    std::optional<serio::core::LastWatchDate> lastWatchDate = readLastWatchDate(query.value(2));
    return serio::core::TvShow(name, thumbnailUrl, lastWatchDate);
}

serio::core::Episode serio::qt::DatabaseTvShowStorage::readEpisodeFrom(const QSqlQuery &query) const {
    unsigned int id = query.value(0).toUInt();
    std::string name = query.value(1).toString().toStdString();
    std::string videoUrl = query.value(2).toString().toStdString();
    std::optional<serio::core::LastWatchDate> lastWatchDate = readLastWatchDate(query.value(3));
    serio::core::WatchProgress watchProgress(query.value(4).toDouble());
    return serio::core::Episode(id, videoUrl, name, lastWatchDate, watchProgress);
}

std::optional<serio::core::LastWatchDate> serio::qt::DatabaseTvShowStorage::readLastWatchDate(const QVariant &variant) const {
    return variant.isNull() ? std::optional<serio::core::LastWatchDate>() : serio::core::LastWatchDate(variant.toLongLong());
}

void serio::qt::DatabaseTvShowStorage::clearTvShowWatchHistory(const std::string &tvShowName) const {
    createAndExec("DELETE FROM EPISODE_VIEW WHERE TV_SHOW_NAME = ?", QString::fromStdString(tvShowName));
}

std::optional<serio::core::Episode> serio::qt::DatabaseTvShowStorage::getEpisodeOfTvShowWithName(const std::string &tvShowName, unsigned int episodeId) {
    auto episodes = findEpisodesMatchingQuery("WHERE EPISODE.TV_SHOW_NAME = ? AND ID = ?", 0, 1, {QString::fromStdString(tvShowName), episodeId});
    return episodes.empty() ? std::optional<serio::core::Episode>() : episodes[0];
}

void serio::qt::DatabaseTvShowStorage::watchTvShowEpisode(const std::string &tvShowName, unsigned int episodeId,
                                                          serio::core::LastWatchDate watchDate,
                                                          serio::core::WatchProgress watchProgress) const {
    QSqlQuery insertEpisodeView(QSqlDatabase::database());
    insertEpisodeView.prepare("INSERT INTO EPISODE_VIEW VALUES(?, ?, ?, ?)");
    insertEpisodeView.addBindValue(QString::fromStdString(tvShowName));
    insertEpisodeView.addBindValue(episodeId);
    insertEpisodeView.addBindValue(watchDate.getSinceEpoch());
    insertEpisodeView.addBindValue(watchProgress.getPercentage());
    insertEpisodeView.exec();
}

std::vector<serio::core::Episode> serio::qt::DatabaseTvShowStorage::findEpisodesMatchingQuery(const QString &query,
                                                                                              unsigned int offset,
                                                                                              unsigned int limit,
                                                                                              const std::vector<QVariant> &values) const {
    std::vector<core::Episode> result;
    QSqlQuery findEpisodes(QSqlDatabase::database());
    findEpisodes.prepare("SELECT ID, NAME, VIDEO_URL, LAST_WATCH_DATE, WATCH_PROGRESS "
                                 "FROM EPISODE "
                                 "LEFT OUTER JOIN EPISODE_VIEW ON (EPISODE.ID = EPISODE_VIEW.EPISODE_ID AND EPISODE.TV_SHOW_NAME = EPISODE_VIEW.TV_SHOW_NAME) "
                                 + query +
                                 " ORDER BY ID LIMIT ? OFFSET ?");
    for (const auto& value: values) {
        findEpisodes.addBindValue(value);
    }
    findEpisodes.addBindValue(limit);
    findEpisodes.addBindValue(offset);
    findEpisodes.exec();
    while (findEpisodes.next()) {
        result.push_back(readEpisodeFrom(findEpisodes));
    }
    return result;
}
