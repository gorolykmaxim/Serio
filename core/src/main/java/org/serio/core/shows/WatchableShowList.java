package org.serio.core.shows;

import org.serio.core.showstorage.ShowMetaData;
import org.serio.core.watchhistory.ShowView;

import java.util.*;
import java.util.stream.Collectors;

/**
 * Collection of {@link WatchableShowMetaData}.
 *
 * <p>Provides some advanced querying and filtering capabilities.</p>
 */
public class WatchableShowList {
    private final List<WatchableShowMetaData> allShows;

    /**
     * Create a collection of {@link WatchableShowMetaData} based of the specified list of shows and their views.
     *
     * @param shows list of shows
     * @param showViews views of the specified shows
     * @return collection of {@link WatchableShowMetaData}
     */
    public static WatchableShowList from(List<ShowMetaData> shows, List<ShowView> showViews) {
        Map<String, ShowView> showIdToView = new HashMap<>();
        showViews.forEach(showView -> showIdToView.put(showView.getShowId(), showView));
        return new WatchableShowList(shows
                .stream()
                .map(showMetaData -> new WatchableShowMetaData(showMetaData, showIdToView.get(showMetaData.getId().toString())))
                .collect(Collectors.toList()));
    }

    private WatchableShowList(List<WatchableShowMetaData> allShows) {
        this.allShows = Collections.unmodifiableList(allShows);
    }

    /**
     * Get all shows, stored in the collection.
     *
     * @return all shows, stored in the collection
     */
    public List<WatchableShowMetaData> getAllShows() {
        return allShows;
    }

    /**
     * Get shows, stored in the collection, that have been watched by the user.
     *
     * <p>Returned sub-set of shows will be ordered in such a way, were the most recently watched show
     * will go first.</p>
     *
     * @return shows watched by the user lately
     */
    public List<WatchableShowMetaData> getLastWatchedShows() {
        return allShows
                .stream()
                .filter(WatchableShowMetaData::hasBeenWatched)
                .sorted(Comparator.comparing(show -> show.getLastWatchedDate().get(), Comparator.reverseOrder()))
                .collect(Collectors.toList());
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        WatchableShowList that = (WatchableShowList) o;
        return Objects.equals(allShows, that.allShows);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int hashCode() {
        return Objects.hash(allShows);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String toString() {
        return "WatchableShowList{" +
                "allShows=" + allShows +
                '}';
    }
}
