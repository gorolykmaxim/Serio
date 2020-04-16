package org.serio.core.watchhistory;

import java.time.Duration;
import java.util.List;
import java.util.Optional;

public interface WatchHistory {
    List<ShowView> getShowWatchHistory();
    Optional<ShowView> getShowViewById(String showId);
    List<EpisodeView> getEpisodeWatchHistoryOfShow(String showId);
    void watchShowEpisode(String showId, String episodeId, Duration watchProgress);
    void clearWatchHistoryOfShow(String showId);
}
