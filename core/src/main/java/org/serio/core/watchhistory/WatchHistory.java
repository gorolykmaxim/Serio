package org.serio.core.watchhistory;

import java.time.Duration;
import java.util.List;
import java.util.concurrent.Future;

public interface WatchHistory {
    Future<List<ShowView>> getShowWatchHistory();
    Future<List<EpisodeView>> getEpisodeWatchHistoryOfShow(String showId);
    void watchShowEpisode(String showId, String episodeId, Duration watchProgress);
    void clearWatchHistoryOfShow(String showId);
}
