#ifndef SERIO_TVSHOWVIEWER_H
#define SERIO_TVSHOWVIEWER_H

#include <tv-show-storage/TvShowStorage.h>

namespace serio::core {

class TvShowViewer {
public:
    explicit TvShowViewer(TvShowStorage& storage);
    [[nodiscard]] virtual TvShow getSelectedTvShow() const;
    [[nodiscard]] virtual ListPage<Episode> getTvShowEpisodes(unsigned int offset, unsigned int limit);
    virtual void openTvShowWithName(const std::string& tvShowName);
private:
    std::optional<TvShow> selectedTvShow;
    TvShowStorage& storage;
};

class NoTvShowSelectedError : public std::logic_error {
public:
    NoTvShowSelectedError();
};

}

#endif //SERIO_TVSHOWVIEWER_H
