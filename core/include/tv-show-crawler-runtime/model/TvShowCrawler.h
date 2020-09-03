#ifndef SERIO_TVSHOWCRAWLER_H
#define SERIO_TVSHOWCRAWLER_H

#include <string>
#include "Crawler.h"

namespace serio::core {

class TvShowCrawler {
public:
    TvShowCrawler(std::string tvShowName, Crawler episodeVideoCrawler, Crawler thumbnailCrawler = Crawler(), Crawler episodeNameCrawler = Crawler());
    [[nodiscard]] std::string getTvShowName() const;
    [[nodiscard]] Crawler getEpisodeVideoCrawler() const;
    [[nodiscard]] Crawler getThumbnailCrawler() const;
    [[nodiscard]] Crawler getEpisodeNameCrawler() const;
    bool operator==(const TvShowCrawler &rhs) const;
    bool operator!=(const TvShowCrawler &rhs) const;
private:
    std::string tvShowName;
    Crawler episodeVideoCrawler;
    Crawler thumbnailCrawler;
    Crawler episodeNameCrawler;
};

class EpisodeVideosToNamesMismatchError : public std::runtime_error {
public:
    EpisodeVideosToNamesMismatchError(unsigned int videoUrlsCount, unsigned int namesCount);
};

class TvShowCrawlerExecutionError: public std::runtime_error {
public:
    TvShowCrawlerExecutionError(const std::string& tvShowName, const std::exception& cause);
};

}

#endif //SERIO_TVSHOWCRAWLER_H
