package org.serio.core.shows;

import java.util.Collections;
import java.util.Comparator;
import java.util.List;
import java.util.stream.Collectors;

public class WatchableShowList {
    private final List<WatchableShowMetaData> allShows;

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
