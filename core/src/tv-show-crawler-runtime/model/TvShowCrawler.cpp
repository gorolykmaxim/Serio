#include <utility>
#include "tv-show-crawler-runtime/model/TvShowCrawler.h"

serio::core::TvShowCrawler::TvShowCrawler(std::string tvShowName, Crawler episodeVideoCrawler, Crawler thumbnailCrawler, Crawler episodeNameCrawler)
    : tvShowName(std::move(tvShowName)),
      episodeVideoCrawler(std::move(episodeVideoCrawler)),
      thumbnailCrawler(std::move(thumbnailCrawler)),
      episodeNameCrawler(std::move(episodeNameCrawler)) {}

std::string serio::core::TvShowCrawler::getTvShowName() const {
    return tvShowName;
}

serio::core::Crawler serio::core::TvShowCrawler::getEpisodeVideoCrawler() const {
    return episodeVideoCrawler;
}

serio::core::Crawler serio::core::TvShowCrawler::getThumbnailCrawler() const {
    return thumbnailCrawler;
}

serio::core::Crawler serio::core::TvShowCrawler::getEpisodeNameCrawler() const {
    return episodeNameCrawler;
}

bool serio::core::TvShowCrawler::operator==(const serio::core::TvShowCrawler &rhs) const {
    return tvShowName == rhs.tvShowName &&
           episodeVideoCrawler == rhs.episodeVideoCrawler &&
           thumbnailCrawler == rhs.thumbnailCrawler &&
           episodeNameCrawler == rhs.episodeNameCrawler;
}

bool serio::core::TvShowCrawler::operator!=(const serio::core::TvShowCrawler &rhs) const {
    return !(rhs == *this);
}

serio::core::EpisodeVideosToNamesMismatchError::EpisodeVideosToNamesMismatchError(unsigned int videoUrlsCount, unsigned int namesCount)
    : std::runtime_error("Can't assign crawled episode names to episode video URLs: video URLs count = " + std::to_string(videoUrlsCount) + " episode names count = " + std::to_string(namesCount)) {}

serio::core::TvShowCrawlerExecutionError::TvShowCrawlerExecutionError(const std::string &tvShowName,
                                                                      const std::exception &cause)
    : std::runtime_error("Failed to crawl '" + tvShowName + "': " + cause.what()) {}
