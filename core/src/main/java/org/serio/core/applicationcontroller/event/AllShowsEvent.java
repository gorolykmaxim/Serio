package org.serio.core.applicationcontroller.event;

import org.serio.core.applicationcontroller.model.DateFormat;
import org.serio.core.applicationcontroller.model.DisplayableShowMetaData;
import org.serio.core.shows.WatchableShowList;
import org.serio.core.userinterface.ApplicationEvent;
import org.serio.core.userinterface.ViewIds;

import java.util.List;
import java.util.Objects;
import java.util.stream.Collectors;

public class AllShowsEvent implements ApplicationEvent {
    private final List<DisplayableShowMetaData> allShows;
    private final List<DisplayableShowMetaData> lastWatchedShows;

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

    @Override
    public int getViewId() {
        return ViewIds.ALL_SHOWS;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        AllShowsEvent that = (AllShowsEvent) o;
        return Objects.equals(allShows, that.allShows) &&
                Objects.equals(lastWatchedShows, that.lastWatchedShows);
    }

    @Override
    public int hashCode() {
        return Objects.hash(allShows, lastWatchedShows);
    }

    @Override
    public String toString() {
        return "AllShowsEvent{" +
                "allShows=" + allShows +
                ", lastWatchedShows=" + lastWatchedShows +
                '}';
    }
}
