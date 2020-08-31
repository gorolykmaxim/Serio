#ifndef SERIO_DATABASETVSHOWSTORAGE_H
#define SERIO_DATABASETVSHOWSTORAGE_H

#include <QSqlQuery>
#include <tv-show-storage/TvShow.h>

namespace serio::qt {

class DatabaseTvShowStorage {
public:
    void initialize();
    void deleteTvShowWithName(const std::string& name);
    void insertTvShow(const core::TvShow& tvShow);
    std::vector<core::TvShow> getAllTvShowsInAlphabeticOrder(unsigned int offset, unsigned int limit);
    std::vector<core::TvShow> getWatchedTvShows(unsigned int offset, unsigned int limit);
    unsigned int countAllTvShows();
    unsigned int countWatchedTvShows();
private:
    unsigned int countTvShowsMatchingQuery(const QString& query = "");
    core::TvShow readTvShowFrom(const QSqlQuery& query) const;
    std::vector<core::TvShow> findTvShowsMatchingQuery(const QString& query, unsigned int offset, unsigned int limit);
};

}

#endif //SERIO_DATABASETVSHOWSTORAGE_H
