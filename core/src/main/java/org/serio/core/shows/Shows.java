package org.serio.core.shows;

import org.serio.core.showstorage.Show;
import org.serio.core.showstorage.ShowMetaData;
import org.serio.core.showstorage.ShowStorage;
import org.serio.core.watchhistory.EpisodeView;
import org.serio.core.watchhistory.ShowView;
import org.serio.core.watchhistory.WatchHistory;

import java.time.Duration;
import java.util.*;
import java.util.stream.Collectors;

public class Shows {
    private final ShowStorage showStorage;
    private final WatchHistory watchHistory;

    public Shows(ShowStorage showStorage, WatchHistory watchHistory) {
        this.showStorage = showStorage;
        this.watchHistory = watchHistory;
    }

    public WatchableShowList findAllShows() {
        try {
            List<ShowMetaData> allShows = showStorage.findAll();
            List<ShowView> showWatchHistory = watchHistory.getShowWatchHistory();
            Map<String, ShowView> showIdToView = new HashMap<>();
            showWatchHistory.forEach(showView -> showIdToView.put(showView.getShowId(), showView));
            List<WatchableShowMetaData> allWatchableShows = allShows
                    .stream()
                    .map(showMetaData -> new WatchableShowMetaData(showMetaData, showIdToView.get(showMetaData.getId().toString())))
                    .collect(Collectors.toList());
            return new WatchableShowList(allWatchableShows);
        } catch (Exception e) {
            throw new AllShowsLookupException(e);
        }
    }

    public WatchableShow findShowById(UUID id) {
        try {
            Show show = showStorage.findById(id);
            String showId = id.toString();
            Optional<ShowView> possibleShowView = watchHistory.getShowViewById(showId);
            List<EpisodeView> episodeViews = watchHistory.getEpisodeWatchHistoryOfShow(showId);
            return WatchableShow.from(show, possibleShowView.orElse(null), episodeViews);
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
