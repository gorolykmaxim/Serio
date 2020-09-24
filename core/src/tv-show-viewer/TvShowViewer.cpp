#include <tv-show-viewer/TvShowViewer.h>

serio::core::TvShowViewer::TvShowViewer(serio::core::TvShowStorage &storage) : storage(storage) {}

serio::core::TvShow serio::core::TvShowViewer::getSelectedTvShow() const {
    if (!selectedTvShow) {
        throw NoTvShowSelectedError();
    }
    return *selectedTvShow;
}

serio::core::ListPage<serio::core::Episode> serio::core::TvShowViewer::getTvShowEpisodes(unsigned int offset, unsigned int limit) {
    return storage.getEpisodesOfTvShowWithName(getSelectedTvShow().getName(), offset, limit);
}

void serio::core::TvShowViewer::openTvShowWithName(const std::string &tvShowName) {
    selectedTvShow = storage.getTvShowByName(tvShowName);
}

serio::core::NoTvShowSelectedError::NoTvShowSelectedError() : std::logic_error("No tv show selected") {}
