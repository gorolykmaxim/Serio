#ifndef SERIO_TVSHOWSTORAGE_H
#define SERIO_TVSHOWSTORAGE_H

#include "TvShow.h"
#include "ListPage.h"
#include "Episode.h"

namespace serio::core {

class TvShowStorage {
public:
    virtual std::optional<TvShow> getTvShowByName(const std::string& tvShowName) = 0;
    virtual ListPage<TvShow> getAllTvShows(unsigned int offset, unsigned int limit) = 0;
    virtual ListPage<TvShow> getWatchedTvShows(unsigned int offset, unsigned int limit) = 0;
    virtual ListPage<Episode> getEpisodesOfTvShowWithName(const std::string& tvShowName, unsigned int offset, unsigned int limit) = 0;
    virtual void saveTvShow(const TvShow& tvShow, const std::vector<Episode>& episodes) = 0;
    virtual void clearTvShowWatchHistory(const std::string& tvShowName) = 0;
    virtual void deleteTvShow(const std::string& tvShowName) = 0;
    virtual std::optional<Episode> getEpisodeOfTvShowWithName(const std::string& tvShowName, unsigned int episodeId) = 0;
};

}

#endif //SERIO_TVSHOWSTORAGE_H
