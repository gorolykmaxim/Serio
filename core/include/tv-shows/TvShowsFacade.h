#ifndef SERIO_TVSHOWSFACADE_H
#define SERIO_TVSHOWSFACADE_H

#include "TvShow.h"
#include "ListPage.h"

namespace serio::core {
    class TvShowsFacade {
    public:
        virtual ListPage<TvShow> getAllTvShows(unsigned int offset, unsigned int limit) = 0;
        virtual void saveTvShow(const TvShow& tvShow) = 0;
        virtual ~TvShowsFacade() {};
    };
}

#endif //SERIO_TVSHOWSFACADE_H
