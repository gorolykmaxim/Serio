#include <tv-show-viewer/TvShowViewer.h>

namespace serio::core {

TvShowViewer::TvShowViewer(TvShowStorage &tvShowStorage, TvShowCrawlerStorage& crawlerStorage,
                           TvShowCrawlerRuntime& runtime)
    : tvShowStorage(tvShowStorage), crawlerStorage(crawlerStorage), runtime(runtime) {}

TvShow TvShowViewer::getSelectedTvShow() const {
    auto tvShowName = getSelectedTvShowNameOrFail();
    auto tvShow = tvShowStorage.getTvShowByName(tvShowName);
    if (!tvShow) {
        throw DeletedTvShowStillSelectedError(tvShowName);
    } else {
        return *tvShow;
    }
}

ListPage<Episode> TvShowViewer::getTvShowEpisodes(unsigned int offset, unsigned int limit) {
    return tvShowStorage.getEpisodesOfTvShowWithName(getSelectedTvShowNameOrFail(), offset, limit);
}

void TvShowViewer::openTvShowWithName(const std::string &tvShowName) {
    if (!tvShowStorage.getTvShowByName(tvShowName)) {
        throw TvShowDoesNotExistError(tvShowName);
    }
    selectedTvShow = tvShowName;
}

std::string TvShowViewer::getRawCrawlerOfSelectedTvShow() {
    auto tvShowName = getSelectedTvShowNameOrFail();
    auto rawCrawler = crawlerStorage.getTvShowCrawlerByTvShowName(tvShowName);
    if (rawCrawler) {
        return *rawCrawler;
    } else {
        throw DeletedTvShowStillSelectedError(tvShowName);
    }
}

std::string TvShowViewer::getSelectedTvShowNameOrFail() const {
    if (!selectedTvShow) {
        throw NoTvShowSelectedError();
    }
    return *selectedTvShow;
}

void TvShowViewer::crawlSelectedTvShow() {
    runtime.crawlTvShow(getSelectedTvShowNameOrFail());
}

void TvShowViewer::clearSelectedTvShowWatchHistory() {
    tvShowStorage.clearTvShowWatchHistory(getSelectedTvShowNameOrFail());
}

void TvShowViewer::deleteSelectedTvShow() {
    auto tvShowName = getSelectedTvShowNameOrFail();
    tvShowStorage.deleteTvShow(tvShowName);
    crawlerStorage.deleteCrawlerOfTvShow(tvShowName);
}

NoTvShowSelectedError::NoTvShowSelectedError() : std::logic_error("No tv show selected") {}

TvShowDoesNotExistError::TvShowDoesNotExistError(const std::string &tvShowName)
    : logic_error("TV show '" + tvShowName + "' does not exist") {}

DeletedTvShowStillSelectedError::DeletedTvShowStillSelectedError(const std::string &tvShowName)
    : logic_error("TV show '" + tvShowName + "' has been deleted, but is still selected in tv-show-viewer") {}

}