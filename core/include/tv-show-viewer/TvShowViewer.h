#ifndef SERIO_TVSHOWVIEWER_H
#define SERIO_TVSHOWVIEWER_H

#include <tv-show-storage/TvShowStorage.h>
#include <stdexcept>
#include <tv-show-crawler-storage/TvShowCrawlerStorage.h>

namespace serio::core {

class TvShowViewer {
public:
    TvShowViewer(TvShowStorage& tvShowStorage, TvShowCrawlerStorage& crawlerStorage);
    [[nodiscard]] virtual TvShow getSelectedTvShow() const;
    [[nodiscard]] virtual ListPage<Episode> getTvShowEpisodes(unsigned int offset, unsigned int limit);
    virtual void openTvShowWithName(const std::string& tvShowName);
    virtual std::string getRawCrawlerOfSelectedTvShow();
private:
    std::optional<std::string> selectedTvShow;
    TvShowStorage& tvShowStorage;
    TvShowCrawlerStorage& crawlerStorage;
    [[nodiscard]] std::string getSelectedTvShowNameOrFail() const;
};

class NoTvShowSelectedError : public std::logic_error {
public:
    NoTvShowSelectedError();
};

class TvShowDoesNotExistError : public std::logic_error {
public:
    explicit TvShowDoesNotExistError(const std::string& tvShowName);
};

class DeletedTvShowStillSelectedError : public std::logic_error {
public:
    explicit DeletedTvShowStillSelectedError(const std::string& tvShowName);
};

}

#endif //SERIO_TVSHOWVIEWER_H
