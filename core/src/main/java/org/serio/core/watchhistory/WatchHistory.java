package org.serio.core.watchhistory;

import java.time.Duration;
import java.util.List;
import java.util.concurrent.Future;

/**
 * Keeps track of user's show and episode watch history.
 */
public interface WatchHistory {
    /**
     * Get information about the shows the user has watched.
     *
     * @return list of views of shows the user has watched
     */
    Future<List<ShowView>> getShowWatchHistory();

    /**
     * Get information about the episodes of the specified show the user has watched.
     *
     * @param showId ID of the show
     * @return list of views of episodes of the specified show that the user has watched. If the user hasn't watched
     * any episodes of the specified show - an empty array will be returned.
     */
    Future<List<EpisodeView>> getEpisodeWatchHistoryOfShow(String showId);

    /**
     * Indicate that the user has watched the specified episode of the specified show.
     *
     * @param showId ID of the show the user has just watched
     * @param episodeId ID of the episode the user has just watched
     * @param watchProgress amount of episode's duration the user has already watched
     */
    void watchShowEpisode(String showId, String episodeId, Duration watchProgress);

    /**
     * Clear user's watch history information about the specified show.
     *
     * @param showId ID of the show, watch history of which should be cleared
     */
    void clearWatchHistoryOfShow(String showId);
}
