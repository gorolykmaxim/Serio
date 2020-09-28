#include <tv-show-viewer/TvShowViewer.h>

serio::core::TvShowViewer::TvShowViewer(serio::core::TvShowStorage &tvShowStorage,
                                        serio::core::TvShowCrawlerStorage& crawlerStorage,
                                        serio::core::TvShowCrawlerRuntime& runtime)
    : tvShowStorage(tvShowStorage), crawlerStorage(crawlerStorage), runtime(runtime) {}

serio::core::TvShow serio::core::TvShowViewer::getSelectedTvShow() const {
    std::string tvShowName = getSelectedTvShowNameOrFail();
    std::optional<serio::core::TvShow> tvShow = tvShowStorage.getTvShowByName(tvShowName);
    if (!tvShow) {
        throw DeletedTvShowStillSelectedError(tvShowName);
    } else {
        return *tvShow;
    }
}

serio::core::ListPage<serio::core::Episode> serio::core::TvShowViewer::getTvShowEpisodes(unsigned int offset, unsigned int limit) {
    return tvShowStorage.getEpisodesOfTvShowWithName(getSelectedTvShowNameOrFail(), offset, limit);
}

void serio::core::TvShowViewer::openTvShowWithName(const std::string &tvShowName) {
    if (!tvShowStorage.getTvShowByName(tvShowName)) {
        throw TvShowDoesNotExistError(tvShowName);
    }
    selectedTvShow = tvShowName;
}

std::string serio::core::TvShowViewer::getRawCrawlerOfSelectedTvShow() {
    std::string tvShowName = getSelectedTvShowNameOrFail();
    std::optional<std::string> rawCrawler = crawlerStorage.getTvShowCrawlerByTvShowName(tvShowName);
    if (rawCrawler) {
        return *rawCrawler;
    } else {
        throw DeletedTvShowStillSelectedError(tvShowName);
    }
}

std::string serio::core::TvShowViewer::getSelectedTvShowNameOrFail() const {
    if (!selectedTvShow) {
        throw NoTvShowSelectedError();
    }
    return *selectedTvShow;
}

void serio::core::TvShowViewer::crawlSelectedTvShow() {
    runtime.crawlTvShow(getSelectedTvShowNameOrFail());
}

void serio::core::TvShowViewer::clearSelectedTvShowWatchHistory() {
    tvShowStorage.clearTvShowWatchHistory(getSelectedTvShowNameOrFail());
}

serio::core::NoTvShowSelectedError::NoTvShowSelectedError() : std::logic_error("No tv show selected") {}

serio::core::TvShowDoesNotExistError::TvShowDoesNotExistError(const std::string &tvShowName)
    : logic_error("TV show '" + tvShowName + "' does not exist") {}

serio::core::DeletedTvShowStillSelectedError::DeletedTvShowStillSelectedError(const std::string &tvShowName)
    : logic_error("TV show '" + tvShowName + "' has been deleted, but is still selected in tv-show-viewer") {}
