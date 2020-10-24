#include <QSqlDatabase>
#include <QVariant>
#include "DatabaseTvShowStorage.h"

namespace serio::qt {

void DatabaseTvShowStorage::saveTvShow(const core::TvShow &tvShow, const std::vector<core::Episode> &episodes) const {
    deleteTvShowWithName(tvShow.getName());
    insertTvShow(tvShow);
    insertEpisodes(tvShow.getName(), episodes);
}

std::optional<core::TvShow> DatabaseTvShowStorage::getTvShowByName(const std::string &tvShowName) const {
    auto tvShows = findTvShowsMatchingQuery(0, 1, "WHERE NAME = ?", "",
                                            {QString::fromStdString(tvShowName)});
    return tvShows.empty() ? std::optional<core::TvShow>() : tvShows[0];
}

std::vector<core::TvShow> DatabaseTvShowStorage::getAllTvShowsInAlphabeticOrder(unsigned int offset, unsigned int limit) const {
    return findTvShowsMatchingQuery(offset, limit, "", "ORDER BY NAME");
}

std::vector<core::TvShow> DatabaseTvShowStorage::getWatchedTvShows(unsigned int offset, unsigned int limit) const {
    return findTvShowsMatchingQuery(offset, limit, "WHERE LAST_WATCH_DATE IS NOT NULL",
                                    "ORDER BY LAST_WATCH_DATE DESC");
}

std::vector<core::Episode> DatabaseTvShowStorage::getEpisodesOfTvShowWithName(const std::string &tvShowName,
                                                                              unsigned int offset,
                                                                              unsigned int limit) const {
    return findEpisodesMatchingQuery(
            offset,
            limit,
            "WHERE EPISODE.TV_SHOW_NAME = ?",
            "ORDER BY ID",
            {QString::fromStdString(tvShowName)});
}

unsigned int DatabaseTvShowStorage::countAllTvShows() const {
    return countTvShowsMatchingQuery();
}

unsigned int DatabaseTvShowStorage::countWatchedTvShows() const {
    return countTvShowsMatchingQuery("WHERE LAST_WATCH_DATE IS NOT NULL");
}

unsigned int DatabaseTvShowStorage::countEpisodesOfTvShowWithName(const std::string &tvShowName) const {
    auto countEpisodes = createAndExec("SELECT COUNT() FROM EPISODE WHERE TV_SHOW_NAME = ?",
                                       QString::fromStdString(tvShowName));
    countEpisodes.next();
    return countEpisodes.value(0).toUInt();
}

void DatabaseTvShowStorage::deleteTvShowWithName(const std::string &name) const {
    createAndExec("DELETE FROM TV_SHOW WHERE NAME = ?", QString::fromStdString(name));
}

void DatabaseTvShowStorage::insertTvShow(const core::TvShow &tvShow) const {
    QSqlQuery addShow(QSqlDatabase::database());
    addShow.prepare("INSERT INTO TV_SHOW VALUES(?, ?)");
    addShow.addBindValue(QString::fromStdString(tvShow.getName()));
    addShow.addBindValue(QString::fromStdString(tvShow.getThumbnailUrl()));
    addShow.exec();
}

void DatabaseTvShowStorage::insertEpisodes(const std::string& tvShowName, const std::vector<core::Episode>& episodes) const {
    QSqlQuery insertEpisodes(QSqlDatabase::database());
    insertEpisodes.prepare("INSERT INTO EPISODE VALUES(?, ?, ?, ?)");
    auto qTvShowName = QString::fromStdString(tvShowName);
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

unsigned int DatabaseTvShowStorage::countTvShowsMatchingQuery(const QString &query) const {
    auto countTvShows = createAndExec("SELECT COUNT(DISTINCT NAME) " + fromTvShow + query);
    countTvShows.next();
    return countTvShows.value(0).toUInt();
}

std::vector<core::TvShow> DatabaseTvShowStorage::findTvShowsMatchingQuery(unsigned int offset, unsigned int limit,
                                                                          const QString &query, const QString &orderBy,
                                                                          const std::vector<QVariant>& values) const {
    std::vector<core::TvShow> result;
    QSqlQuery findAllTvShows(QSqlDatabase::database());
    findAllTvShows.prepare("SELECT NAME, THUMBNAIL_URL, MAX(LAST_WATCH_DATE) "
                           + fromTvShow + query +
                           " GROUP BY NAME "
                           + orderBy +
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

core::TvShow DatabaseTvShowStorage::readTvShowFrom(const QSqlQuery &query) const {
    auto name = query.value(0).toString().toStdString();
    auto thumbnailUrl = query.value(1).toString().toStdString();
    auto lastWatchDate = readLastWatchDate(query.value(2));
    return core::TvShow(name, thumbnailUrl, lastWatchDate);
}

core::Episode DatabaseTvShowStorage::readEpisodeFrom(const QSqlQuery &query) const {
    unsigned int id = query.value(0).toUInt();
    auto name = query.value(1).toString().toStdString();
    auto videoUrl = query.value(2).toString().toStdString();
    auto lastWatchDate = readLastWatchDate(query.value(3));
    core::WatchProgress watchProgress(query.value(4).toDouble());
    return core::Episode(id, videoUrl, name, lastWatchDate, watchProgress);
}

std::optional<core::LastWatchDate> DatabaseTvShowStorage::readLastWatchDate(const QVariant &variant) const {
    return variant.isNull() ? std::optional<core::LastWatchDate>() : core::LastWatchDate(variant.toLongLong());
}

void DatabaseTvShowStorage::clearTvShowWatchHistory(const std::string &tvShowName) const {
    createAndExec("DELETE FROM EPISODE_VIEW WHERE TV_SHOW_NAME = ?", QString::fromStdString(tvShowName));
}

std::optional<core::Episode> DatabaseTvShowStorage::getEpisodeOfTvShowWithName(const std::string &tvShowName,
                                                                               unsigned int episodeId) {
    auto episodes = findEpisodesMatchingQuery(
            0,
            1,
            "WHERE EPISODE.TV_SHOW_NAME = ? AND ID = ?",
            "ORDER BY ID",
            {QString::fromStdString(tvShowName), episodeId});
    return episodes.empty() ? std::optional<core::Episode>() : episodes[0];
}

void DatabaseTvShowStorage::watchTvShowEpisode(const std::string &tvShowName, unsigned int episodeId,
                                               core::LastWatchDate watchDate, core::WatchProgress watchProgress) const {
    QSqlQuery insertEpisodeView(QSqlDatabase::database());
    insertEpisodeView.prepare("INSERT OR REPLACE INTO EPISODE_VIEW VALUES(?, ?, ?, ?)");
    insertEpisodeView.addBindValue(QString::fromStdString(tvShowName));
    insertEpisodeView.addBindValue(episodeId);
    insertEpisodeView.addBindValue(watchDate.getSinceEpoch());
    insertEpisodeView.addBindValue(watchProgress.getPercentage());
    insertEpisodeView.exec();
}

std::vector<core::Episode> DatabaseTvShowStorage::findEpisodesMatchingQuery(unsigned int offset, unsigned int limit,
                                                                            const QString &query,
                                                                            const QString &orderBy,
                                                                            const std::vector<QVariant> &values) const {
    std::vector<core::Episode> result;
    QSqlQuery findEpisodes(QSqlDatabase::database());
    findEpisodes.prepare("SELECT ID, NAME, VIDEO_URL, LAST_WATCH_DATE, WATCH_PROGRESS "
                         "FROM EPISODE "
                         "LEFT OUTER JOIN EPISODE_VIEW ON (EPISODE.ID = EPISODE_VIEW.EPISODE_ID AND EPISODE.TV_SHOW_NAME = EPISODE_VIEW.TV_SHOW_NAME) "
                         + query + " " + orderBy +
                         " LIMIT ? OFFSET ?");
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

std::optional<core::Episode> DatabaseTvShowStorage::getLastWatchedEpisodeOfTvShow(const std::string &tvShowName) {
    auto episodes = findEpisodesMatchingQuery(
            0,
            1,
            "WHERE EPISODE.TV_SHOW_NAME = ? AND LAST_WATCH_DATE IS NOT NULL",
            "ORDER BY LAST_WATCH_DATE DESC",
            {QString::fromStdString(tvShowName)});
    return episodes.empty() ? std::optional<core::Episode>() : episodes[0];
}

void DatabaseTvShowStorage::backupOldVersion() const {
    createAndExec("ALTER TABLE EPISODE RENAME TO OLD_EPISODE");
    createAndExec("ALTER TABLE EPISODE_VIEW RENAME TO OLD_EPISODE_VIEW");
}

void DatabaseTvShowStorage::createNewVersion() const {
    createAndExec("CREATE TABLE IF NOT EXISTS TV_SHOW("
                  "NAME TEXT NOT NULL PRIMARY KEY, "
                  "THUMBNAIL_URL TEXT NOT NULL)");
    createAndExec("CREATE TABLE IF NOT EXISTS EPISODE("
                  "ID UNSIGNED BIG INT NOT NULL, "
                  "TV_SHOW_NAME TEXT NOT NULL, "
                  "NAME TEXT NOT NULL, "
                  "VIDEO_URL TEXT NOT NULL, "
                  "PRIMARY KEY(ID, TV_SHOW_NAME),"
                  "CONSTRAINT FK_TV_SHOW FOREIGN KEY (TV_SHOW_NAME) REFERENCES TV_SHOW(NAME) ON DELETE CASCADE)");
    createAndExec("CREATE TABLE IF NOT EXISTS EPISODE_VIEW("
                  "TV_SHOW_NAME TEXT NOT NULL, "
                  "EPISODE_ID UNSIGNED BIG INT NOT NULL, "
                  "LAST_WATCH_DATE BIGINT NOT NULL, "
                  "WATCH_PROGRESS REAL NOT NULL, "
                  "PRIMARY KEY(TV_SHOW_NAME, EPISODE_ID))");
}

void DatabaseTvShowStorage::migrateRecordsFromOldVersion() const {
    createAndExec("INSERT INTO TV_SHOW SELECT NAME, THUMBNAIL_URL FROM SHOW");
    createAndExec("INSERT INTO EPISODE "
                  "SELECT E.ID, S.NAME, E.NAME, E.VIDEO_URL "
                  "FROM OLD_EPISODE E "
                  "LEFT JOIN SHOW S ON E.SHOW_ID = S.ID");
    createAndExec("INSERT INTO EPISODE_VIEW "
                  "SELECT S.NAME, EV.EPISODE_ID, EV.LAST_WATCH_DATE * 10000, EV.PROGRESS "
                  "FROM OLD_EPISODE_VIEW EV "
                  "LEFT JOIN SHOW S ON EV.SHOW_ID = S.ID");
}

void DatabaseTvShowStorage::dropOldVersion() const {
    createAndExec("DROP TABLE SHOW");
    createAndExec("DROP TABLE OLD_EPISODE");
    createAndExec("DROP TABLE OLD_EPISODE_VIEW");
}

}