package org.serio.core.shows;

import org.serio.core.showstorage.Show;
import org.serio.core.showstorage.ShowMetaData;
import org.serio.core.showstorage.ShowStorage;
import org.serio.core.watchhistory.EpisodeView;
import org.serio.core.watchhistory.ShowView;
import org.serio.core.watchhistory.WatchHistory;

import java.time.Duration;
import java.util.List;
import java.util.UUID;
import java.util.concurrent.Future;

/**
 * Module that operates on shows, their episodes and views.
 *
 * <p>To access show information, this module should be used instead of a plain {@link ShowStorage}.
 * It is a higher abstraction level than the {@link ShowStorage}, since {@link Shows} takes into
 * consideration what shows and episodes have been watched by the user and provides more querying options primarily
 * based on shows watch history.</p>
 */
public class Shows {
    private final ShowStorage showStorage;
    private final WatchHistory watchHistory;

    /**
     * Create module instance.
     *
     * @param showStorage the place where show and episode information will be stored
     * @param watchHistory the place where show and episode watch progress and history will be stored
     */
    public Shows(ShowStorage showStorage, WatchHistory watchHistory) {
        this.showStorage = showStorage;
        this.watchHistory = watchHistory;
    }

    /**
     * Find collection of all shows and their watch progresses.
     *
     * <p>Show information, returned by this method, will not include episodes due to performance considerations.</p>
     *
     * @return collection of all shows and their watch progresses
     */
    public WatchableShowList findAllShows() {
        try {
            Future<List<ShowMetaData>> allShowsFuture = showStorage.findAll();
            Future<List<ShowView>> showWatchHistoryFuture = watchHistory.getShowWatchHistory();
            return WatchableShowList.from(allShowsFuture.get(), showWatchHistoryFuture.get());
        } catch (Exception e) {
            throw new AllShowsLookupException(e);
        }
    }

    /**
     * Find show by it's ID.
     *
     * <p>Returned show will also contain information about it's episodes.</p>
     *
     * @param id ID of the show
     * @return show, it's episodes and information about their watch progress. Can't return null: the case where
     * there is no show with the specified ID is not expected and will produce an exception
     */
    public WatchableShow findShowById(UUID id) {
        try {
            Future<Show> showFuture = showStorage.findById(id);
            Future<List<EpisodeView>> episodeViewsFuture = watchHistory.getEpisodeWatchHistoryOfShow(id.toString());
            return WatchableShow.from(showFuture.get(), episodeViewsFuture.get());
        } catch (Exception e) {
            throw new ShowLookupException(id, e);
        }
    }

    /**
     * Create the specified show, if such does not already exist, or completely replace existing one with the new one.
     *
     * <p>In case of a complete show replacement, watch history will not be affected, thus a new show will inherit
     * watch history of the previous one.</p>
     *
     * @param show show to save
     */
    public void saveShow(Show show) {
        showStorage.save(show);
    }

    /**
     * Specify the watch progress of a user watching specified episode of the show.
     *
     * @param showId ID of the show, the user is watching
     * @param episodeId ID of the episode, the user is watching
     * @param watchProgress amount of the specified episode's duration the user has just watched
     */
    public void watchShowEpisode(UUID showId, long episodeId, Duration watchProgress) {
        watchHistory.watchShowEpisode(showId.toString(), Long.toString(episodeId), watchProgress);
    }

    /**
     * Clear user's watch history of the show with the specified ID by removing information about what episodes
     * the user has watched and for how long.
     *
     * <p>In case the user hasn't watched any of the episodes of the specified show,
     * this method call will be ignored.</p>
     *
     * @param showId ID of the show
     */
    public void clearWatchHistoryOfShow(UUID showId) {
        watchHistory.clearWatchHistoryOfShow(showId.toString());
    }

    /**
     * Delete the show with the specified ID.
     *
     * <p>This method does not affect watch history of the specified show, thus the history will be preserved.</p>
     *
     * @param showId ID of the show
     */
    public void deleteShow(UUID showId) {
        showStorage.deleteById(showId);
    }
}
