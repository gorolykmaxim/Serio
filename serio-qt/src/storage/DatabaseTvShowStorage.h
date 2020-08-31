#ifndef SERIO_DATABASETVSHOWSTORAGE_H
#define SERIO_DATABASETVSHOWSTORAGE_H

#include <QSqlDatabase>
#include <string>
#include <tv-show-storage/TvShowStorage.h>

namespace serio::qt {

class DatabaseTvShowStorage : public serio::core::TvShowStorage {
public:
    void initialize(const std::string& storageUrl);
    core::ListPage<core::TvShow> getAllTvShows(unsigned int offset, unsigned int limit) override;
    core::ListPage<core::TvShow> getWatchedTvShows(unsigned int offset, unsigned int limit) override;
    void saveTvShow(const core::TvShow& tvShow) override;
private:
    void openDatabaseConnection(const std::string& storageUrl);
    void initializeStorage();
    unsigned int countTvShowsMatchingQuery(const QString& query = "");
    core::TvShow readTvShowFrom(const QSqlQuery& query);
    std::vector<core::TvShow> findTvShowsMatchingQuery(const QString& query, unsigned int offset, unsigned int limit);
    void deleteTvShowWithName(const std::string& name);
    void insertTvShow(const core::TvShow& tvShow);
};

}

#endif //SERIO_DATABASETVSHOWSTORAGE_H
