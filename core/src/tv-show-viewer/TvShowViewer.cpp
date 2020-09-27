#include <tv-show-viewer/TvShowViewer.h>

serio::core::TvShowViewer::TvShowViewer(serio::core::TvShowStorage &storage) : storage(storage) {}

serio::core::TvShow serio::core::TvShowViewer::getSelectedTvShow() const {
    std::string tvShowName = getSelectedTvShowNameOrFail();
    std::optional<serio::core::TvShow> tvShow = storage.getTvShowByName(tvShowName);
    if (!tvShow) {
        throw DeletedTvShowStillSelectedError(tvShowName);
    } else {
        return *tvShow;
    }
}

serio::core::ListPage<serio::core::Episode> serio::core::TvShowViewer::getTvShowEpisodes(unsigned int offset, unsigned int limit) {
    return storage.getEpisodesOfTvShowWithName(getSelectedTvShowNameOrFail(), offset, limit);
}

void serio::core::TvShowViewer::openTvShowWithName(const std::string &tvShowName) {
    if (!storage.getTvShowByName(tvShowName)) {
        throw TvShowDoesNotExistError(tvShowName);
    }
    selectedTvShow = tvShowName;
}

std::string serio::core::TvShowViewer::getSelectedTvShowNameOrFail() const {
    if (!selectedTvShow) {
        throw NoTvShowSelectedError();
    }
    return *selectedTvShow;
}

serio::core::NoTvShowSelectedError::NoTvShowSelectedError() : std::logic_error("No tv show selected") {}

serio::core::TvShowDoesNotExistError::TvShowDoesNotExistError(const std::string &tvShowName)
    : logic_error("TV show '" + tvShowName + "' does not exist") {}

serio::core::DeletedTvShowStillSelectedError::DeletedTvShowStillSelectedError(const std::string &tvShowName)
    : logic_error("TV show '" + tvShowName + "' has been deleted, but is still selected in tv-show-viewer") {}
