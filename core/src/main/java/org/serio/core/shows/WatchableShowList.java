package org.serio.core.shows;

import org.serio.core.showstorage.ShowMetaData;
import org.serio.core.watchhistory.ShowView;

import java.util.*;
import java.util.stream.Collectors;

public class WatchableShowList {
    private final List<WatchableShowMetaData> allShows;

    static WatchableShowList from(List<ShowMetaData> allShows, List<ShowView> showViews) {
        Map<String, ShowView> showIdToView = new HashMap<>();
        showViews.forEach(showView -> showIdToView.put(showView.getShowId(), showView));
        return new WatchableShowList(allShows
                .stream()
                .map(showMetaData -> new WatchableShowMetaData(showMetaData, showIdToView.get(showMetaData.getId().toString())))
                .collect(Collectors.toList()));
    }

    WatchableShowList(List<WatchableShowMetaData> allShows) {
        this.allShows = Collections.unmodifiableList(allShows);
    }

    public List<WatchableShowMetaData> getAllShows() {
        return allShows;
    }

    public List<WatchableShowMetaData> getLastWatchedShows() {
        return allShows
                .stream()
                .filter(WatchableShowMetaData::hasBeenWatched)
                .sorted(Comparator.comparing(show -> show.getLastWatchedDate().get(), Comparator.reverseOrder()))
                .collect(Collectors.toList());
    }
}
