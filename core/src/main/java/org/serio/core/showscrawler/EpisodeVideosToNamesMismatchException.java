package org.serio.core.showscrawler;

import java.util.List;

public class EpisodeVideosToNamesMismatchException extends RuntimeException {
    public EpisodeVideosToNamesMismatchException(List<String> episodeVideos, List<String> episodeNames) {
        super(String.format("Count of crawled episode video URLs (%d) does not match the count of crawled episode names (%d).\nCrawled episode videos: %s.\nCrawled episode names: %s", episodeVideos.size(), episodeNames.size(), episodeVideos, episodeNames));
    }
}
