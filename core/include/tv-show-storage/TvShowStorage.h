#ifndef SERIO_TVSHOWSTORAGE_H
#define SERIO_TVSHOWSTORAGE_H

#include "TvShow.h"
#include "ListPage.h"

namespace serio::core {

class TvShowStorage {
public:
    virtual ListPage<TvShow> getAllTvShows(unsigned int offset, unsigned int limit) = 0;
    virtual ListPage<TvShow> getWatchedTvShows(unsigned int offset, unsigned int limit) = 0;
    virtual void saveTvShow(const TvShow& tvShow) = 0;
    virtual ~TvShowStorage() {};
};

}

#endif //SERIO_TVSHOWSTORAGE_H
