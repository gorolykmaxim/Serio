package org.serio.core.applicationcontroller.event;

import org.serio.core.applicationcontroller.model.DateFormat;
import org.serio.core.applicationcontroller.model.DisplayableShowMetaData;
import org.serio.core.shows.WatchableShowList;
import org.serio.core.userinterface.ApplicationEvent;
import org.serio.core.userinterface.ViewIds;

import java.util.List;
import java.util.Objects;
import java.util.stream.Collectors;

/**
 * Event that will trigger rendering of {@link ViewIds#ALL_SHOWS} view.
 */
public class AllShowsEvent implements ApplicationEvent {
    private final List<DisplayableShowMetaData> allShows;
    private final List<DisplayableShowMetaData> lastWatchedShows;

    /**
     * Construct an event.
     *
     * @param showList list of all shows, that can be watched by the user, to be rendered
     * @param lastWatchedDateFormat date format to be applied to show's last watched dates
     */
    public AllShowsEvent(WatchableShowList showList, DateFormat lastWatchedDateFormat) {
        allShows = showList.getAllShows()
                .stream()
                .map(show -> new DisplayableShowMetaData(show, lastWatchedDateFormat))
                .collect(Collectors.toList());
        lastWatchedShows = showList.getLastWatchedShows()
                .stream()
                .map(show -> new DisplayableShowMetaData(show, lastWatchedDateFormat))
                .collect(Collectors.toList());
    }

    /**
     * @see WatchableShowList#getAllShows()
     */
    public List<DisplayableShowMetaData> getAllShows() {
        return allShows;
    }

    /**
     * @see WatchableShowList#getLastWatchedShows()
     */
    public List<DisplayableShowMetaData> getLastWatchedShows() {
        return lastWatchedShows;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getViewId() {
        return ViewIds.ALL_SHOWS;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        AllShowsEvent that = (AllShowsEvent) o;
        return Objects.equals(allShows, that.allShows) &&
                Objects.equals(lastWatchedShows, that.lastWatchedShows);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int hashCode() {
        return Objects.hash(allShows, lastWatchedShows);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String toString() {
        return "AllShowsEvent{" +
                "allShows=" + allShows +
                ", lastWatchedShows=" + lastWatchedShows +
                '}';
    }
}
