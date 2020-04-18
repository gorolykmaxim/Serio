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

public class Shows {
    private final ShowStorage showStorage;
    private final WatchHistory watchHistory;

    public Shows(ShowStorage showStorage, WatchHistory watchHistory) {
        this.showStorage = showStorage;
        this.watchHistory = watchHistory;
    }

    public WatchableShowList findAllShows() {
        try {
            Future<List<ShowMetaData>> allShowsFuture = showStorage.findAll();
            Future<List<ShowView>> showWatchHistoryFuture = watchHistory.getShowWatchHistory();
            return WatchableShowList.from(allShowsFuture.get(), showWatchHistoryFuture.get());
        } catch (Exception e) {
            throw new AllShowsLookupException(e);
        }
    }

    public WatchableShow findShowById(UUID id) {
        try {
            Future<Show> showFuture = showStorage.findById(id);
            Future<List<EpisodeView>> episodeViewsFuture = watchHistory.getEpisodeWatchHistoryOfShow(id.toString());
            return WatchableShow.from(showFuture.get(), episodeViewsFuture.get());
        } catch (Exception e) {
            throw new ShowLookupException(id, e);
        }
    }

    public void saveShow(Show show) {
        showStorage.save(show);
    }

    public void watchShowEpisode(UUID showId, long episodeId, Duration watchProgress) {
        watchHistory.watchShowEpisode(showId.toString(), Long.toString(episodeId), watchProgress);
    }

    public void clearWatchHistoryOfShow(UUID showId) {
        watchHistory.clearWatchHistoryOfShow(showId.toString());
    }

    public void deleteShow(UUID showId) {
        showStorage.deleteById(showId);
    }
}
