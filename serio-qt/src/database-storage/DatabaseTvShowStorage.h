#ifndef SERIO_DATABASETVSHOWSTORAGE_H
#define SERIO_DATABASETVSHOWSTORAGE_H

#include <QSqlQuery>
#include <tv-show-storage/TvShow.h>
#include <tv-show-storage/Episode.h>
#include "BaseDatabaseStorage.h"
#include "Schema.h"

namespace serio::qt {

class DatabaseTvShowStorage : public BaseDatabaseStorage, public Schema {
public:
    void saveTvShow(const core::TvShow& tvShow, const std::vector<core::Episode>& episodes) const;
    [[nodiscard]] std::optional<core::TvShow> getTvShowByName(const std::string& tvShowName) const;
    [[nodiscard]] std::vector<core::TvShow> getAllTvShowsInAlphabeticOrder(unsigned int offset, unsigned int limit) const;
    [[nodiscard]] std::vector<core::TvShow> getWatchedTvShows(unsigned int offset, unsigned int limit) const;
    [[nodiscard]] std::vector<core::Episode> getEpisodesOfTvShowWithName(const std::string &tvShowName, unsigned int offset, unsigned int limit) const;
    [[nodiscard]] unsigned int countAllTvShows() const;
    [[nodiscard]] unsigned int countWatchedTvShows() const;
    [[nodiscard]] unsigned int countEpisodesOfTvShowWithName(const std::string& tvShowName) const;
    void clearTvShowWatchHistory(const std::string &tvShowName) const;
    void deleteTvShowWithName(const std::string& name) const;
    [[nodiscard]] std::optional<core::Episode> getEpisodeOfTvShowWithName(const std::string &tvShowName, unsigned int episodeId);
    void watchTvShowEpisode(const std::string &tvShowName, unsigned int episodeId, core::LastWatchDate watchDate,
                            core::WatchProgress watchProgress) const;
    std::optional<core::Episode> getLastWatchedEpisodeOfTvShow(const std::string &tvShowName);
protected:
    void backupOldVersion() const override;
    void createNewVersion() const override;
    void migrateRecordsFromOldVersion() const override;
    void dropOldVersion() const override;
private:
    const QString fromTvShow = "FROM TV_SHOW "
                               "LEFT OUTER JOIN EPISODE_VIEW ON TV_SHOW.NAME = EPISODE_VIEW.TV_SHOW_NAME ";
    void insertTvShow(const core::TvShow& tvShow) const;
    void insertEpisodes(const std::string& tvShowName, const std::vector<core::Episode>& episodes) const;
    [[nodiscard]] unsigned int countTvShowsMatchingQuery(const QString& query = "") const;
    [[nodiscard]] std::vector<core::TvShow> findTvShowsMatchingQuery(unsigned int offset, unsigned int limit,
                                                                     const QString& query = "", const QString& orderBy = "",
                                                                     const std::vector<QVariant>& values = {}) const;
    [[nodiscard]] core::TvShow readTvShowFrom(const QSqlQuery& query) const;
    [[nodiscard]] core::Episode readEpisodeFrom(const QSqlQuery& query) const;
    [[nodiscard]] std::optional<core::LastWatchDate> readLastWatchDate(const QVariant& variant) const;
    [[nodiscard]] std::vector<core::Episode> findEpisodesMatchingQuery(unsigned int offset, unsigned int limit,
                                                                       const QString& query,
                                                                       const QString& orderBy,
                                                                       const std::vector<QVariant>& values = {}) const;
};

}

#endif //SERIO_DATABASETVSHOWSTORAGE_H
