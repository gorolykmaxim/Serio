#ifndef SERIO_TVSHOWSTORAGE_H
#define SERIO_TVSHOWSTORAGE_H

#include <QSqlDatabase>
#include <string>
#include <tv-shows/TvShowsFacade.h>

namespace serio::qt {
    class TvShowStorage : public serio::core::TvShowsFacade {
    public:
        void initialize(const std::string& storageUrl);
        core::ListPage<core::TvShow> getAllTvShows(unsigned int offset, unsigned int limit) override;
        void saveTvShow(const core::TvShow& tvShow) override;
    private:
        void openDatabaseConnection(const std::string& storageUrl);
        void initializeStorage();
        unsigned int countAllTvShows();
        core::TvShow readTvShowFrom(const QSqlQuery& query);
        std::vector<core::TvShow> findAllTvShows(unsigned int offset, unsigned int limit);
        void deleteTvShowWithName(const std::string& name);
        void insertTvShow(const core::TvShow& tvShow);
    };
}

#endif //SERIO_TVSHOWSTORAGE_H
