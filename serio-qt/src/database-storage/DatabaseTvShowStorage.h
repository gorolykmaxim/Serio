#ifndef SERIO_DATABASETVSHOWSTORAGE_H
#define SERIO_DATABASETVSHOWSTORAGE_H

#include <QSqlQuery>
#include <tv-show-storage/TvShow.h>
#include <tv-show-storage/Episode.h>

namespace serio::qt {

class DatabaseTvShowStorage {
public:
    void initialize();
    void saveTvShow(const core::TvShow& tvShow, const std::vector<core::Episode>& episodes);
    std::optional<core::TvShow> getTvShowByName(const std::string& tvShowName);
    std::vector<core::TvShow> getAllTvShowsInAlphabeticOrder(unsigned int offset, unsigned int limit);
    std::vector<core::TvShow> getWatchedTvShows(unsigned int offset, unsigned int limit);
    std::vector<core::Episode> getEpisodesOfTvShowWithName(const std::string &tvShowName, unsigned int offset, unsigned int limit);
    unsigned int countAllTvShows();
    unsigned int countWatchedTvShows();
    unsigned int countEpisodesOfTvShowWithName(const std::string& tvShowName);
private:
    void createTvShowTable();
    void createEpisodeTable();
    void deleteTvShowWithName(const std::string& name);
    void insertTvShow(const core::TvShow& tvShow);
    void insertEpisodes(const std::string& tvShowName, const std::vector<core::Episode>& episodes);
    unsigned int countTvShowsMatchingQuery(const QString& query = "");
    std::vector<core::TvShow> findTvShowsMatchingQuery(const QString& query, unsigned int offset, unsigned int limit, const std::vector<QVariant>& values = {});
    [[nodiscard]] core::TvShow readTvShowFrom(const QSqlQuery& query) const;
    [[nodiscard]] core::Episode readEpisodeFrom(const QSqlQuery& query) const;
};

}

#endif //SERIO_DATABASETVSHOWSTORAGE_H
