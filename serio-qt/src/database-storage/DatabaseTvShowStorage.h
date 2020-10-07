#ifndef SERIO_DATABASETVSHOWSTORAGE_H
#define SERIO_DATABASETVSHOWSTORAGE_H

#include <QSqlQuery>
#include <tv-show-storage/TvShow.h>
#include <tv-show-storage/Episode.h>
#include "BaseDatabaseStorage.h"

namespace serio::qt {

class DatabaseTvShowStorage : public BaseDatabaseStorage {
public:
    void initialize() const;
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
private:
    void createTvShowTable() const;
    void createEpisodeTable() const;
    void insertTvShow(const core::TvShow& tvShow) const;
    void insertEpisodes(const std::string& tvShowName, const std::vector<core::Episode>& episodes) const;
    [[nodiscard]] unsigned int countTvShowsMatchingQuery(const QString& query = "") const;
    [[nodiscard]] std::vector<core::TvShow> findTvShowsMatchingQuery(const QString& query, unsigned int offset, unsigned int limit, const std::vector<QVariant>& values = {}) const;
    [[nodiscard]] core::TvShow readTvShowFrom(const QSqlQuery& query) const;
    [[nodiscard]] core::Episode readEpisodeFrom(const QSqlQuery& query) const;
    [[nodiscard]] std::optional<core::LastWatchDate> readLastWatchDate(const QVariant& variant) const;
    void updateTvShowLastWatchDate(const QString& tvShowName, long long lastWatchDateTimeSinceEpoch) const;
    void updateEpisodeLastWatchDate(const QString& tvShowName, unsigned int episodeId,
                                    long long lastWatchDateTimeSinceEpoch, double watchProgress) const;
};

}

#endif //SERIO_DATABASETVSHOWSTORAGE_H
